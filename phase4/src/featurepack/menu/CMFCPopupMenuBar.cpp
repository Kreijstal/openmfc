// CMFCPopupMenuBar — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

// Symbol: ?ImportFromMenu@CMFCPopupMenuBar@@UEAAHPEAUHMENU__@@H@Z
extern "C" int MS_ABI impl__ImportFromMenu_CMFCPopupMenuBar__UEAAHPEAUHMENU____H_Z(CMFCPopupMenuBar* pThis, HMENU hMenu, int bShowAllCommands) {
    return pThis ? pThis->ImportFromMenu(hMenu, bShowAllCommands) : FALSE;
}
// Symbol: ?ExportToMenu@CMFCPopupMenuBar@@UEBAPEAUHMENU__@@XZ
extern "C" HMENU MS_ABI impl__ExportToMenu_CMFCPopupMenuBar__UEBAPEAUHMENU____XZ(const CMFCPopupMenuBar* pThis) {
    return pThis ? pThis->ExportToMenu() : nullptr;
}
// Symbol: ?BuildOrigItems@CMFCPopupMenuBar@@QEAAHI@Z
extern "C" int MS_ABI impl__BuildOrigItems_CMFCPopupMenuBar__QEAAHI_Z(CMFCPopupMenuBar* pThis, unsigned int uiMenuResID) {
    return pThis ? pThis->BuildOrigItems(uiMenuResID) : FALSE;
}
// Symbol: ?GetGutterWidth@CMFCPopupMenuBar@@QEAAHXZ
extern "C" int MS_ABI impl__GetGutterWidth_CMFCPopupMenuBar__QEAAHXZ(CMFCPopupMenuBar* pThis) {
    return pThis ? pThis->GetGutterWidth() : 0;
}
// Symbol: ?CalcSize@CMFCPopupMenuBar@@MEAA?AVCSize@@H@Z
extern "C" void MS_ABI impl__CalcSize_CMFCPopupMenuBar__MEAA_AVCSize__H_Z(CSize* pRet, CMFCPopupMenuBar* pThis, int bVertDock) {
    new (pRet) CSize(pThis ? pThis->CalcSize(bVertDock) : CSize());
}
// Symbol: ?AdjustLayout@CMFCPopupMenuBar@@MEAAXXZ
extern "C" void MS_ABI impl__AdjustLayout_CMFCPopupMenuBar__MEAAXXZ(CMFCPopupMenuBar* pThis) {
    if (pThis) pThis->AdjustLayout();
}
// Symbol: ?AdjustLocations@CMFCPopupMenuBar@@MEAAXXZ
extern "C" void MS_ABI impl__AdjustLocations_CMFCPopupMenuBar__MEAAXXZ(CMFCPopupMenuBar* pThis) {
    if (pThis) pThis->AdjustLocations();
}
// Symbol: ?CloseDelayedSubMenu@CMFCPopupMenuBar@@UEAAXXZ
extern "C" void MS_ABI impl__CloseDelayedSubMenu_CMFCPopupMenuBar__UEAAXXZ(CMFCPopupMenuBar* pThis) {
    if (pThis) pThis->CloseDelayedSubMenu();
}
CMFCPopupMenuBar::CMFCPopupMenuBar() {
    memset(_mfcpopupmenubar_padding, 0, sizeof(_mfcpopupmenubar_padding));
}
CMFCPopupMenuBar::~CMFCPopupMenuBar() {
    ClearPopupMenuBarState(this);
}
BOOL CMFCPopupMenuBar::ImportFromMenu(HMENU hMenu, BOOL) {
    PopupMenuBarState& state = g_popupMenuBarStates[this];
    state.menu = hMenu;
    ImportMenuIntoPopupItems(hMenu, state.items, state.ownedItems);
    return TRUE;
}
HMENU CMFCPopupMenuBar::ExportToMenu() const {
    auto it = g_popupMenuBarStates.find(this);
    if (it == g_popupMenuBarStates.end()) return nullptr;
    if (it->second.menu) return it->second.menu;

    HMENU hMenu = ::CreatePopupMenu();
    for (CMFCToolBarMenuButton* item : it->second.items) {
        if (!item) continue;
        if (item->m_hMenu) {
            ::AppendMenuW(hMenu, MF_POPUP, reinterpret_cast<UINT_PTR>(item->m_hMenu), static_cast<const wchar_t*>(item->m_strText));
        } else {
            ::AppendMenuW(hMenu, MF_STRING, static_cast<UINT_PTR>(item->m_nID), static_cast<const wchar_t*>(item->m_strText));
        }
    }
    return hMenu;
}
BOOL CMFCPopupMenuBar::BuildOrigItems(UINT) { return TRUE; }
CMFCToolBarMenuButton* CMFCPopupMenuBar::GetMenuItem(int nIndex) const {
    auto it = g_popupMenuBarStates.find(this);
    if (it == g_popupMenuBarStates.end()) return nullptr;
    if (nIndex < 0 || nIndex >= static_cast<int>(it->second.items.size())) return nullptr;
    return it->second.items[nIndex];
}
int CMFCPopupMenuBar::GetGutterWidth() const { return 22; }
CSize CMFCPopupMenuBar::CalcSize(BOOL) {
    auto it = g_popupMenuBarStates.find(this);
    const int count = it == g_popupMenuBarStates.end() ? 0 : static_cast<int>(it->second.items.size());
    return CSize(160, std::max(22, count * 22));
}
void CMFCPopupMenuBar::AdjustLayout() {
    PopupMenuBarState& state = g_popupMenuBarStates[this];
    if (GetSafeHwnd()) {
        ::InvalidateRect(GetSafeHwnd(), nullptr, TRUE);
        ::UpdateWindow(GetSafeHwnd());
    }
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    g_paneCoreState_Mfccore[this].recentRect = CRect(0, 0, 160, std::max(22, static_cast<int>(state.items.size()) * 22));
}
void CMFCPopupMenuBar::AdjustLocations() {
    AdjustLayout();
}
void CMFCPopupMenuBar::CloseDelayedSubMenu() {
    PopupMenuBarState& state = g_popupMenuBarStates[this];
    state.delayedSubMenuOpen = false;
    state.selected = -1;
}

// ===========================================================================
// The bodies below are transcribed from the retail mfc140u.dll exports (RVA
// cited at each function; every address is an mfc140u address resolved through
// the export table).  They address the RETAIL CMFCPopupMenuBar layout on the
// standing assumption of this directory (see the object-model note at the top
// of CMFCPopupMenu.cpp): the caller's object is retail-sized.
//
// Retail own-member layout, read off the constructor (??0CMFCPopupMenuBar@@QEAA@XZ,
// RVA 0xbbf80, mfc140u) in the declaration order of the on-host MSVC 14.51
// afxpopupmenubar.h.  The CMFCToolBar subobject ends at 0x1350 (pinned by the
// static_assert below); the retail object is 0x1420 bytes, which is also what
// featurepack/menu/RuntimeClasses.cpp declares as m_nObjectSize (5152).
// OpenMFC's clean-room class is only 0x13b0 bytes (_mfcpopupmenubar_padding[96]),
// so the members from m_arColumns (0x13b0) on lie OUTSIDE an object that OpenMFC
// itself allocates -- growing that padding to 208 bytes is a header change this
// file cannot make (reported as a header request).
// ===========================================================================

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cwchar>
#include <cstdlib>

namespace {
constexpr int kOffHWnd                        = 0x40;    // CWnd::m_hWnd
constexpr int kOffHWndOwner                   = 0xa0;    // CWnd::m_hWndOwner (not named by OpenMFC's CWnd; see OwnerHwnd)
constexpr int kOffAccelKeys                   = 0x1318;  // CMFCToolBar: CMap<UINT,UINT,CMFCToolBarButton*,CMFCToolBarButton*> (OpenMFC: char m_AccelKeys[56], zeroed by the ctor)

constexpr int kOffDisableSideBarInXPMode      = 0x1350;  // BOOL  m_bDisableSideBarInXPMode   (ctor: 0)
constexpr int kOffRelatedToolbar              = 0x1358;  // CMFCToolBar* m_pRelatedToolbar    (ctor: NULL)
constexpr int kOffDefaultMenuCmdId            = 0x1360;  // UINT  m_uiDefaultMenuCmdId        (ctor: 0)
constexpr int kOffOffset                      = 0x1364;  // int   m_iOffset                   (ctor: 0)
constexpr int kOffSeparatorOffsetLeft         = 0x1368;  // int   m_xSeparatorOffsetLeft      (ctor: 0)
constexpr int kOffSeparatorOffsetRight        = 0x136c;  // int   m_xSeparatorOffsetRight     (ctor: 0)
constexpr int kOffMaxWidth                    = 0x1370;  // int   m_iMaxWidth                 (ctor: -1)
constexpr int kOffMinWidth                    = 0x1374;  // int   m_iMinWidth                 (ctor: -1)
constexpr int kOffFirstClick                  = 0x1378;  // BOOL  m_bFirstClick               (ctor: TRUE)
constexpr int kOffFirstMove                   = 0x137c;  // BOOL  m_bFirstMove                (ctor: TRUE)
constexpr int kOffAreAllCommandsShown         = 0x1380;  // BOOL  m_bAreAllCommandsShown      (ctor: TRUE)
constexpr int kOffInCommand                   = 0x1384;  // BOOL  m_bInCommand                (ctor: FALSE)
constexpr int kOffTrackMode                   = 0x1388;  // BOOL  m_bTrackMode                (ctor: FALSE)
constexpr int kOffIsClickOutsideItem          = 0x138c;  // BOOL  m_bIsClickOutsideItem       (ctor: TRUE)
constexpr int kOffPaletteMode                 = 0x1390;  // BOOL  m_bPaletteMode              (ctor: FALSE)
constexpr int kOffPaletteRows                 = 0x1394;  // BOOL  m_bPaletteRows              (ctor: TRUE)
constexpr int kOffDropDownListMode            = 0x1398;  // BOOL  m_bDropDownListMode         (ctor: FALSE)
constexpr int kOffDropDownPageSize            = 0x139c;  // BOOL  m_nDropDownPageSize         (ctor: 0)
constexpr int kOffInScrollMode                = 0x13a0;  // BOOL  m_bInScrollMode             (ctor: FALSE)
constexpr int kOffResizeTracking              = 0x13a4;  // BOOL  m_bResizeTracking           (ctor: FALSE)
constexpr int kOffPtCursor                    = 0x13a8;  // CPoint m_ptCursor                 (ctor: (-1,-1))
constexpr int kOffArColumns                   = 0x13b0;  // CUIntArray m_arColumns (0x28 bytes; vftable slot 0 = ?GetRuntimeClass@CUIntArray@@)
constexpr int kOffDelayedPopupMenuButton      = 0x13d8;  // CMFCToolBarMenuButton* m_pDelayedPopupMenuButton      (ctor: NULL)
constexpr int kOffDelayedClosePopupMenuButton = 0x13e0;  // CMFCToolBarMenuButton* m_pDelayedClosePopupMenuButton (ctor: NULL)
constexpr int kOffHiddenItemsAccel            = 0x13e8;  // CMap<UINT,UINT,UINT,UINT> m_HiddenItemsAccel (0x38 bytes; ctor: 17 buckets, block size 10)
constexpr int kRetailPopupMenuBarSize         = 0x1420;

static_assert(sizeof(CMFCToolBar) == kOffDisableSideBarInXPMode,
              "CMFCPopupMenuBar's own members start where the CMFCToolBar subobject ends (0x1350)");
static_assert(kOffPtCursor + 8 == 0x13b0, "m_ptCursor is the last member inside OpenMFC's 0x13b0-byte class");
static_assert(sizeof(CMFCPopupMenuBar) <= kRetailPopupMenuBarSize,
              "OpenMFC's CMFCPopupMenuBar must never be larger than the retail object");
static_assert(kOffHiddenItemsAccel + 0x38 == kRetailPopupMenuBarSize, "retail CMFCPopupMenuBar is 0x1420 bytes");

// Retail CMFCPopupMenu members read here (afxpopupmenu.h order, the same
// offsets CMFCPopupMenu.cpp pins against detail/CMFCPopupMenuSupport.h).
constexpr int kOffPopupMessageWnd             = 0x1f8;   // CWnd*  m_pMessageWnd
constexpr int kOffPopupParentBtn              = 0x228;   // CMFCToolBarMenuButton* m_pParentBtn
constexpr int kOffPopupHMenu                  = 0x1658;  // HMENU  m_hMenu
constexpr int kOffPopupParentRibbonElement    = 0x19b8;  // CMFCRibbonBaseElement* m_pParentRibbonElement

// Retail CMFCToolBarMenuButton members (afxtoolbarmenubutton.h order: the
// 0x88-byte CMFCToolBarButton, then CObList m_listCommands at 0x88, so):
constexpr int kOffMbPopupMenu                 = 0xc0;    // CMFCPopupMenu* m_pPopupMenu   (IsDroppedDown() is `m_pPopupMenu != NULL`, RVA 0x2a4c0)
constexpr int kOffMbDrawDownArrow             = 0xc8;    // BOOL m_bDrawDownArrow
constexpr int kOffMbToBeClosed                = 0xdc;    // BOOL m_bToBeClosed
constexpr int kOffMbRectButton                = 0x118;   // CRect m_rectButton
static_assert(sizeof(CMFCToolBarMenuButton) == 0x128, "retail CMFCToolBarMenuButton is 0x128 bytes");

// Retail CCommandManager (afxcommandmanager.h): vfptr, then five containers.
constexpr int kOffCmdMgrCommandsWithoutImages = 0xb0;    // CList<UINT,UINT> m_lstCommandsWithoutImages
// CList<UINT,UINT> / CObList: m_pNodeHead at +8; CNode: pNext +0, pPrev +8, data +0x10.
constexpr int kOffListNodeHead                = 0x08;
constexpr int kOffNodeNext                    = 0x00;
constexpr int kOffNodeData                    = 0x10;

// Timer ids and private messages the bodies use (afxpopupmenubar.cpp / afxres.h;
// none of them is spelled in the OpenMFC public headers).
constexpr UINT_PTR kTimerDelayedPopup         = 0xec17;  // uiPopupTimerEvent
constexpr UINT_PTR kTimerDelayedClose         = 0xec18;  // uiRemovePopupTimerEvent
constexpr UINT_PTR kTimerAccHot               = 0xec07;  // CMFCToolBar accessibility hot-item timer
constexpr UINT     kWmSetMessageString        = 0x362;   // WM_SETMESSAGESTRING
constexpr UINT     kAfxIdsIdleMessage         = 0xe001;  // AFX_IDS_IDLEMESSAGE
constexpr unsigned kTbbsChecked               = 0x10000; // TBBS_CHECKED  = MAKELONG(0, TBSTATE_CHECKED)
constexpr unsigned kTbbsPressed               = 0x20000; // TBBS_PRESSED  = MAKELONG(0, TBSTATE_PRESSED)
constexpr unsigned kTbbsDisabled              = 0x40000; // TBBS_DISABLED = MAKELONG(0, TBSTATE_ENABLED)
constexpr unsigned kTbbsSeparator             = 0x1;     // TBBS_SEPARATOR = TBSTYLE_SEP
// AUX_DATA::cxBorder2 / cyBorder2 (afxData + 0x10 / + 0x14, i.e. 0x3c32d0 /
// 0x3c32d4 in mfc140u): the AUX_DATA static initializer at RVA 0x2270
// (mfc140u) stores 2 to both.  OpenMFC's exported afxData is a zero blob, so
// the retail values are used as constants (same choice as CMFCVisualManager.cpp).
constexpr int kAuxDataCxBorder2               = 2;
constexpr int kAuxDataCyBorder2               = 2;
constexpr unsigned kIdAfxBarResToolbarImage        = 0x4212; // ID_AFXBARRES_TOOLBAR_IMAGE
constexpr unsigned kIdAfxBarResToolbarText         = 0x4213; // ID_AFXBARRES_TOOLBAR_TEXT
constexpr unsigned kIdAfxBarResToolbarImageAndText = 0x4214; // ID_AFXBARRES_TOOLBAR_IMAGE_AND_TEXT

template <class T> inline T& At(void* p, int off) {
    return *reinterpret_cast<T*>(static_cast<char*>(p) + off);
}
template <class T> inline const T& At(const void* p, int off) {
    return *reinterpret_cast<const T*>(static_cast<const char*>(p) + off);
}
inline HWND HWndOf(const void* p) { return At<HWND>(p, kOffHWnd); }
inline CWnd* AsWnd(void* p) { return static_cast<CWnd*>(static_cast<CMFCPopupMenuBar*>(p)); }
inline CMFCToolBar* AsToolBar(void* p) { return static_cast<CMFCToolBar*>(static_cast<CMFCPopupMenuBar*>(p)); }

// CPoint passed BY VALUE is an 8-byte aggregate in a register under the MS x64
// ABI: x in the low dword, y in the high dword.
inline CPoint UnpackPoint(long long v) {
    return CPoint(static_cast<int>(static_cast<std::uint32_t>(v & 0xffffffffu)),
                  static_cast<int>(static_cast<std::uint32_t>(static_cast<std::uint64_t>(v) >> 32)));
}
} // namespace

// Class statics with storage in featurepack/menu/StaticData.cpp,
// featurepack/toolbar/StaticData.cpp, featurepack/toolbar/CMFCToolBar.cpp,
// core/runtime/StaticData.cpp and featurepack/CMFC_misc_stubs.cpp.  The retail
// addresses are the mfc140u ones read by the bodies below.
extern "C" std::uint32_t impl__m_uiPopupTimerDelay_CMFCPopupMenuBar__1IA;          // 0x1803b1b38
extern "C" std::int32_t  impl__m_nLastCommandIndex_CMFCPopupMenuBar__1HA;          // 0x1803b1b3c
extern "C" std::int32_t  impl__m_bCustomizeMode_CMFCToolBar__1HA;                  // 0x1803be35c
extern "C" void*         impl__m_pUserImages_CMFCToolBar__1PEAVCMFCToolBarImages__EA; // 0x1803be370
extern "C" unsigned char impl__m_UsageCount_CMFCToolBar__1VCMFCCmdUsageCount__A[80];  // 0x1803b1fd0
extern "C" std::int32_t  impl__m_bRecentlyUsedMenus_CMFCMenuBar__1HA;              // 0x1803b1b08
extern "C" std::int32_t  impl__m_bSendMenuSelectMsg_CMFCPopupMenu__1HA;            // 0x1803be298
extern "C" void*         impl__g_pTopLevelFrame__3PEAVCFrameWnd__EA;               // 0x1803be1b8
extern "C" void*         impl__afxUserToolsManager__3PEAVCUserToolsManager__EA;    // 0x1803be3b0
extern "C" void*         impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;         // 0x1803be3c0
extern "C" unsigned char impl__afxMenuHash__3VCMenuHash__A[128];                   // 0x1803b1d10 (72-byte CMenuHash; storage is the upper bound)

// Cross-file thunks.  Each was grepped to its definition; parameter lists are
// derived from the mangled names.
//   core/runtime/CObject.cpp                : IsKindOf
//   core/window/CWnd.cpp                    : FromHandle, ShowWindow, DestroyWindow, IsFrameWnd
//   core/window/Thunks.cpp                  : Default, GetStyle, GetParentFrame, GetTopLevelFrame, GetCurrentMessage
//   core/frame/CFrameWnd.cpp                : CFrameWnd::GetMessageString
//   core/app/Globals.cpp / CWinThread.cpp   : AfxGetThread, CWinThread::GetMainWnd
//   core/app/CSettingsStoreSP.cpp / CSettingsStore.cpp : CSettingsStoreSP::Create, CSettingsStore::Open / Read(CString&)
//   core/gdi/CDC.cpp                        : CDC::SelectObject(CPen*), MoveTo, LineTo
//   core/window/CMenuHash.cpp               : CMenuHash::LoadMenuBar
//   detail/MfcExceptionsSupport.cpp         : AfxThrowInvalidArgException
//   detail/MemcoreSupport.cpp               : ??2@YAPEAX_K@Z / ??3@YAXPEAX@Z (the exported operator new/delete retail calls)
//   featurepack/CMFC_misc_stubs.cpp         : AFXGetParentFrame, AFXPlaySystemSound, GetCmdMgr
//   featurepack/menu/RuntimeClasses.cpp / toolbar/RuntimeClasses.cpp /
//   controls/RuntimeClasses.cpp / customize/RuntimeClasses.cpp : GetThisClass getters
//   featurepack/menu/CMFCPopupMenu.cpp      : GetParentPopupMenu, GetParentToolBar, ActivatePopupMenu, PostCommand, SaveState
//   featurepack/toolbar/CMFCToolBar.cpp     : the CMFCToolBar base handlers and helpers named below
//   featurepack/toolbar/Thunks.cpp          : GetButton, GetCount, ~CMFCToolBarButton, ~CMFCToolBarMenuButton
//   featurepack/toolbar/CMFCToolBarButton.cpp : CreateFromOleData (returns NULL today), ExportToMenuButton (stub),
//                                               CMFCToolBarButton::OnToolHitTest (stub) -- all three carry
//                                               auto-generated parameter lists; the declarations below are the
//                                               ones derived from the mangled names
//   featurepack/toolbar/CMFCToolBarMenuButton.cpp : OnClick / OpenPopupMenu / OnCancelMode / GetImageRect (stubs
//                                               with auto-generated parameter lists, same remark)
//   detail/CbarcoreSupport.cpp              : ??0CMFCToolBarMenuButton@@QEAA@IPEAUHMENU__@@HPEB_WH@Z
//   featurepack/customize/CMFCCustomizeButton.cpp : InvokeCommand
//   featurepack/customize/CUserToolsManager.cpp   : InvokeTool
//   featurepack/customize/CMFCCmdUsageCount.cpp   : AddCmd
//   featurepack/customize/CCommandManager.cpp     : EnableMenuItemImage (stub, auto-generated list)
//   featurepack/customize/CTooltipManager.cpp     : SetTooltipText
//   featurepack/customize/CKeyboardManager.cpp    : IsKeyPrintable / TranslateCharToUpper
//   featurepack/ribbon/CMFCRibbonBaseElement.cpp  : SetDroppedDown / PostMenuCommand (stubs, auto-generated lists)
//   featurepack/visualmanager/Thunks.cpp          : OnDrawSeparator (dispatches virtually on the C++ manager)
//   featurepack/visualmanager/CMFCVisualManager.cpp : OnHighlightRarelyUsedMenuItems (base body only)
//   featurepack/docking/CBasePane.cpp             : get_accRole / get_accState
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);
extern "C" int MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* pThis);
extern "C" int MS_ABI impl__IsFrameWnd_CWnd__UEBAHXZ(const CWnd* pThis);
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);
extern "C" CFrameWnd* MS_ABI impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);
extern "C" const MSG* MS_ABI impl__GetCurrentMessage_CWnd__KAPEBUtagMSG__XZ();
extern "C" void MS_ABI impl__GetMessageString_CFrameWnd__UEBAXIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CFrameWnd* pThis, unsigned int nID, CString* rMessage);
extern "C" CWinThread* MS_ABI impl__AfxGetThread__YAPEAVCWinThread__XZ();
extern "C" CWnd* MS_ABI impl__GetMainWnd_CWinThread__UEAAPEAVCWnd__XZ(CWinThread* pThis);
extern "C" void* MS_ABI impl__Create_CSettingsStoreSP__QEAAAEAVCSettingsStore__HH_Z(void* pThis, int bAdmin, int bReadOnly);
extern "C" int MS_ABI impl__Open_CSettingsStore__UEAAHPEB_W_Z(void* pThis, const wchar_t* pszPath);
extern "C" int MS_ABI impl__Read_CSettingsStore__UEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pThis, const wchar_t* pszKey, CString* pValue);
extern "C" CPen* MS_ABI impl__SelectObject_CDC__QEAAPEAVCPen__PEAV2__Z(CDC* pThis, CPen* pPen);
extern "C" CPoint MS_ABI impl__MoveTo_CDC__QEAA_AVCPoint__HH_Z(CDC* pThis, int x, int y);
extern "C" int MS_ABI impl__LineTo_CDC__QEAAHHH_Z(CDC* pThis, int x, int y);
extern "C" int MS_ABI impl__LoadMenuBar_CMenuHash__QEAAHPEAUHMENU____PEAVCMFCToolBar___Z(CMenuHash* pThis, HMENU hMenu, CMFCToolBar* pBar);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);      // ??2@YAPEAX_K@Z (detail/MemcoreSupport.cpp)
extern "C" void  MS_ABI impl___3_YAXPEAX_Z(void* ptr);            // ??3@YAXPEAX@Z
extern "C" CFrameWnd* MS_ABI impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(const CWnd* pWnd);
extern "C" void MS_ABI impl__AFXPlaySystemSound__YAXH_Z(int nSound);
extern "C" void* MS_ABI impl__GetCmdMgr__YAPEAVCCommandManager__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCPopupMenu__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBarButton__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBarMenuButton__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCCustomizeMenuButton__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCCustomizeButton__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCShowAllButton__SAPEAUCRuntimeClass__XZ();
extern "C" void* MS_ABI impl__GetParentPopupMenu_CMFCPopupMenu__QEBAPEAV1_XZ(const void* pThis);
extern "C" CMFCToolBar* MS_ABI impl__GetParentToolBar_CMFCPopupMenu__QEBAPEAVCMFCToolBar__XZ(const void* pThis);
extern "C" int MS_ABI impl__ActivatePopupMenu_CMFCPopupMenu__SAHPEAVCFrameWnd__PEAV1__Z(CFrameWnd* pTopFrame, CMFCPopupMenu* pPopupMenu);
extern "C" int MS_ABI impl__PostCommand_CMFCPopupMenu__QEAAHI_Z(void* pThis, unsigned int uiCommandID);
extern "C" void MS_ABI impl__SaveState_CMFCPopupMenu__UEAAXXZ(void* pThis);
extern "C" int MS_ABI impl__OnCreate_CMFCToolBar__IEAAHPEAUtagCREATESTRUCTW___Z(CMFCToolBar* pThis, LPCREATESTRUCTW lpCreateStruct);
extern "C" void MS_ABI impl__OnDestroy_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis);
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCToolBar__IEAAXIVCPoint___Z(CMFCToolBar* pThis, unsigned int nFlags, CPoint point);
extern "C" void MS_ABI impl__OnLButtonDown_CMFCToolBar__IEAAXIVCPoint___Z(CMFCToolBar* pThis, unsigned int nFlags, CPoint point);
extern "C" void MS_ABI impl__OnLButtonUp_CMFCToolBar__IEAAXIVCPoint___Z(CMFCToolBar* pThis, unsigned int nFlags, CPoint point);
extern "C" void MS_ABI impl__OnMouseMove_CMFCToolBar__IEAAXIVCPoint___Z(CMFCToolBar* pThis, unsigned int nFlags, CPoint point);
extern "C" unsigned long MS_ABI impl__OnDragOver_CMFCToolBar__MEAAKPEAVCOleDataObject__KVCPoint___Z(
    CMFCToolBar* pThis, COleDataObject* pDataObject, unsigned long dwKeyState, CPoint point);
extern "C" int MS_ABI impl__HitTest_CMFCToolBar__UEAAHVCPoint___Z(CMFCToolBar* pThis, CPoint point);
extern "C" CMFCToolBarButton* MS_ABI impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(const CMFCToolBar* pThis, int iIndex);
extern "C" int MS_ABI impl__GetCount_CMFCToolBar__QEBAHXZ(const CMFCToolBar* pThis);
extern "C" int MS_ABI impl__ButtonToIndex_CMFCToolBar__QEBAHPEBVCMFCToolBarButton___Z(const CMFCToolBar* pThis, const CMFCToolBarButton* pButton);
extern "C" CMFCToolBarButton* MS_ABI impl__InvalidateButton_CMFCToolBar__QEAAPEAVCMFCToolBarButton__H_Z(CMFCToolBar* pThis, int iButton);
extern "C" int MS_ABI impl__SetHot_CMFCToolBar__QEAAHPEAVCMFCToolBarButton___Z(CMFCToolBar* pThis, CMFCToolBarButton* pMenuButton);
extern "C" int MS_ABI impl__OnSetDefaultButtonText_CMFCToolBar__UEAAHPEAVCMFCToolBarButton___Z(CMFCToolBar* pThis, CMFCToolBarButton* pButton);
extern "C" int MS_ABI impl__EnableContextMenuItems_CMFCToolBar__MEAAHPEAVCMFCToolBarButton__PEAVCMenu___Z(
    CMFCToolBar* pThis, CMFCToolBarButton* pButton, CMenu* pPopup);
extern "C" void MS_ABI impl__ShowCommandMessageString_CMFCToolBar__MEAAXI_Z(CMFCToolBar* pThis, unsigned int uiCmdId);
extern "C" __int64 MS_ABI impl__OnToolHitTest_CMFCToolBar__MEBA_JVCPoint__PEAUtagTOOLINFOW___Z(const CMFCToolBar* pThis, CPoint point, TOOLINFOW* pTI);
extern "C" void MS_ABI impl__OnToolbarAppearance_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis);
extern "C" void MS_ABI impl__Deactivate_CMFCToolBar__UEAAXXZ(CMFCToolBar* pThis);
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCToolBar__UEAAXPEAVCFrameWnd__H_Z(CMFCToolBar* pThis, CFrameWnd* pTarget, int bDisableIfNoHandler);
extern "C" void MS_ABI impl__AccNotifyObjectFocusEvent_CMFCToolBar__MEAAXH_Z(CMFCToolBar* pThis, int iButton);
extern "C" int MS_ABI impl__IsCommandRarelyUsed_CMFCToolBar__SAHI_Z(unsigned int uiCmd);
extern "C" void MS_ABI impl___1CMFCToolBarButton__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CMFCToolBarMenuButton__UEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0CMFCToolBarMenuButton__QEAA_IPEAUHMENU____HPEB_WH_Z(
    void* pThis, unsigned int uiID, HMENU hMenu, int iImage, const wchar_t* lpszText, int bUserButton);
extern "C" CMFCToolBarButton* MS_ABI impl__CreateFromOleData_CMFCToolBarButton__SAPEAV1_PEAVCOleDataObject___Z(COleDataObject* pDataObject);
extern "C" int MS_ABI impl__ExportToMenuButton_CMFCToolBarButton__UEBAHAEAVCMFCToolBarMenuButton___Z(
    const CMFCToolBarButton* pThis, CMFCToolBarMenuButton* pMenuButton);
extern "C" int MS_ABI impl__OnToolHitTest_CMFCToolBarButton__UEAAHPEBVCWnd__PEAUtagTOOLINFOW___Z(
    CMFCToolBarButton* pThis, const CWnd* pWnd, TOOLINFOW* pTI);
extern "C" int MS_ABI impl__OnClick_CMFCToolBarMenuButton__UEAAHPEAVCWnd__H_Z(CMFCToolBarMenuButton* pThis, CWnd* pWnd, int bDelay);
extern "C" int MS_ABI impl__OpenPopupMenu_CMFCToolBarMenuButton__UEAAHPEAVCWnd___Z(CMFCToolBarMenuButton* pThis, CWnd* pWnd);
extern "C" void MS_ABI impl__OnCancelMode_CMFCToolBarMenuButton__UEAAXXZ(void* pButton);
extern "C" void MS_ABI impl__GetImageRect_CMFCToolBarMenuButton__QEAAXAEAVCRect___Z(CMFCToolBarMenuButton* pThis, CRect* pRect);
extern "C" int MS_ABI impl__InvokeCommand_CMFCCustomizeButton__UEAAHPEAVCMFCPopupMenuBar__PEBVCMFCToolBarButton___Z(
    void* pThis, void* pMenuBar, const CMFCToolBarButton* pButton);
extern "C" int MS_ABI impl__InvokeTool_CUserToolsManager__QEAAHI_Z(void* pThis, unsigned int uiCmdId);
extern "C" void MS_ABI impl__AddCmd_CMFCCmdUsageCount__QEAAXI_Z(void* pThis, unsigned int uiCmd);
extern "C" void MS_ABI impl__EnableMenuItemImage_CCommandManager__QEAAXIHH_Z(void* pThis, unsigned int uiCmd, int bEnable, int nUserImage);
extern "C" void MS_ABI impl__SetTooltipText_CTooltipManager__SAXPEAUtagTOOLINFOW__PEAVCToolTipCtrl__IV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(
    TOOLINFOW* pTI, CToolTipCtrl* pToolTip, unsigned int nType, const CString* pText, const wchar_t* lpszDescr);
extern "C" void MS_ABI impl__SetDroppedDown_CMFCRibbonBaseElement__IEAAXPEAVCMFCPopupMenu___Z(void* pThis, void* pPopupMenu);
extern "C" void MS_ABI impl__PostMenuCommand_CMFCRibbonBaseElement__QEAAXI_Z(void* pThis, unsigned int uiCmdId);
extern "C" void MS_ABI impl__OnDrawSeparator_CMFCVisualManager__UEAAXPEAVCDC__PEAVCBasePane__VCRect__H_Z(
    CMFCVisualManager* pThis, CDC* pDC, CBasePane* pBar, CRect rect, int bHorz);
extern "C" void MS_ABI impl__OnHighlightRarelyUsedMenuItems_CMFCVisualManager__UEAAXPEAVCDC__VCRect___Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect);
extern "C" long MS_ABI impl__get_accRole_CBasePane__UEAAJUtagVARIANT__PEAU2__Z(CBasePane* pThis, const VARIANT* pvarChild, VARIANT* pvarRole);
extern "C" long MS_ABI impl__get_accState_CBasePane__UEAAJUtagVARIANT__PEAU2__Z(CBasePane* pThis, const VARIANT* pvarChild, VARIANT* pvarState);

// Exports of this class defined later in this file and reached by earlier
// bodies (retail reaches them through vtable slots; see each call site).
extern "C" CMFCToolBar* MS_ABI impl__FindDestintationToolBar_CMFCPopupMenuBar__QEAAPEAVCMFCToolBar__VCPoint___Z(CMFCPopupMenuBar* pThis, long long point);
extern "C" void MS_ABI impl__InvokeMenuCommand_CMFCPopupMenuBar__IEAAXIPEBVCMFCToolBarButton___Z(
    CMFCPopupMenuBar* pThis, unsigned int uiCmdId, const CMFCToolBarButton* pMenuItem);
extern "C" void MS_ABI impl__SetInCommand_CMFCPopupMenuBar__IEAAXH_Z(CMFCPopupMenuBar* pThis, int bInCommand);
extern "C" void MS_ABI impl__RestoreDelayedSubMenu_CMFCPopupMenuBar__UEAAXXZ(CMFCPopupMenuBar* pThis);
extern "C" int MS_ABI impl__OnSendCommand_CMFCPopupMenuBar__MEAAHPEBVCMFCToolBarButton___Z(CMFCPopupMenuBar* pThis, const CMFCToolBarButton* pButton);
extern "C" void MS_ABI impl__ShowCommandMessageString_CMFCPopupMenuBar__MEAAXI_Z(CMFCPopupMenuBar* pThis, unsigned int uiCmdId);
extern "C" int MS_ABI impl__OnKey_CMFCPopupMenuBar__MEAAHI_Z(CMFCPopupMenuBar* pThis, unsigned int nChar);
// featurepack/customize/CKeyboardManager.cpp (real bodies: printable = 0x20..0x7e, towupper).
extern "C" int MS_ABI impl__IsKeyPrintable_CKeyboardManager__SAHI_Z(unsigned int nChar);
extern "C" unsigned int MS_ABI impl__TranslateCharToUpper_CKeyboardManager__SAII_Z(unsigned int nChar);

namespace {
// The parent-popup lookup every retail body inlines:
//     CWnd* p = CWnd::FromHandle(::GetParent(m_hWnd));
//     if (p == NULL || !p->IsKindOf(RUNTIME_CLASS(CMFCPopupMenu))) p = NULL;
inline CMFCPopupMenu* ParentPopupMenu(const void* pThis) {
    CWnd* p = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(HWndOf(pThis)));
    if (p == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(p, impl__GetThisClass_CMFCPopupMenu__SAPEAUCRuntimeClass__XZ())) {
        return nullptr;
    }
    return static_cast<CMFCPopupMenu*>(p);
}

// GetOwner() as retail inlines it: `m_hWndOwner ? m_hWndOwner : ::GetParent(m_hWnd)`
// (CWnd + 0xa0).  DEVIATION, shared with CMFCToolBar.cpp: OpenMFC's CWnd does
// not name m_hWndOwner (that byte range is anonymous padding that nothing ever
// writes), so the ::GetParent branch is taken unconditionally.
inline HWND OwnerHwnd(const void* pThis) {
    return ::GetParent(HWndOf(pThis));
}

inline int IsMenuButton(const CMFCToolBarButton* p) {
    return p != nullptr &&
           impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(p, impl__GetThisClass_CMFCToolBarMenuButton__SAPEAUCRuntimeClass__XZ());
}

// CMFCPopupMenu virtuals the bodies below reach.  OpenMFC's C++ classes do not
// carry the retail vtable, so each one is folded to the retail BASE body and
// noted as a deviation at the call site:
//   vslot 0x3a0 GetMenuBar()   : `lea 0x230(%rcx),%rax` (RVA 0x34cf0) -- the embedded bar
inline CMFCPopupMenuBar* PopupMenuBarOf(void* pPopupMenu) {
    return reinterpret_cast<CMFCPopupMenuBar*>(static_cast<char*>(pPopupMenu) + 0x230);
}
} // namespace

// Retail (RVA 0xbcfe0, mfc140u), fully transcribed:
//     CMFCToolBarButton* pButton = CMFCToolBarButton::CreateFromOleData(pDataObject);  // 0x15c790
//     if (pButton == NULL) AfxThrowInvalidArgException();                             // 0x227720
//     if (pButton->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton))) return pButton;     // 0x3b1748
//     CMFCToolBarMenuButton* pMenuButton = new CMFCToolBarMenuButton(               // 0x128 bytes, ctor 0x172870
//         pButton->m_nID, NULL,
//         pButton->m_bLocked ? -1 : pButton->GetImage(),   // +0x50; GetImage = m_bUserButton ? m_iUserImage : m_iImage
//         pButton->m_strText, pButton->m_bUserButton);      // +0x38, +0x08
//     if (pMenuButton == NULL) AfxThrowInvalidArgException();  (operator new returned NULL)
//     pMenuButton->m_bText  = TRUE;                            // +0x0c
//     pMenuButton->m_bImage = !pButton->m_bLocked;             // +0x10
//     BOOL bOk = pButton->ExportToMenuButton(*pMenuButton);    // button vslot 0x68 (slot 13 of 0x3184e8)
//     delete pButton;                                          // vslot 0x08, deleting dtor
//     if (bOk && pMenuButton->m_strText.GetLength() != 0)      // CStringData::nDataLength at text-0x10
//         return pMenuButton;
//     delete pMenuButton;                                      // vslot 0x08
//     return NULL;
// ExportToMenuButton (button vslot 0x68) is reached through the exported base
// thunk, non-virtually (DEVIATION for a client-overridden button class).  The
// two deleting-destructor calls (vslot 0x08, flag 1) become the exported ??1
// thunks -- which invoke the C++ virtual destructor, so an OpenMFC-built
// derived button is destroyed correctly (a client-built button's own slot-1
// body is not consulted) -- followed by the exported ??3@YAXPEAX@Z.  `new` is
// the exported ??2@YAPEAX_K@Z (exactly what retail calls) + the exported ctor
// thunk, because the C++ constructor is not linkable from here.  CreateFromOleData is
// still a NULL-returning stub in featurepack/toolbar/CMFCToolBarButton.cpp, so
// today every call ends in the same AfxThrowInvalidArgException retail raises
// for a data object without a toolbar-button format.
// Symbol: ?CreateDroppedButton@CMFCPopupMenuBar@@MEAAPEAVCMFCToolBarButton@@PEAVCOleDataObject@@@Z
extern "C" CMFCToolBarButton* MS_ABI impl__CreateDroppedButton_CMFCPopupMenuBar__MEAAPEAVCMFCToolBarButton__PEAVCOleDataObject___Z(
    CMFCPopupMenuBar* pThis, COleDataObject* pDataObject)
{
    (void)pThis;
    CMFCToolBarButton* pButton = impl__CreateFromOleData_CMFCToolBarButton__SAPEAV1_PEAVCOleDataObject___Z(pDataObject);
    if (pButton == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return nullptr;
    }
    if (IsMenuButton(pButton)) {
        return pButton;
    }

    void* pStorage = impl___2_YAPEAX_K_Z(sizeof(CMFCToolBarMenuButton));   // ??2@YAPEAX_K@Z, 0x128 bytes
    CMFCToolBarMenuButton* pMenuButton = nullptr;
    if (pStorage != nullptr) {
        const int iImage = pButton->m_bLocked ? -1 : pButton->GetImage();
        pMenuButton = static_cast<CMFCToolBarMenuButton*>(
            impl___0CMFCToolBarMenuButton__QEAA_IPEAUHMENU____HPEB_WH_Z(
                pStorage, pButton->m_nID, nullptr, iImage,
                static_cast<const wchar_t*>(pButton->m_strText), pButton->m_bUserButton));
    }
    if (pMenuButton == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return nullptr;
    }

    pMenuButton->m_bText  = TRUE;
    pMenuButton->m_bImage = (pButton->m_bLocked == 0);

    const int bOk = impl__ExportToMenuButton_CMFCToolBarButton__UEBAHAEAVCMFCToolBarMenuButton___Z(pButton, pMenuButton);

    impl___1CMFCToolBarButton__UEAA_XZ(pButton);          // retail: pButton->`vector deleting dtor`(1)
    impl___3_YAXPEAX_Z(pButton);

    if (bOk && pMenuButton->m_strText.GetLength() != 0) {
        return pMenuButton;
    }

    impl___1CMFCToolBarMenuButton__UEAA_XZ(pMenuButton);  // retail: pMenuButton->`vector deleting dtor`(1)
    impl___3_YAXPEAX_Z(pMenuButton);
    return nullptr;
}

// Retail (RVA 0xbcda0, mfc140u), fully transcribed:
//     if (m_bPaletteMode) return;                                   // +0x1390 (does NOT fall back to the base)
//     CPen* pOldPen = pDC->SelectObject(&m_penDrag);                // +0x12c0, 0x2a2730 (the CPen and
//                                                                   //   CBrush overloads fold to one body)
//     for (int i = 0; i < 2; i++) {
//         int y = m_rectDrag.top + (m_rectDrag.bottom - m_rectDrag.top) / 2 - 1 + i;   // +0x12a0
//         pDC->MoveTo(m_rectDrag.left, y);            pDC->LineTo(m_rectDrag.right, y);
//         pDC->MoveTo(m_rectDrag.left + i,  m_rectDrag.top + i);
//         pDC->LineTo(m_rectDrag.left + i,  m_rectDrag.bottom - i);
//         pDC->MoveTo(m_rectDrag.right - i - 1, m_rectDrag.top + i);
//         pDC->LineTo(m_rectDrag.right - i - 1, m_rectDrag.bottom - i);
//     }
//     pDC->SelectObject(pOldPen);
// (MoveTo is 0x2a30c0, LineTo 0x2a3120.  The division is the signed `cltd;
// sub %edx,%eax; sar $1` idiom, i.e. C's `/ 2`.)
// Symbol: ?DrawDragCursor@CMFCPopupMenuBar@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DrawDragCursor_CMFCPopupMenuBar__MEAAXPEAVCDC___Z(CMFCPopupMenuBar* pThis, CDC* pDC)
{
    if (!pThis || !pDC) return;
    if (At<int>(pThis, kOffPaletteMode) != 0) return;

    CMFCToolBar* pBar = AsToolBar(pThis);
    CPen* pOldPen = impl__SelectObject_CDC__QEAAPEAVCPen__PEAV2__Z(pDC, &pBar->m_penDrag);
    for (int i = 0; i < 2; ++i) {
        const CRect& r = pBar->m_rectDrag;
        const int y = r.top + (r.bottom - r.top) / 2 - 1 + i;
        impl__MoveTo_CDC__QEAA_AVCPoint__HH_Z(pDC, r.left, y);
        impl__LineTo_CDC__QEAAHHH_Z(pDC, r.right, y);
        impl__MoveTo_CDC__QEAA_AVCPoint__HH_Z(pDC, r.left + i, r.top + i);
        impl__LineTo_CDC__QEAAHHH_Z(pDC, r.left + i, r.bottom - i);
        impl__MoveTo_CDC__QEAA_AVCPoint__HH_Z(pDC, r.right - i - 1, r.top + i);
        impl__LineTo_CDC__QEAAHHH_Z(pDC, r.right - i - 1, r.bottom - i);
    }
    impl__SelectObject_CDC__QEAAPEAVCPen__PEAV2__Z(pDC, pOldPen);
}

// Retail (RVA 0xbc9f0, mfc140u), fully transcribed:
//     CMFCVisualManager::GetInstance()->OnDrawSeparator(pDC, this, rect, FALSE);   // 0x9774, then vftable +0xa0
// bHorz is NOT forwarded: the [rsp+0x20] slot is stored as 0 unconditionally
// (0xbca13), so a popup menu bar always draws its separators as horizontal
// lines of a vertical bar.  Slot 0xa0 of the mfc140u CMFCVisualManager vftable
// (0x18031c128) is ?OnDrawSeparator@CMFCVisualManager@@UEAAXPEAVCDC@@PEAVCBasePane@@VCRect@@H@Z
// (0x185140).  0x9774 is the inlined CMFCVisualManager::GetInstance(); the same
// deviation as CMFCToolBar::DrawSeparator applies -- the current manager is read
// from ?m_pVisManager@CMFCVisualManager@@1PEAV1@EA and the lazy construction
// of a default manager is not reproduced, so with no manager created yet
// nothing is drawn.  The OnDrawSeparator thunk (visualmanager/Thunks.cpp)
// dispatches virtually on the C++ manager, so a derived manager's override IS
// reached.
// Symbol: ?DrawSeparator@CMFCPopupMenuBar@@MEAAXPEAVCDC@@AEBVCRect@@H@Z
extern "C" void MS_ABI impl__DrawSeparator_CMFCPopupMenuBar__MEAAXPEAVCDC__AEBVCRect__H_Z(
    CMFCPopupMenuBar* pThis, CDC* pDC, const CRect* pRect, int bHorz)
{
    (void)bHorz;    // retail ignores it (see above)
    if (!pThis || !pRect) return;
    CMFCVisualManager* pVM = static_cast<CMFCVisualManager*>(impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
    if (!pVM) {
        // TODO(clean-room): retail's GetInstance() would create the default manager here.
        return;
    }
    CRect rect(*pRect);
    impl__OnDrawSeparator_CMFCVisualManager__UEAAXPEAVCDC__PEAVCBasePane__VCRect__H_Z(pVM, pDC, AsToolBar(pThis), rect, FALSE);
}

// Retail (RVA 0xbea20, mfc140u), fully transcribed:
//     if (!CMFCToolBar::m_bCustomizeMode) return FALSE;                        // 0x3be35c
//     pButton->m_bText = TRUE;                                                 // +0x0c
//     CMFCToolBar::EnableContextMenuItems(pButton, pPopup);                    // 0x157550, result dropped
//     ::EnableMenuItem(pPopup->m_hMenu, ID_AFXBARRES_TOOLBAR_IMAGE (0x4212), MF_GRAYED);
//     ::EnableMenuItem(pPopup->m_hMenu, ID_AFXBARRES_TOOLBAR_TEXT  (0x4213), MF_ENABLED);
//     ::EnableMenuItem(pPopup->m_hMenu, ID_AFXBARRES_TOOLBAR_IMAGE_AND_TEXT (0x4214),
//                      (pButton->GetImage() < 0 && CMFCToolBar::m_pUserImages == NULL) ? MF_GRAYED : MF_ENABLED);  // 0x3be370
//     if (afxCommandManager->m_lstCommandsWithoutImages.Find(pButton->m_nID) != NULL) {   // 0x3c1520 = GetCmdMgr()+0xb0, 0x11f1c = CList<UINT,UINT>::Find
//         ::CheckMenuItem(pPopup->m_hMenu, 0x4213, MF_CHECKED);
//         ::CheckMenuItem(pPopup->m_hMenu, 0x4214, MF_UNCHECKED);
//     }
//     return TRUE;
// (Import slots 0x2c6cd8 / 0x2c6da0 resolve to USER32!EnableMenuItem /
// CheckMenuItem; pPopup->m_hMenu is CMenu + 8.  0x11f1c walks the list from
// m_pNodeHead (+8) comparing each node's +0x10 with the id, which is the
// inlined IsMenuItemWithoutImage().)  The list walk is reproduced verbatim
// against the retail-laid-out singleton that GetCmdMgr() returns.
// Symbol: ?EnableContextMenuItems@CMFCPopupMenuBar@@MEAAHPEAVCMFCToolBarButton@@PEAVCMenu@@@Z
extern "C" int MS_ABI impl__EnableContextMenuItems_CMFCPopupMenuBar__MEAAHPEAVCMFCToolBarButton__PEAVCMenu___Z(
    CMFCPopupMenuBar* pThis, CMFCToolBarButton* pButton, CMenu* pPopup)
{
    if (!pThis || !pButton || !pPopup) return FALSE;
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA == 0) return FALSE;

    pButton->m_bText = TRUE;
    impl__EnableContextMenuItems_CMFCToolBar__MEAAHPEAVCMFCToolBarButton__PEAVCMenu___Z(AsToolBar(pThis), pButton, pPopup);

    const HMENU hMenu = pPopup->m_hMenu;
    ::EnableMenuItem(hMenu, kIdAfxBarResToolbarImage, MF_GRAYED);
    ::EnableMenuItem(hMenu, kIdAfxBarResToolbarText, MF_ENABLED);
    ::EnableMenuItem(hMenu, kIdAfxBarResToolbarImageAndText,
                     (pButton->GetImage() < 0 && impl__m_pUserImages_CMFCToolBar__1PEAVCMFCToolBarImages__EA == nullptr)
                         ? MF_GRAYED : MF_ENABLED);

    bool bWithoutImage = false;
    if (void* pCmdMgr = impl__GetCmdMgr__YAPEAVCCommandManager__XZ()) {
        const char* pList = static_cast<const char*>(pCmdMgr) + kOffCmdMgrCommandsWithoutImages;
        for (const char* pNode = At<const char*>(pList, kOffListNodeHead); pNode != nullptr;
             pNode = At<const char*>(pNode, kOffNodeNext)) {
            if (At<unsigned int>(pNode, kOffNodeData) == pButton->m_nID) { bWithoutImage = true; break; }
        }
    }
    if (bWithoutImage) {
        ::CheckMenuItem(hMenu, kIdAfxBarResToolbarText, MF_CHECKED);
        ::CheckMenuItem(hMenu, kIdAfxBarResToolbarImageAndText, MF_UNCHECKED);
    }
    return TRUE;
}

// Retail (RVA 0xbec20, mfc140u), fully transcribed.  `point` arrives in screen
// coordinates:
//     ::ScreenToClient(m_hWnd, &point);
//     CMFCPopupMenu* pParentMenu = DYNAMIC_DOWNCAST(CMFCPopupMenu, CWnd::FromHandle(::GetParent(m_hWnd)));
//     if (pParentMenu == NULL) return NULL;
//     for (CMFCPopupMenu* p = pParentMenu->GetParentPopupMenu(); p != NULL;        // 0xb7550
//          pParentMenu = p, p = p->GetParentPopupMenu()) {
//         CMFCPopupMenuBar* pBar = p->GetMenuBar();                                 // vslot 0x3a0
//         CRect rc; ::GetClientRect(pBar->m_hWnd, &rc);
//         ::MapWindowPoints(pBar->m_hWnd, m_hWnd, (POINT*)&rc, 2);
//         if (rc.PtInRect(point)) return pBar;                                      // NB: returns the BAR (vslot result), typed CMFCToolBar*
//     }
//     CMFCToolBar* pToolBar = pParentMenu->GetParentToolBar();   // 0xb75f0, on the outermost popup
//     if (pToolBar == NULL) return NULL;
//     CRect rc; ::GetClientRect(pToolBar->m_hWnd, &rc);
//     ::MapWindowPoints(pToolBar->m_hWnd, m_hWnd, (POINT*)&rc, 2);
//     return rc.PtInRect(point) ? pToolBar : NULL;
// vslot 0x3a0 (GetMenuBar) is folded to its retail base body, `this + 0x230`
// (DEVIATION for a client override).
// Symbol: ?FindDestintationToolBar@CMFCPopupMenuBar@@QEAAPEAVCMFCToolBar@@VCPoint@@@Z
extern "C" CMFCToolBar* MS_ABI impl__FindDestintationToolBar_CMFCPopupMenuBar__QEAAPEAVCMFCToolBar__VCPoint___Z(
    CMFCPopupMenuBar* pThis, long long pointPacked)
{
    if (!pThis) return nullptr;
    const HWND hWnd = HWndOf(pThis);
    POINT point = UnpackPoint(pointPacked);
    ::ScreenToClient(hWnd, &point);

    CMFCPopupMenu* pParentMenu = ParentPopupMenu(pThis);
    if (pParentMenu == nullptr) return nullptr;

    for (void* p = impl__GetParentPopupMenu_CMFCPopupMenu__QEBAPEAV1_XZ(pParentMenu); p != nullptr;
         pParentMenu = static_cast<CMFCPopupMenu*>(p), p = impl__GetParentPopupMenu_CMFCPopupMenu__QEBAPEAV1_XZ(p)) {
        CMFCPopupMenuBar* pBar = PopupMenuBarOf(p);                                  // vslot 0x3a0
        RECT rc = {};
        ::GetClientRect(HWndOf(pBar), &rc);
        ::MapWindowPoints(HWndOf(pBar), hWnd, reinterpret_cast<POINT*>(&rc), 2);
        if (::PtInRect(&rc, point)) return pBar;
    }

    CMFCToolBar* pToolBar = impl__GetParentToolBar_CMFCPopupMenu__QEBAPEAVCMFCToolBar__XZ(pParentMenu);
    if (pToolBar == nullptr) return nullptr;
    RECT rc = {};
    ::GetClientRect(HWndOf(pToolBar), &rc);
    ::MapWindowPoints(HWndOf(pToolBar), hWnd, reinterpret_cast<POINT*>(&rc), 2);
    return ::PtInRect(&rc, point) ? pToolBar : nullptr;
}

// Retail (RVA 0xbced0, mfc140u), fully transcribed:
//     if (m_bPaletteMode) return -1;                                   // +0x1390
//     ::GetClientRect(m_hWnd, &rectDrag);
//     if (m_Buttons.GetCount() == 0) { rectDrag.bottom = rectDrag.top + 6; return 0; }   // +0x11a0
//     int y = max(p.y, 0);
//     int iIndex = 0, iDropIndex = -1;
//     for (POSITION pos = m_Buttons.GetHeadPosition(); pos != NULL; iIndex++) {
//         CMFCToolBarButton* pButton = m_Buttons.GetNext(pos);  if (!pButton) AfxThrowInvalidArgException();
//         const CRect& rc = pButton->m_rect;                           // +0x68
//         if (y < rc.top)         { rectDrag.top = rc.top;  iDropIndex = iIndex; break; }
//         if (y <= rc.bottom) {
//             rectDrag = rc;
//             if (y - rc.top > rc.bottom - y) { iDropIndex = iIndex + 1; rectDrag.top = rc.bottom; }
//             else                            { iDropIndex = iIndex;     rectDrag.top = rc.top;    }
//             break;
//         }
//     }
//     if (iDropIndex == -1) { iDropIndex = iIndex; rectDrag.top = rectDrag.bottom - 6; }   // past the last button
//     rectDrag.bottom = rectDrag.top + 6;
//     ::OffsetRect(&rectDrag, 0, -3);
//     return iDropIndex;
// DEVIATION (shared with the whole toolbar family): the buttons are read through
// GetCount()/GetButton(), which are backed by the mfccore.cpp side table where
// OpenMFC keeps the live buttons, instead of the real m_Buttons CObList that
// OpenMFC leaves empty.
// Symbol: ?FindDropIndex@CMFCPopupMenuBar@@MEBAHVCPoint@@AEAVCRect@@@Z
extern "C" int MS_ABI impl__FindDropIndex_CMFCPopupMenuBar__MEBAHVCPoint__AEAVCRect___Z(
    const CMFCPopupMenuBar* pThis, long long pointPacked, CRect* pRectDrag)
{
    if (!pThis || !pRectDrag) return -1;
    if (At<int>(pThis, kOffPaletteMode) != 0) return -1;

    const CMFCToolBar* pBar = static_cast<const CMFCToolBar*>(pThis);
    ::GetClientRect(HWndOf(pThis), reinterpret_cast<RECT*>(pRectDrag));

    const int nCount = impl__GetCount_CMFCToolBar__QEBAHXZ(pBar);
    if (nCount == 0) {
        pRectDrag->bottom = pRectDrag->top + 6;
        return 0;
    }

    const POINT p = UnpackPoint(pointPacked);
    const int y = p.y < 0 ? 0 : p.y;
    int iIndex = 0;
    int iDropIndex = -1;
    for (; iIndex < nCount; ++iIndex) {
        const CMFCToolBarButton* pButton = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, iIndex);
        if (pButton == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return -1; }
        const CRect& rc = pButton->m_rect;
        if (y < rc.top) {
            pRectDrag->top = rc.top;
            iDropIndex = iIndex;
            break;
        }
        if (y <= rc.bottom) {
            *pRectDrag = rc;
            if (y - rc.top > rc.bottom - y) { iDropIndex = iIndex + 1; pRectDrag->top = rc.bottom; }
            else                            { iDropIndex = iIndex;     pRectDrag->top = rc.top;    }
            break;
        }
    }
    if (iDropIndex == -1) {
        iDropIndex = iIndex;
        pRectDrag->top = pRectDrag->bottom - 6;
    }
    pRectDrag->bottom = pRectDrag->top + 6;
    ::OffsetRect(reinterpret_cast<RECT*>(pRectDrag), 0, -3);
    return iDropIndex;
}

// Retail (RVA 0xbfd70, mfc140u), fully transcribed:
//     if (pvarRole == NULL) return E_INVALIDARG;                       // 0x80070057
//     if (varChild.vt == VT_I4 && varChild.lVal == CHILDID_SELF) {
//         pvarRole->vt = VT_I4; pvarRole->lVal = ROLE_SYSTEM_MENUPOPUP;  // 0x16
//         return S_OK;
//     }
//     return CBasePane::get_accRole(varChild, pvarRole);                // 0xd1b0
// varChild is a 24-byte VARIANT passed by hidden reference (rdx); the base call
// copies it to the outgoing frame.
// Symbol: ?get_accRole@CMFCPopupMenuBar@@UEAAJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__get_accRole_CMFCPopupMenuBar__UEAAJUtagVARIANT__PEAU2__Z(
    CMFCPopupMenuBar* pThis, const VARIANT* pvarChild, VARIANT* pvarRole)
{
    if (pvarRole == nullptr) return E_INVALIDARG;
    if (pvarChild != nullptr && pvarChild->vt == VT_I4 && pvarChild->lVal == 0 /*CHILDID_SELF*/) {
        pvarRole->vt = VT_I4;
        pvarRole->lVal = 0x16;   // ROLE_SYSTEM_MENUPOPUP
        return S_OK;
    }
    if (!pThis) return E_INVALIDARG;
    return impl__get_accRole_CBasePane__UEAAJUtagVARIANT__PEAU2__Z(static_cast<CBasePane*>(pThis), pvarChild, pvarRole);
}

// Retail (RVA 0xbfdd0, mfc140u), fully transcribed -- the twin of get_accRole:
//     if (pvarState == NULL) return E_INVALIDARG;
//     if (varChild.vt == VT_I4 && varChild.lVal == CHILDID_SELF) {
//         pvarState->vt = VT_I4; pvarState->lVal = STATE_SYSTEM_FOCUSABLE;   // 0x100
//         return S_OK;
//     }
//     return CBasePane::get_accState(varChild, pvarState);                   // 0xd230
// Symbol: ?get_accState@CMFCPopupMenuBar@@UEAAJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__get_accState_CMFCPopupMenuBar__UEAAJUtagVARIANT__PEAU2__Z(
    CMFCPopupMenuBar* pThis, const VARIANT* pvarChild, VARIANT* pvarState)
{
    if (pvarState == nullptr) return E_INVALIDARG;
    if (pvarChild != nullptr && pvarChild->vt == VT_I4 && pvarChild->lVal == 0 /*CHILDID_SELF*/) {
        pvarState->vt = VT_I4;
        pvarState->lVal = 0x100;   // STATE_SYSTEM_FOCUSABLE
        return S_OK;
    }
    if (!pThis) return E_INVALIDARG;
    return impl__get_accState_CBasePane__UEAAJUtagVARIANT__PEAU2__Z(static_cast<CBasePane*>(pThis), pvarChild, pvarState);
}

// Retail (RVA 0xbf490, mfc140u), fully transcribed:
//     if (m_bTrackMode) return NULL;                                             // +0x1388
//     CMFCPopupMenu* pParentMenu = DYNAMIC_DOWNCAST(CMFCPopupMenu, CWnd::FromHandle(::GetParent(m_hWnd)));
//     if (pParentMenu != NULL && pParentMenu->m_pMessageWnd != NULL)             // +0x1f8
//         return pParentMenu;
//     return CMFCToolBar::GetCommandTarget();                                    // 0x5cbd0, not exported
// 0x5cbd0 is the (non-exported) base body, inlined here as retail has it:
//     CWnd* pOwner = CWnd::FromHandle(m_hWndOwner ? m_hWndOwner : ::GetParent(m_hWnd));   // +0xa0
//     if (pOwner != NULL && m_bRouteCommandsViaFrame && pOwner->{CWnd vslot 0x2b0}())   // +0x10f4; slot 86 = IsFrameWnd
//         return pOwner;
//     return AFXGetParentFrame(this);                                            // 0x6bd00
// (Note retail returns pOwner without the frame test when m_bRouteCommandsViaFrame
// is FALSE and pOwner is non-NULL: the `cmpl $0,0x10f4; je 0x5cc2e` jumps
// straight to the return with rdi = pOwner.)  The owner lookup takes the
// ::GetParent branch unconditionally (see OwnerHwnd), and IsFrameWnd is called
// through the CWnd thunk rather than the owner's vtable slot 86 (DEVIATION:
// core/window/CWnd.cpp answers it with `dynamic_cast<const CFrameWnd*>`, so
// only an owner whose C++ type OpenMFC built is recognised as a frame; the
// slot-86 override of a client-built frame class is not consulted).
// Symbol: ?GetCommandTarget@CMFCPopupMenuBar@@MEBAPEAVCWnd@@XZ
extern "C" CWnd* MS_ABI impl__GetCommandTarget_CMFCPopupMenuBar__MEBAPEAVCWnd__XZ(const CMFCPopupMenuBar* pThis)
{
    if (!pThis) return nullptr;
    if (At<int>(pThis, kOffTrackMode) != 0) return nullptr;

    CMFCPopupMenu* pParentMenu = ParentPopupMenu(pThis);
    if (pParentMenu != nullptr && At<CWnd*>(pParentMenu, kOffPopupMessageWnd) != nullptr) {
        return pParentMenu;
    }

    // Inlined CMFCToolBar::GetCommandTarget (RVA 0x5cbd0, mfc140u).
    const CMFCToolBar* pBar = static_cast<const CMFCToolBar*>(pThis);
    CWnd* pOwner = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(OwnerHwnd(pThis));
    if (pOwner != nullptr) {
        if (pBar->m_bRouteCommandsViaFrame == 0) return pOwner;
        if (impl__IsFrameWnd_CWnd__UEBAHXZ(pOwner)) return pOwner;
    }
    return impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(static_cast<const CWnd*>(pThis));
}

// Retail (RVA 0xbc2d0, mfc140u), transcribed (deviations listed after the body):
//     CMFCPopupMenu* pParentMenu = DYNAMIC_DOWNCAST(CMFCPopupMenu, CWnd::FromHandle(::GetParent(m_hWnd)));
//     if (pParentMenu != NULL && pParentMenu->m_pMessageWnd != NULL)                // +0x1f8
//         ::SendMessage(pParentMenu->m_pMessageWnd->m_hWnd, WM_SETMESSAGESTRING, AFX_IDS_IDLEMESSAGE, 0);
//     else {
//         ::SendMessage(GetOwner()->m_hWnd, WM_SETMESSAGESTRING, AFX_IDS_IDLEMESSAGE, 0);   // owner = +0xa0 ?: ::GetParent
//         if (pParentMenu == NULL) goto skipDeactivate;
//     }
//     {   // deactivate the toolbar the menu chain hangs off
//         CMFCToolBar* pToolBar = NULL;
//         for (CMFCPopupMenu* p = pParentMenu; p != NULL; p = p->GetParentPopupMenu()) {   // 0xb7550
//             if (p->m_pParentBtn == NULL) break;                                       // +0x228
//             pToolBar = p->m_pParentBtn->m_pWndParent;                                 // +0x80
//             if (pToolBar != NULL && !pToolBar->IsKindOf(RUNTIME_CLASS(CMFCToolBar))) pToolBar = NULL;   // 0x3b15f8
//         }
//         if (pToolBar != NULL) pToolBar->Deactivate();                                 // vslot 0x6d0 (slot 218 of 0x2f8fc8 = ?Deactivate@CMFCToolBar@@)
//     }
// skipDeactivate:
//     if (uiCmdId != 0) {
//         SetInCommand(TRUE);                                                           // 0xbf5f0
//         AFXPlaySystemSound(AFX_SOUND_MENU_COMMAND);                                   // 0x132460(1)
//         if (m_bDropDownListMode) {                                                    // +0x1398
//             if (pParentMenu != NULL) pParentMenu->OnChooseItem(uiCmdId);              // vslot 0x410 (slot 130 of 0x2f85b0 = bare `ret`)
//         } else if (m_bTrackMode) {                                                    // +0x1388
//             if (afxContextMenuManager != NULL) afxContextMenuManager->m_nLastCommandID = uiCmdId;   // 0x3be1b0, +0xb0
//         } else {
//             pParentMenu = DYNAMIC_DOWNCAST(CMFCPopupMenu, CWnd::FromHandle(::GetParent(m_hWnd)));   // re-read
//             if (pParentMenu != NULL && pParentMenu->m_pParentBtn != NULL &&
//                 pParentMenu->m_pParentBtn->IsKindOf(RUNTIME_CLASS(CMFCCustomizeButton)) &&    // 0x3b1148
//                 ((CMFCCustomizeButton*)pParentMenu->m_pParentBtn)->InvokeCommand(this, pMenuItem))   // vslot 0x208
//                 goto done;
//             CMFCToolBar::m_UsageCount.AddCmd(uiCmdId);                                // 0x3b1fd0, 0x232e0
//             if (pParentMenu != NULL) {
//                 if (pParentMenu->PostCommand(uiCmdId)) goto done;                     // 0xb97b0
//                 if (afxUserToolsManager != NULL && afxUserToolsManager->InvokeTool(uiCmdId)) goto done;   // 0x3be3b0, 0x183ee0
//                 UINT msg = (uiCmdId - 0xF000 < 0x1F0) ? WM_SYSCOMMAND : WM_COMMAND;   // SC_* range 0xF000..0xF1EF
//                 ::PostMessage(GetOwner()->m_hWnd, msg, uiCmdId, 0);
//                 if (pParentMenu->m_pParentRibbonElement != NULL) {                    // +0x19b8
//                     CMFCRibbonBaseElement* pElem = pParentMenu->m_pParentRibbonElement;
//                     pElem->SetDroppedDown(NULL);  pParentMenu->m_pParentRibbonElement = NULL;   // 0x10180
//                     pElem->PostMenuCommand(uiCmdId);                                  // 0x10c90
//                 }
//             }
//         }
//     }
// done:
//     m_nLastCommandIndex = (pMenuItem == NULL) ? -1 : ButtonToIndex(pMenuItem);       // 0x3b1b3c, 0x14f000
//     if (m_bPaletteMode) {                                                             // +0x1390
//         pParentMenu = DYNAMIC_DOWNCAST(CMFCPopupMenu, CWnd::FromHandle(::GetParent(m_hWnd)));
//         if (pParentMenu != NULL && pParentMenu->m_pParentBtn != NULL &&
//             pParentMenu->m_pParentBtn->m_pWndParent != NULL) {                        // +0x80
//             CMFCToolBarMenuButton* pBtn = pParentMenu->m_pParentBtn;
//             pBtn->m_nID = uiCmdId;                                                    // +0x24
//             int iImage = -1;
//             if (!afxCommandManager->m_CommandIndex.Lookup(uiCmdId, iImage)) iImage = -1;   // 0x3c1478 = GetCmdMgr()+8, 0x2f610
//             pBtn->SetImage(iImage);                                                   // vslot 0x190 (slot 50 = ?SetImage@CMFCToolBarButton@@)
//             CRect rectImage; pBtn->GetImageRect(rectImage);                           // 0x175ea0
//             ::InvalidateRect(pBtn->m_pWndParent->m_hWnd, &rectImage, TRUE);
//             ::UpdateWindow(pBtn->m_pWndParent->m_hWnd);
//         }
//     }
//     CFrameWnd* pFrame = AFXGetParentFrame(this);                                      // 0x6bd00
//     SetInCommand(FALSE);
//     pFrame->DestroyWindow();                                                          // vslot 0xd0 (slot 26 = ?DestroyWindow@CWnd@@), pFrame unchecked in retail
// DEVIATIONS:
//   * afxContextMenuManager (0x3be1b0) is not exported and OpenMFC keeps no such
//     global, so the track-mode store of m_nLastCommandID is dropped (OpenMFC's
//     CContextMenuManager::TrackPopupMenu never sets m_bTrackMode either).
//   * The palette-mode image lookup (afxCommandManager->m_CommandIndex) and the
//     SetImage virtual are not reproduced: OpenMFC's CCommandManager singleton
//     keeps its CMaps empty and SetImage is not dispatchable; the m_nID store,
//     GetImageRect and the invalidation are kept.
//   * Virtuals (Deactivate, OnChooseItem, InvokeCommand, DestroyWindow) are
//     reached through the exported base thunks, non-virtually.  OnChooseItem's
//     base body is a bare `ret`, so it is omitted rather than "called".
//   * The owner window is ::GetParent(m_hWnd) (see OwnerHwnd); null handles are
//     guarded where retail dereferences unchecked.
// Symbol: ?InvokeMenuCommand@CMFCPopupMenuBar@@IEAAXIPEBVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__InvokeMenuCommand_CMFCPopupMenuBar__IEAAXIPEBVCMFCToolBarButton___Z(
    CMFCPopupMenuBar* pThis, unsigned int uiCmdId, const CMFCToolBarButton* pMenuItem)
{
    if (!pThis) return;
    const HWND hWnd = HWndOf(pThis);

    CMFCPopupMenu* pParentMenu = ParentPopupMenu(pThis);
    if (pParentMenu != nullptr && At<CWnd*>(pParentMenu, kOffPopupMessageWnd) != nullptr) {
        ::SendMessage(HWndOf(At<CWnd*>(pParentMenu, kOffPopupMessageWnd)), kWmSetMessageString, kAfxIdsIdleMessage, 0);
    } else {
        CWnd* pOwner = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(OwnerHwnd(pThis));
        if (pOwner != nullptr) {
            ::SendMessage(HWndOf(pOwner), kWmSetMessageString, kAfxIdsIdleMessage, 0);
        }
    }

    if (pParentMenu != nullptr) {
        CMFCToolBar* pToolBar = nullptr;
        for (void* p = pParentMenu; p != nullptr; p = impl__GetParentPopupMenu_CMFCPopupMenu__QEBAPEAV1_XZ(p)) {
            CMFCToolBarMenuButton* pParentBtn = At<CMFCToolBarMenuButton*>(p, kOffPopupParentBtn);
            if (pParentBtn == nullptr) break;
            pToolBar = static_cast<CMFCToolBar*>(pParentBtn->m_pWndParent);
            if (pToolBar != nullptr &&
                !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pToolBar, impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ())) {
                pToolBar = nullptr;
            }
        }
        if (pToolBar != nullptr) {
            impl__Deactivate_CMFCToolBar__UEAAXXZ(pToolBar);                           // vslot 0x6d0
        }
    }

    if (uiCmdId != 0) {
        impl__SetInCommand_CMFCPopupMenuBar__IEAAXH_Z(pThis, TRUE);
        impl__AFXPlaySystemSound__YAXH_Z(1);   // AFX_SOUND_MENU_COMMAND

        if (At<int>(pThis, kOffDropDownListMode) != 0) {
            // pParentMenu->OnChooseItem(uiCmdId): vslot 0x410, base body is a bare `ret`.
        } else if (At<int>(pThis, kOffTrackMode) != 0) {
            // TODO(clean-room): afxContextMenuManager->m_nLastCommandID = uiCmdId (see DEVIATIONS).
        } else {
            pParentMenu = ParentPopupMenu(pThis);
            bool bHandled = false;
            if (pParentMenu != nullptr) {
                CMFCToolBarMenuButton* pParentBtn = At<CMFCToolBarMenuButton*>(pParentMenu, kOffPopupParentBtn);
                if (pParentBtn != nullptr &&
                    impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pParentBtn, impl__GetThisClass_CMFCCustomizeButton__SAPEAUCRuntimeClass__XZ()) &&
                    impl__InvokeCommand_CMFCCustomizeButton__UEAAHPEAVCMFCPopupMenuBar__PEBVCMFCToolBarButton___Z(pParentBtn, pThis, pMenuItem)) {   // vslot 0x208
                    bHandled = true;
                }
            }
            if (!bHandled) {
                impl__AddCmd_CMFCCmdUsageCount__QEAAXI_Z(impl__m_UsageCount_CMFCToolBar__1VCMFCCmdUsageCount__A, uiCmdId);
                if (pParentMenu != nullptr) {
                    if (impl__PostCommand_CMFCPopupMenu__QEAAHI_Z(pParentMenu, uiCmdId)) {
                        bHandled = true;
                    } else if (impl__afxUserToolsManager__3PEAVCUserToolsManager__EA != nullptr &&
                               impl__InvokeTool_CUserToolsManager__QEAAHI_Z(impl__afxUserToolsManager__3PEAVCUserToolsManager__EA, uiCmdId)) {
                        bHandled = true;
                    } else {
                        const UINT msg = (uiCmdId - 0xF000u < 0x1F0u) ? WM_SYSCOMMAND : WM_COMMAND;
                        CWnd* pOwner = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(OwnerHwnd(pThis));
                        if (pOwner != nullptr) {
                            ::PostMessage(HWndOf(pOwner), msg, uiCmdId, 0);
                        }
                        void* pElem = At<void*>(pParentMenu, kOffPopupParentRibbonElement);
                        if (pElem != nullptr) {
                            impl__SetDroppedDown_CMFCRibbonBaseElement__IEAAXPEAVCMFCPopupMenu___Z(pElem, nullptr);
                            At<void*>(pParentMenu, kOffPopupParentRibbonElement) = nullptr;
                            impl__PostMenuCommand_CMFCRibbonBaseElement__QEAAXI_Z(pElem, uiCmdId);
                        }
                    }
                }
            }
        }
    }

    impl__m_nLastCommandIndex_CMFCPopupMenuBar__1HA =
        (pMenuItem == nullptr) ? -1 : impl__ButtonToIndex_CMFCToolBar__QEBAHPEBVCMFCToolBarButton___Z(AsToolBar(pThis), pMenuItem);

    if (At<int>(pThis, kOffPaletteMode) != 0) {
        pParentMenu = ParentPopupMenu(pThis);
        if (pParentMenu != nullptr) {
            CMFCToolBarMenuButton* pBtn = At<CMFCToolBarMenuButton*>(pParentMenu, kOffPopupParentBtn);
            if (pBtn != nullptr && pBtn->m_pWndParent != nullptr) {
                pBtn->m_nID = uiCmdId;
                // TODO(clean-room): afxCommandManager->m_CommandIndex.Lookup + pBtn->SetImage (vslot 0x190) not reproduced.
                CRect rectImage(0, 0, 0, 0);
                impl__GetImageRect_CMFCToolBarMenuButton__QEAAXAEAVCRect___Z(pBtn, &rectImage);
                const HWND hParent = HWndOf(pBtn->m_pWndParent);
                if (hParent != nullptr) {
                    ::InvalidateRect(hParent, static_cast<const RECT*>(rectImage), TRUE);
                    ::UpdateWindow(hParent);
                }
            }
        }
    }

    CFrameWnd* pFrame = impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(static_cast<const CWnd*>(pThis));
    impl__SetInCommand_CMFCPopupMenuBar__IEAAXH_Z(pThis, FALSE);
    if (pFrame != nullptr) {
        impl__DestroyWindow_CWnd__UEAAHXZ(pFrame);                                        // vslot 0xd0
    }
    (void)hWnd;
}

// Retail (RVA 0xbf5e0, mfc140u), fully transcribed -- a tail jump:
//     return afxMenuHash.LoadMenuBar(hMenu, this);        // 0x3b1d10, 0x8ee80
// ?afxMenuHash@@3VCMenuHash@@A is a zero-initialised 128-byte blob in
// featurepack/CMFC_misc_stubs.cpp (retail's object is 72 bytes); CMenuHash::LoadMenuBar (core/window/CMenuHash.cpp)
// keys its side table by the object address and never dereferences it, so
// the blob is a valid `this` for it.  Note that OpenMFC's CMenuHash records the
// (hMenu -> bar) pair instead of restoring an archived bar state, so the
// observable effect is only the TRUE return.
// Symbol: ?LoadFromHash@CMFCPopupMenuBar@@QEAAHPEAUHMENU__@@@Z
extern "C" int MS_ABI impl__LoadFromHash_CMFCPopupMenuBar__QEAAHPEAUHMENU_____Z(CMFCPopupMenuBar* pThis, HMENU hMenu)
{
    if (!pThis) return FALSE;
    return impl__LoadMenuBar_CMenuHash__QEAAHPEAUHMENU____PEAVCMFCToolBar___Z(
        reinterpret_cast<CMenuHash*>(impl__afxMenuHash__3VCMenuHash__A), hMenu, AsToolBar(pThis));
}

// Retail (RVA 0xbf930, mfc140u), fully transcribed:
//     AdjustLayout();                                                          // vslot 0x428 (slot 133 = ?AdjustLayout@CMFCPopupMenuBar@@)
//     ::RedrawWindow(m_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);   // 0x105, tail jump
// AdjustLayout is reached through this file's exported thunk, i.e. the C++
// virtual -- a derived override is dispatched only for OpenMFC-built objects.
// Symbol: ?OnAfterButtonDelete@CMFCPopupMenuBar@@MEAAXXZ
extern "C" void MS_ABI impl__OnAfterButtonDelete_CMFCPopupMenuBar__MEAAXXZ(CMFCPopupMenuBar* pThis)
{
    if (!pThis) return;
    impl__AdjustLayout_CMFCPopupMenuBar__MEAAXXZ(pThis);
    ::RedrawWindow(HWndOf(pThis), nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}

// Retail (RVA 0xbf8a0, mfc140u), fully transcribed:
//     CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);
//     rectSeparator = pButton->m_rect;                                 // +0x68
//     if (pButton->m_bWrap && bHorz && m_bPaletteMode) {               // +0x14, +0x1390
//         rectSeparator.right  = rectClient.right;
//         rectSeparator.top    = pButton->m_rect.bottom;
//         rectSeparator.bottom = pButton->m_rect.bottom + 5;
//     }
// (The base CMFCToolBar::OnCalcSeparatorRect is NOT called.)
// Symbol: ?OnCalcSeparatorRect@CMFCPopupMenuBar@@MEAAXPEAVCMFCToolBarButton@@AEAVCRect@@H@Z
extern "C" void MS_ABI impl__OnCalcSeparatorRect_CMFCPopupMenuBar__MEAAXPEAVCMFCToolBarButton__AEAVCRect__H_Z(
    CMFCPopupMenuBar* pThis, CMFCToolBarButton* pButton, CRect* pRectSeparator, int bHorz)
{
    if (!pThis || !pButton || !pRectSeparator) return;
    RECT rectClient = {};
    ::GetClientRect(HWndOf(pThis), &rectClient);
    *pRectSeparator = pButton->m_rect;
    if (pButton->m_bWrap && bHorz && At<int>(pThis, kOffPaletteMode) != 0) {
        pRectSeparator->right  = rectClient.right;
        pRectSeparator->top    = pButton->m_rect.bottom;
        pRectSeparator->bottom = pButton->m_rect.bottom + 5;
    }
}

// Retail (RVA 0xbd940, mfc140u), transcribed (deviations after the body):
//     if (!::IsWindow(m_hWnd)) AfxThrowInvalidArgException();
//     if (iHot == -1) {
//         CPoint pt; ::GetCursorPos(&pt); ::ScreenToClient(m_hWnd, &pt);
//         if (HitTest(pt) == m_iHot) { m_iHighlighted = m_iHot; return; }     // vslot 0x730, +0x1140, +0x1138
//     }
//     CMFCToolBarMenuButton* pCurrPopupMenuButton = NULL;                     // first dropped-down menu button
//     for (POSITION pos = m_Buttons.GetHeadPosition(); pos != NULL; ) {       // +0x1190
//         CMFCToolBarButton* p = m_Buttons.GetNext(pos);
//         if (p && p->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton)) && p->IsDroppedDown())   // vslot 0xf0 = `m_pPopupMenu != NULL`
//             { pCurrPopupMenuButton = (CMFCToolBarMenuButton*)p; break; }
//     }
//     CMFCToolBarMenuButton* pMenuButton = NULL;
//     if (iHot >= 0) {
//         CMFCToolBarButton* p = GetButton(iHot);  if (!p) AfxThrowInvalidArgException();   // 0x14fe00
//         pMenuButton = DYNAMIC_DOWNCAST(CMFCToolBarMenuButton, p);
//     }
//     if (pMenuButton != pCurrPopupMenuButton) {
//         CMFCPopupMenu* pParentMenu = DYNAMIC_DOWNCAST(CMFCPopupMenu, CWnd::FromHandle(::GetParent(m_hWnd)));
//         if (pCurrPopupMenuButton != NULL) {
//             const MSG* pMsg = CWnd::GetCurrentMessage();                       // 0x28ac10
//             if (!CMFCToolBar::m_bCustomizeMode && !(pMsg && pMsg->message == WM_KEYDOWN)) {
//                 m_pDelayedClosePopupMenuButton = pCurrPopupMenuButton;         // +0x13e0
//                 pCurrPopupMenuButton->m_bToBeClosed = TRUE;                    // +0xdc
//                 ::SetTimer(m_hWnd, 0xEC18, m_uiPopupTimerDelay - 1, NULL);
//                 ::InvalidateRect(m_hWnd, &pCurrPopupMenuButton->m_rect, TRUE); ::UpdateWindow(m_hWnd);
//             } else {
//                 ::KillTimer(m_hWnd, 0xEC18);  m_pDelayedClosePopupMenuButton = NULL;
//                 pCurrPopupMenuButton->OnCancelMode();                          // vslot 0xc0 (slot 24 of 0x3184e8)
//                 if (pParentMenu != NULL)
//                     CMFCPopupMenu::ActivatePopupMenu(g_pTopLevelFrame ? g_pTopLevelFrame : GetTopLevelFrame(), pParentMenu);  // 0xb7920
//             }
//         }
//         if (pMenuButton != NULL && (pMenuButton->m_nID == (UINT)-1 || pMenuButton->m_bDrawDownArrow))   // +0x24, +0xc8
//             pMenuButton->OnClick(this, TRUE);                                  // vslot 0x50
//         if (pParentMenu != NULL && pParentMenu->GetParentPopupMenu() != NULL) {          // 0xb7550
//             CMFCPopupMenuBar* pParentBar = pParentMenu->GetParentPopupMenu()->GetMenuBar();   // vslot 0x3a0
//             if (pParentBar != NULL && pParentBar->m_pDelayedClosePopupMenuButton == pParentMenu->m_pParentBtn)   // +0x228
//                 pParentBar->RestoreDelayedSubMenu();                           // vslot 0x898
//         }
//     } else if (pMenuButton != NULL && pMenuButton == m_pDelayedClosePopupMenuButton) {
//         m_pDelayedClosePopupMenuButton->m_bToBeClosed = FALSE;
//         m_pDelayedClosePopupMenuButton = NULL;  ::KillTimer(m_hWnd, 0xEC18);
//     }
//     m_iHot = iHot;                                                             // +0x1140
//     if (m_bDropDownListMode) {                                                 // +0x1398
//         CMFCPopupMenu* p = DYNAMIC_DOWNCAST(CMFCPopupMenu, CWnd::FromHandle(::GetParent(m_hWnd)));
//         if (p != NULL) p->OnChangeHot(m_iHot);                                 // vslot 0x418 (slot 131 of 0x2f85b0 = bare `ret`)
//     }
//     if (CMFCPopupMenu::m_bSendMenuSelectMsg) {                                 // 0x3be298
//         CWnd* pMsgWnd = g_pTopLevelFrame;                                      // 0x3be1b8
//         if (pMsgWnd == NULL) { pMsgWnd = GetTopLevelFrame();                   // 0x28e490
//             if (pMsgWnd == NULL) { CWinThread* pThread = AfxGetModuleThreadState()->m_pCurrentWinThread;   // +8
//                                    pMsgWnd = pThread ? pThread->GetMainWnd() : NULL; } }   // CWinThread vslot 0xf8
//         CMFCPopupMenu* pParentMenu = DYNAMIC_DOWNCAST(CMFCPopupMenu, CWnd::FromHandle(::GetParent(m_hWnd)));
//         if (pParentMenu == NULL) return;
//         if (pParentMenu->m_pMessageWnd != NULL) pMsgWnd = pParentMenu->m_pMessageWnd;   // +0x1f8
//         else if (pMsgWnd == NULL) return;
//         UINT nFlags, nItem;
//         if (pMenuButton != NULL) {
//             nFlags = MF_HILITE | ((pMenuButton->m_nStyle & TBBS_DISABLED) ? MF_DISABLED : 0);   // (style & 0x40000 | 0x1000000) >> 17
//             if (pMenuButton->m_nStyle & TBBS_CHECKED) nFlags |= MF_CHECKED;
//             if (pMenuButton->m_nID == (UINT)-1) { nFlags |= MF_POPUP; nItem = iHot; } else nItem = pMenuButton->m_nID;
//         } else { nFlags = MF_HILITE; nItem = 0; }
//         ::SendMessage(pMsgWnd->m_hWnd, WM_MENUSELECT, MAKEWPARAM(nItem, nFlags), (LPARAM)pParentMenu->m_hMenu);   // +0x1658
//     }
// DEVIATIONS:
//   * The buttons are walked through GetCount()/GetButton() (side table), not
//     the real m_Buttons CObList that OpenMFC leaves empty.
//   * IsDroppedDown, OnCancelMode, OnClick, GetMenuBar, RestoreDelayedSubMenu
//     are folded to their retail base bodies / exported thunks (no dispatch
//     to a client override); OnChangeHot on the parent popup (base body `ret`)
//     is omitted.  OnClick / OnCancelMode are still stubs in
//     featurepack/toolbar/CMFCToolBarMenuButton.cpp, so no submenu opens yet.
//   * AfxGetModuleThreadState()->m_pCurrentWinThread is not modelled; the
//     equivalent AfxGetThread() is used for the main-window fallback.
//   * The two AfxThrowInvalidArgException paths (0xbdd70) go through the
//     exported thunk, as the sibling bodies do; the `return` after each call
//     is only there for a non-throwing build.
// Symbol: ?OnChangeHot@CMFCPopupMenuBar@@MEAAXH@Z
extern "C" void MS_ABI impl__OnChangeHot_CMFCPopupMenuBar__MEAAXH_Z(CMFCPopupMenuBar* pThis, int iHot)
{
    if (!pThis) return;
    const HWND hWnd = HWndOf(pThis);
    if (!::IsWindow(hWnd)) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    CMFCToolBar* pBar = AsToolBar(pThis);

    if (iHot == -1) {
        POINT pt = {};
        ::GetCursorPos(&pt);
        ::ScreenToClient(hWnd, &pt);
        if (impl__HitTest_CMFCToolBar__UEAAHVCPoint___Z(pBar, CPoint(pt)) == pBar->m_iHot) {   // vslot 0x730
            pBar->m_iHighlighted = pBar->m_iHot;
            return;
        }
    }

    CMFCToolBarMenuButton* pCurrPopupMenuButton = nullptr;
    const int nCount = impl__GetCount_CMFCToolBar__QEBAHXZ(pBar);
    for (int i = 0; i < nCount; ++i) {
        CMFCToolBarButton* p = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, i);
        if (IsMenuButton(p) && At<void*>(p, kOffMbPopupMenu) != nullptr) {          // IsDroppedDown(), vslot 0xf0
            pCurrPopupMenuButton = static_cast<CMFCToolBarMenuButton*>(p);
            break;
        }
    }

    CMFCToolBarMenuButton* pMenuButton = nullptr;
    if (iHot >= 0) {
        CMFCToolBarButton* p = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, iHot);
        if (p == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
        pMenuButton = IsMenuButton(p) ? static_cast<CMFCToolBarMenuButton*>(p) : nullptr;
    }

    if (pMenuButton != pCurrPopupMenuButton) {
        CMFCPopupMenu* pParentMenu = ParentPopupMenu(pThis);
        if (pCurrPopupMenuButton != nullptr) {
            const MSG* pMsg = impl__GetCurrentMessage_CWnd__KAPEBUtagMSG__XZ();
            if (impl__m_bCustomizeMode_CMFCToolBar__1HA == 0 && !(pMsg != nullptr && pMsg->message == WM_KEYDOWN)) {
                At<CMFCToolBarMenuButton*>(pThis, kOffDelayedClosePopupMenuButton) = pCurrPopupMenuButton;
                At<int>(pCurrPopupMenuButton, kOffMbToBeClosed) = TRUE;
                ::SetTimer(hWnd, kTimerDelayedClose, impl__m_uiPopupTimerDelay_CMFCPopupMenuBar__1IA - 1, nullptr);
                ::InvalidateRect(hWnd, static_cast<const RECT*>(pCurrPopupMenuButton->m_rect), TRUE);
                ::UpdateWindow(hWnd);
            } else {
                ::KillTimer(hWnd, kTimerDelayedClose);
                At<CMFCToolBarMenuButton*>(pThis, kOffDelayedClosePopupMenuButton) = nullptr;
                impl__OnCancelMode_CMFCToolBarMenuButton__UEAAXXZ(pCurrPopupMenuButton);   // vslot 0xc0
                if (pParentMenu != nullptr) {
                    CFrameWnd* pFrame = static_cast<CFrameWnd*>(impl__g_pTopLevelFrame__3PEAVCFrameWnd__EA);
                    if (pFrame == nullptr) pFrame = impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(static_cast<const CWnd*>(pThis));
                    impl__ActivatePopupMenu_CMFCPopupMenu__SAHPEAVCFrameWnd__PEAV1__Z(pFrame, pParentMenu);
                }
            }
        }

        if (pMenuButton != nullptr &&
            (pMenuButton->m_nID == static_cast<unsigned int>(-1) || At<int>(pMenuButton, kOffMbDrawDownArrow) != 0)) {
            impl__OnClick_CMFCToolBarMenuButton__UEAAHPEAVCWnd__H_Z(pMenuButton, AsWnd(pThis), TRUE);   // vslot 0x50
        }

        if (pParentMenu != nullptr) {
            void* pGrandParent = impl__GetParentPopupMenu_CMFCPopupMenu__QEBAPEAV1_XZ(pParentMenu);
            if (pGrandParent != nullptr) {
                CMFCPopupMenuBar* pParentBar = PopupMenuBarOf(pGrandParent);          // vslot 0x3a0
                if (At<void*>(pParentBar, kOffDelayedClosePopupMenuButton) == At<void*>(pParentMenu, kOffPopupParentBtn)) {
                    impl__RestoreDelayedSubMenu_CMFCPopupMenuBar__UEAAXXZ(pParentBar);   // vslot 0x898
                }
            }
        }
    } else if (pMenuButton != nullptr && pMenuButton == At<CMFCToolBarMenuButton*>(pThis, kOffDelayedClosePopupMenuButton)) {
        At<int>(pMenuButton, kOffMbToBeClosed) = FALSE;
        At<CMFCToolBarMenuButton*>(pThis, kOffDelayedClosePopupMenuButton) = nullptr;
        ::KillTimer(hWnd, kTimerDelayedClose);
    }

    pBar->m_iHot = iHot;
    if (At<int>(pThis, kOffDropDownListMode) != 0) {
        // pParentMenu->OnChangeHot(m_iHot): vslot 0x418, base body is a bare `ret`.
    }

    if (impl__m_bSendMenuSelectMsg_CMFCPopupMenu__1HA != 0) {
        CWnd* pMsgWnd = static_cast<CWnd*>(impl__g_pTopLevelFrame__3PEAVCFrameWnd__EA);
        if (pMsgWnd == nullptr) {
            pMsgWnd = impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(static_cast<const CWnd*>(pThis));
            if (pMsgWnd == nullptr) {
                CWinThread* pThread = impl__AfxGetThread__YAPEAVCWinThread__XZ();
                pMsgWnd = pThread ? impl__GetMainWnd_CWinThread__UEAAPEAVCWnd__XZ(pThread) : nullptr;
            }
        }
        CMFCPopupMenu* pParentMenu = ParentPopupMenu(pThis);
        if (pParentMenu == nullptr) return;
        if (At<CWnd*>(pParentMenu, kOffPopupMessageWnd) != nullptr) {
            pMsgWnd = At<CWnd*>(pParentMenu, kOffPopupMessageWnd);
        } else if (pMsgWnd == nullptr) {
            return;
        }

        unsigned int nFlags;
        unsigned int nItem;
        if (pMenuButton != nullptr) {
            nFlags = MF_HILITE | ((pMenuButton->m_nStyle & kTbbsDisabled) ? MF_DISABLED : 0);
            if (pMenuButton->m_nStyle & kTbbsChecked) nFlags |= MF_CHECKED;
            if (pMenuButton->m_nID == static_cast<unsigned int>(-1)) {
                nFlags |= MF_POPUP;
                nItem = static_cast<unsigned int>(iHot);
            } else {
                nItem = pMenuButton->m_nID;
            }
        } else {
            nFlags = MF_HILITE;
            nItem = 0;
        }
        ::SendMessage(HWndOf(pMsgWnd), WM_MENUSELECT, MAKEWPARAM(static_cast<WORD>(nItem), static_cast<WORD>(nFlags)),
                      reinterpret_cast<LPARAM>(At<HMENU>(pParentMenu, kOffPopupHMenu)));
    }
}

// Retail (RVA 0xbf180, mfc140u), fully transcribed:
//     if (CMFCToolBar::OnCreate(lpCreateStruct) == -1) return -1;              // 0x151ff0
//     if (m_uiPopupTimerDelay == (UINT)-1) {                                     // 0x3b1b38
//         m_uiPopupTimerDelay = 500;
//         CSettingsStoreSP regSP;                                                // 16 zeroed bytes at [rsp+0x20]
//         CSettingsStore& reg = regSP.Create(FALSE, TRUE);                       // 0x12a550
//         if (reg.Open(L"Control Panel\\Desktop")) {                             // vslot 0x30 (slot 6 of 0x30e610 = ?Open@CSettingsStore@@)
//             CString str;
//             if (reg.Read(L"MenuShowDelay", str))                               // vslot 0xa8 (slot 21 = ?Read@CSettingsStore@@..CString&)
//                 m_uiPopupTimerDelay = min(_wtol(str), 5000);                   // cmova against 0x1388
//         }
//         (regSP dtor: delete the store through its deleting dtor, vslot 0x08)
//     }
//     ::GetCursorPos(&m_ptCursor);                                               // +0x13a8
//     return 0;
// The store's two virtuals go through the exported CSettingsStore thunks
// (DEVIATION for a client CSettingsStoreSP::m_pRTIDefault override).  The
// store is released with `delete` through CObject's virtual destructor, which
// is what the deleting dtor call amounts to for an OpenMFC-built store.
// KNOWN GAP (outside this file): retail's .data holds 0xffffffff at
// ?m_uiPopupTimerDelay@ (0x3b1b38, mfc140u), which is what arms the lazy read
// above; featurepack/menu/StaticData.cpp defines the OpenMFC static as 0, so
// the `== -1` gate never opens, the registry is never consulted and the delay
// stays 0 until a client assigns the static itself (OnChangeHot's
// SetTimer(0xEC18, m_uiPopupTimerDelay - 1) then asks for a 0xffffffff ms
// timer).  Even once the gate opens, OpenMFC's CSettingsStore is an in-memory
// store, so MenuShowDelay is not found and 500 stands.  Reported as a
// StaticData.cpp fix (initialise to (UINT)-1); not changed here.
// Symbol: ?OnCreate@CMFCPopupMenuBar@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCPopupMenuBar__IEAAHPEAUtagCREATESTRUCTW___Z(CMFCPopupMenuBar* pThis, LPCREATESTRUCTW lpCreateStruct)
{
    if (!pThis) return -1;
    if (impl__OnCreate_CMFCToolBar__IEAAHPEAUtagCREATESTRUCTW___Z(AsToolBar(pThis), lpCreateStruct) == -1) {
        return -1;
    }

    if (impl__m_uiPopupTimerDelay_CMFCPopupMenuBar__1IA == static_cast<std::uint32_t>(-1)) {
        impl__m_uiPopupTimerDelay_CMFCPopupMenuBar__1IA = 500;

        void* regSP[2] = { nullptr, nullptr };                // CSettingsStoreSP: m_pRegistry, m_dwUserData
        void* pStore = impl__Create_CSettingsStoreSP__QEAAAEAVCSettingsStore__HH_Z(regSP, FALSE, TRUE);
        if (pStore != nullptr) {
            if (impl__Open_CSettingsStore__UEAAHPEB_W_Z(pStore, L"Control Panel\\Desktop")) {
                CString str;
                if (impl__Read_CSettingsStore__UEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
                        pStore, L"MenuShowDelay", &str)) {
                    const long lDelay = ::_wtol(static_cast<const wchar_t*>(str));
                    impl__m_uiPopupTimerDelay_CMFCPopupMenuBar__1IA =
                        (static_cast<std::uint32_t>(lDelay) > 5000u) ? 5000u : static_cast<std::uint32_t>(lDelay);
                }
            }
        }
        if (regSP[0] != nullptr) {
            delete static_cast<CObject*>(regSP[0]);           // CSettingsStoreSP::~CSettingsStoreSP
        }
    }

    ::GetCursorPos(&At<POINT>(pThis, kOffPtCursor));
    return 0;
}

// Retail (RVA 0xbdd80, mfc140u), fully transcribed:
//     ::KillTimer(m_hWnd, 0xEC17);  ::KillTimer(m_hWnd, 0xEC18);
//     m_pDelayedPopupMenuButton = m_pDelayedClosePopupMenuButton = NULL;        // +0x13d8, +0x13e0
//     for (POSITION pos = m_Buttons.GetHeadPosition(); pos != NULL; ) {         // +0x1190
//         CMFCToolBarButton* p = m_Buttons.GetNext(pos);
//         if (!p || !p->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton)) || !p->IsDroppedDown()) continue;   // vslot 0xf0
//         CMFCPopupMenu* pPopup = ((CMFCToolBarMenuButton*)p)->m_pPopupMenu;     // +0xc0
//         if (pPopup == NULL || !::IsWindow(pPopup->m_hWnd)) continue;
//         pPopup->SaveState();                                                   // vslot 0x3d0 (slot 122 of 0x2f85b0 = ?SaveState@CMFCPopupMenu@@)
//         ::PostMessage(pPopup->m_hWnd, WM_CLOSE, 0, 0);
//     }
//     CMFCToolBar::OnDestroy();                                                  // 0x152fd0, tail jump
// DEVIATIONS: side-table button walk (GetCount/GetButton); IsDroppedDown folded
// to its base body; SaveState reached through the CMFCPopupMenu thunk.
// Symbol: ?OnDestroy@CMFCPopupMenuBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCPopupMenuBar__IEAAXXZ(CMFCPopupMenuBar* pThis)
{
    if (!pThis) return;
    const HWND hWnd = HWndOf(pThis);
    ::KillTimer(hWnd, kTimerDelayedPopup);
    ::KillTimer(hWnd, kTimerDelayedClose);
    At<void*>(pThis, kOffDelayedPopupMenuButton) = nullptr;
    At<void*>(pThis, kOffDelayedClosePopupMenuButton) = nullptr;

    CMFCToolBar* pBar = AsToolBar(pThis);
    const int nCount = impl__GetCount_CMFCToolBar__QEBAHXZ(pBar);
    for (int i = 0; i < nCount; ++i) {
        CMFCToolBarButton* p = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, i);
        if (!IsMenuButton(p)) continue;
        void* pPopup = At<void*>(p, kOffMbPopupMenu);                                  // IsDroppedDown() + m_pPopupMenu
        if (pPopup == nullptr || !::IsWindow(HWndOf(pPopup))) continue;
        impl__SaveState_CMFCPopupMenu__UEAAXXZ(pPopup);                                // vslot 0x3d0
        ::PostMessage(HWndOf(pPopup), WM_CLOSE, 0, 0);
    }

    impl__OnDestroy_CMFCToolBar__IEAAXXZ(pBar);
}

// Retail (RVA 0xbed80, mfc140u), fully transcribed:
//     if (!(dwKeyState & MK_CONTROL)) {                                          // 0x8
//         CMFCPopupMenu* pParentMenu = DYNAMIC_DOWNCAST(CMFCPopupMenu, CWnd::FromHandle(::GetParent(m_hWnd)));
//         if (pParentMenu != NULL) {
//             CMFCToolBar* pToolBar = pParentMenu->GetParentToolBar();           // 0xb75f0
//             if (pToolBar != NULL && pParentMenu->m_pParentBtn != NULL &&       // +0x228
//                 pParentMenu->m_pParentBtn == pToolBar->m_pDragButton)          // +0x12f0
//                 return DROPEFFECT_NONE;   // dragging the very button that opened this menu
//         }
//     }
//     return CMFCToolBar::OnDragOver(pDataObject, dwKeyState, point);           // 0x152340
// Symbol: ?OnDragOver@CMFCPopupMenuBar@@MEAAKPEAVCOleDataObject@@KVCPoint@@@Z
extern "C" unsigned long MS_ABI impl__OnDragOver_CMFCPopupMenuBar__MEAAKPEAVCOleDataObject__KVCPoint___Z(
    CMFCPopupMenuBar* pThis, COleDataObject* pDataObject, unsigned long dwKeyState, long long pointPacked)
{
    if (!pThis) return 0;   // DROPEFFECT_NONE
    if (!(dwKeyState & MK_CONTROL)) {
        CMFCPopupMenu* pParentMenu = ParentPopupMenu(pThis);
        if (pParentMenu != nullptr) {
            CMFCToolBar* pToolBar = impl__GetParentToolBar_CMFCPopupMenu__QEBAPEAVCMFCToolBar__XZ(pParentMenu);
            void* pParentBtn = At<void*>(pParentMenu, kOffPopupParentBtn);
            if (pToolBar != nullptr && pParentBtn != nullptr && pParentBtn == pToolBar->m_pDragButton) {
                return 0;   // DROPEFFECT_NONE
            }
        }
    }
    return impl__OnDragOver_CMFCToolBar__MEAAKPEAVCOleDataObject__KVCPoint___Z(
        AsToolBar(pThis), pDataObject, dwKeyState, UnpackPoint(pointPacked));
}

// Retail (RVA 0xbee10, mfc140u), fully transcribed:
//     if (CMFCToolBar::m_bCustomizeMode || !CMFCMenuBar::m_bRecentlyUsedMenus || m_bPaletteMode) return;   // 0x3be35c, 0x3b1b08, +0x1390
//     CMFCPopupMenu* pParentMenu = DYNAMIC_DOWNCAST(CMFCPopupMenu, CWnd::FromHandle(::GetParent(m_hWnd)));
//     if (pParentMenu == NULL || pParentMenu->m_pParentBtn == NULL) return;      // +0x228
//     // Frame every run of rarely-used items:
//     CRect rectGroup;  BOOL bFirst = TRUE;
//     for (POSITION pos = m_Buttons.GetHeadPosition(); pos != NULL; ) {         // +0x1190
//         CMFCToolBarButton* pButton = m_Buttons.GetNext(pos);  if (!pButton) AfxThrowInvalidArgException();
//         if ((pButton->m_nStyle & TBBS_SEPARATOR) && pos != NULL &&            // +0x28 bit 0
//             CMFCToolBar::IsCommandRarelyUsed(m_Buttons.GetAt(pos)->m_nID)) continue;   // 0x158c60: a separator inside a rarely-used run
//         if (CMFCToolBar::IsCommandRarelyUsed(pButton->m_nID)) {
//             if (bFirst) { rectGroup.left = pButton->m_rect.left; rectGroup.top = pButton->m_rect.top; rectGroup.right = pButton->m_rect.right; bFirst = FALSE; }   // +0x68
//             if (pos != NULL) continue;
//             rectGroup.bottom = pButton->m_rect.bottom;                         // run reaches the last button
//         } else {
//             if (bFirst) continue;
//             rectGroup.bottom = pButton->m_rect.top;  bFirst = TRUE;
//         }
//         CMFCVisualManager::GetInstance()->OnHighlightRarelyUsedMenuItems(pDC, rectGroup);   // 0x9774, vftable +0x150 (slot 42 of 0x18031c128 = 0x184e10)
//     }
// DEVIATIONS: side-table button walk; the manager is read from
// ?m_pVisManager@ (no lazy default construction) and the highlight goes to
// the exported CMFCVisualManager base body, which does not dispatch to a
// derived manager's override.
// Symbol: ?OnFillBackground@CMFCPopupMenuBar@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnFillBackground_CMFCPopupMenuBar__MEAAXPEAVCDC___Z(CMFCPopupMenuBar* pThis, CDC* pDC)
{
    if (!pThis || !pDC) return;
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0) return;
    if (impl__m_bRecentlyUsedMenus_CMFCMenuBar__1HA == 0) return;
    if (At<int>(pThis, kOffPaletteMode) != 0) return;

    CMFCPopupMenu* pParentMenu = ParentPopupMenu(pThis);
    if (pParentMenu == nullptr || At<void*>(pParentMenu, kOffPopupParentBtn) == nullptr) return;

    CMFCToolBar* pBar = AsToolBar(pThis);
    const int nCount = impl__GetCount_CMFCToolBar__QEBAHXZ(pBar);
    CRect rectGroup(0, 0, 0, 0);
    bool bFirst = true;
    for (int i = 0; i < nCount; ++i) {
        CMFCToolBarButton* pButton = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, i);
        if (pButton == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
        const bool bHasNext = (i + 1 < nCount);
        if ((pButton->m_nStyle & kTbbsSeparator) && bHasNext) {
            CMFCToolBarButton* pNext = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, i + 1);
            if (pNext != nullptr && impl__IsCommandRarelyUsed_CMFCToolBar__SAHI_Z(pNext->m_nID)) continue;
        }
        if (impl__IsCommandRarelyUsed_CMFCToolBar__SAHI_Z(pButton->m_nID)) {
            if (bFirst) {
                rectGroup.left  = pButton->m_rect.left;
                rectGroup.top   = pButton->m_rect.top;
                rectGroup.right = pButton->m_rect.right;
                bFirst = false;
            }
            if (bHasNext) continue;
            rectGroup.bottom = pButton->m_rect.bottom;
        } else {
            if (bFirst) continue;
            rectGroup.bottom = pButton->m_rect.top;
            bFirst = true;
        }
        CMFCVisualManager* pVM = static_cast<CMFCVisualManager*>(impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
        if (pVM != nullptr) {
            impl__OnHighlightRarelyUsedMenuItems_CMFCVisualManager__UEAAXPEAVCDC__VCRect___Z(pVM, pDC, rectGroup);
        }
    }
}

// Retail (RVA 0xbf3d0, mfc140u), fully transcribed (wParam/lParam are not read):
//     if (m_bTrackMode) return 0;                                                // +0x1388
//     if (!(GetStyle() & WS_VISIBLE)) return 0;                                  // 0x2a9690, bit 28
//     CFrameWnd* pTarget = (CFrameWnd*)GetCommandTarget();                       // vslot 0x798 (slot 243 = ?GetCommandTarget@CMFCPopupMenuBar@@)
//     if (pTarget == NULL || !pTarget->IsFrameWnd()) {                           // vslot 0x2b0 (slot 86)
//         pTarget = AFXGetParentFrame(this);  if (pTarget == NULL) return 0;     // 0x6bd00
//     }
//     BOOL bDisableIfNoHandler = pTarget->IsFrameWnd() ? pTarget->m_bAutoMenuEnable : FALSE;   // CFrameWnd + 0xe8
//     OnUpdateCmdUI(pTarget, bDisableIfNoHandler);                               // vslot 0x498 (slot 147 = ?OnUpdateCmdUI@CMFCToolBar@@)
//     return 0;
// DEVIATIONS: GetCommandTarget / OnUpdateCmdUI are reached through the exported
// thunks (no dispatch to a client override); IsFrameWnd through the CWnd thunk.
// OpenMFC's CFrameWnd carries no m_bAutoMenuEnable (its +0xe8 is m_pViewActive),
// so the CFrameWnd default TRUE is passed for a frame target.
// Symbol: ?OnIdleUpdateCmdUI@CMFCPopupMenuBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnIdleUpdateCmdUI_CMFCPopupMenuBar__IEAA_J_K_J_Z(
    CMFCPopupMenuBar* pThis, unsigned __int64 wParam, __int64 lParam)
{
    (void)wParam; (void)lParam;
    if (!pThis) return 0;
    if (At<int>(pThis, kOffTrackMode) != 0) return 0;
    if (!(impl__GetStyle_CWnd__QEBAKXZ(static_cast<const CWnd*>(pThis)) & WS_VISIBLE)) return 0;

    CWnd* pTarget = impl__GetCommandTarget_CMFCPopupMenuBar__MEBAPEAVCWnd__XZ(pThis);   // vslot 0x798
    if (pTarget == nullptr || !impl__IsFrameWnd_CWnd__UEBAHXZ(pTarget)) {
        pTarget = impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(static_cast<const CWnd*>(pThis));
        if (pTarget == nullptr) return 0;
    }
    int bDisableIfNoHandler = FALSE;
    if (impl__IsFrameWnd_CWnd__UEBAHXZ(pTarget)) {
        bDisableIfNoHandler = TRUE;   // retail: pTarget->m_bAutoMenuEnable (see DEVIATIONS)
    }
    impl__OnUpdateCmdUI_CMFCToolBar__UEAAXPEAVCFrameWnd__H_Z(AsToolBar(pThis), static_cast<CFrameWnd*>(pTarget), bDisableIfNoHandler);   // vslot 0x498
    return 0;
}

namespace {
// The two CMap lookups OnKey inlines (RVA 0xbfe24 for the CMFCToolBarButton*-
// valued m_AccelKeys, 0x236e0 for the UINT-valued m_HiddenItemsAccel; both
// mfc140u) share one shape: ATL CMap with m_pHashTable at +8 and
// m_nHashTableSize at +0x10, CAssoc = { KEY key; VALUE value; CAssoc* pNext;
// UINT nHashValue; } (value at +8, pNext at +0x10, nHashValue at +0x18 for a
// pointer VALUE), and the Park-Miller hash computed as
//     ldiv(key, 127773); h = 16807 * rem - 2836 * quot; if (h < 0) h += 0x7fffffff;
// (0xbfe34..0xbfe6b), bucket = h % m_nHashTableSize (`divl 0x10(%rdi)`).
// DEVIATION: retail divides before testing m_pHashTable; OpenMFC's
// CMFCToolBar ctor zeroes the 56-byte block, so a zero size or a NULL table
// is treated as "not found" instead of faulting.
inline unsigned ParkMillerHash(unsigned key) {
    const std::int32_t k = static_cast<std::int32_t>(key);
    const std::int32_t quot = k / 127773;
    const std::int32_t rem  = k % 127773;
    std::int32_t h = static_cast<std::int32_t>(16807u * static_cast<std::uint32_t>(rem)) -
                     static_cast<std::int32_t>(2836u * static_cast<std::uint32_t>(quot));
    if (h < 0) h += 0x7fffffff;
    return static_cast<unsigned>(h);
}
inline bool AccelKeyLookup(const void* pMap, unsigned key, CMFCToolBarButton** ppValue) {
    const void* const* pTable = At<const void* const*>(pMap, 0x08);
    const unsigned nSize = At<unsigned>(pMap, 0x10);
    if (pTable == nullptr || nSize == 0) return false;
    const unsigned nHash = ParkMillerHash(key);
    for (const char* pAssoc = static_cast<const char*>(pTable[nHash % nSize]); pAssoc != nullptr;
         pAssoc = At<const char*>(pAssoc, 0x10)) {
        if (At<unsigned>(pAssoc, 0x18) == nHash && At<unsigned>(pAssoc, 0x00) == key) {
            *ppValue = At<CMFCToolBarButton*>(pAssoc, 0x08);
            return true;
        }
    }
    return false;
}

// m_Buttons CObList positions, expressed as side-table indices: NULL == -1,
// GetNext == +1 (or -1 past the tail), GetPrev == -1 (or -1 before the head).
inline int NextIdx(int i, int n) { return (i >= 0 && i + 1 < n) ? i + 1 : -1; }
inline int PrevIdx(int i)        { return (i > 0) ? i - 1 : -1; }

// The "is this a real item" test of the two navigation loops (0xbe144 / 0xbe1f6):
//     !(m_nStyle & TBBS_SEPARATOR) && !::IsRectEmpty(&m_rect) && m_nID != (UINT)-2
inline bool IsNavigableItem(const CMFCToolBarButton* p) {
    return !(p->m_nStyle & kTbbsSeparator) &&
           !::IsRectEmpty(static_cast<const RECT*>(p->m_rect)) &&
           p->m_nID != static_cast<unsigned int>(-2);
}
} // namespace

// Retail (RVA 0xbde70, mfc140u), transcribed (deviations listed after the body).
//     int iSel = m_iHighlighted;                                                // +0x1138
//     POSITION pos = NULL; CMFCToolBarButton* pSelButton = NULL;
//     if (iSel >= 0 && iSel < m_Buttons.GetCount()) {                          // +0x11a0
//         pos = m_Buttons.FindIndex(iSel);                                     // walk from +0x1190
//         if (pos != NULL) pSelButton = m_Buttons.GetAt(pos);
//     }
//     if (nChar == VK_TAB) nChar = (::GetKeyState(VK_SHIFT) & 0x80) ? VK_UP : VK_DOWN;   // `test %al,%al; jns` (0x2c6ca0)
//     BOOL bResult = FALSE;                                                    // [rbp-0x40]
//     switch (nChar) {
//     case VK_HOME: pos = NULL;  /* fall through */
//     case VK_DOWN:
//         if (m_bDropDownListMode && posOrig == m_Buttons.GetTailPosition() && nChar != VK_HOME) return TRUE;   // +0x1398, +0x1198
//         bResult = TRUE;
//         if (m_Buttons.GetCount() == 0) return bResult;
//         { POSITION p = pos ? m_Buttons.GetNext-of(pos) : NULL;  int iIndex;
//           if (p == NULL) { p = m_Buttons.GetHeadPosition(); iIndex = 0; } else iIndex = iSel + 1;
//           if (p == pos) return bResult;
//           for (;;) {                                                         // 0xbe131
//               ENSURE(p != NULL);  pButton = m_Buttons.GetAt(p);  POSITION pNext = next-of(p);
//               if (IsNavigableItem(pButton)) break;
//               if (pNext == NULL) { if (m_bDropDownListMode) return TRUE; p = head; iIndex = 0; }
//               else               { p = pNext; iIndex++; }
//               if (p == pos) break;   // wrapped: pButton stays the LAST EXAMINED (non-navigable) item
//           }
//           goto select; }
//     case VK_END:  pos = NULL;  /* fall through */
//     case VK_UP:   // mirror image: GetTailPosition / GetPrev, iIndex = count (or iSel), then iIndex-- per step
//         if (m_bDropDownListMode && posOrig == m_Buttons.GetHeadPosition() && nChar != VK_END) return TRUE;
//         bResult = TRUE;  if (m_Buttons.GetCount() == 0) return bResult;
//         ... same loop shape (0xbe1d8..0xbe238) walking pPrev, wrapping to the tail ...
//         goto select;
//     case VK_PRIOR: case VK_NEXT:                                             // 0xbe323
//         if (!m_bDropDownListMode || m_nDropDownPageSize <= 0) return FALSE;  // +0x139c
//         { int iOld = iSel;  m_bInScrollMode = TRUE;                          // +0x13a0
//           UINT k = (nChar == VK_PRIOR) ? VK_UP : VK_DOWN;
//           for (int i = 0; i < m_nDropDownPageSize; i++) OnKey(k);            // vslot 0x810 (slot 258 = this function)
//           m_bInScrollMode = FALSE;
//           if (iOld != m_iHighlighted) AccNotifyObjectFocusEvent(m_iHighlighted);   // vslot 0x4c8
//           return TRUE; }
//     case VK_RETURN:                                                          // 0xbe3aa
//         bResult = TRUE;
//         if (pSelButton == NULL || !pSelButton->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton))) return TRUE;
//         if (!pMenuButton->HasButton() && pMenuButton->OpenPopupMenu(NULL)) return TRUE;   // vslot 0x1f0, vslot 0x1a8
//         ::SendMessage(GetOwner()->m_hWnd, WM_SETMESSAGESTRING, AFX_IDS_IDLEMESSAGE, 0);   // owner = +0xa0 ?: ::GetParent
//         OnSendCommand(pSelButton);                                           // vslot 0x7e0, result dropped
//         return TRUE;
//     default:                                                                 // 0xbdf63
//         if (CMFCToolBar::m_bCustomizeMode || (::GetAsyncKeyState(VK_CONTROL) & 0x8000)) return FALSE;
//         UINT uiChar = nChar;
//         if (CKeyboardManager::IsKeyPrintable(nChar)) {                       // 0x74290
//             uiChar = CKeyboardManager::TranslateCharToUpper(nChar);          // 0x74350
//             CMFCToolBarButton* pButton;
//             if (m_AccelKeys.Lookup(uiChar, pButton)) {                       // +0x1318, inlined GetAssocAt 0xbfe24
//                 int iIndex = iSel;                                           // then the index of pButton in m_Buttons (ENSURE on a NULL entry)
//                 if (pButton != NULL && pButton->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton))) {
//                     if (pMenuButton->OpenPopupMenu(NULL)) {                  // vslot 0x1a8
//                         if (pMenuButton->m_pPopupMenu != NULL)               // +0xc0
//                             ::SendMessage(pMenuButton->m_pPopupMenu->m_hWnd, WM_KEYDOWN, VK_HOME, 0);
//                     } else if (pMenuButton->m_nStyle & TBBS_DISABLED) {
//                         InvokeMenuCommand(0, pMenuButton);  return TRUE;     // 0xbc2d0
//                     } else {
//                         bResult = OnSendCommand(pMenuButton);  if (bResult) return TRUE;   // vslot 0x7e0
//                     }
//                 }
//                 goto select (with NO accessibility notification: ecx = 0 at 0xbe083);
//             }
//         }
//         if (!CMFCMenuBar::m_bRecentlyUsedMenus || m_bAreAllCommandsShown) return FALSE;   // 0x3b1b08, +0x1380
//         UINT uiCmd;
//         if (!m_HiddenItemsAccel.Lookup(uiChar, uiCmd)) return FALSE;         // +0x13e8, inlined GetAssocAt 0x236e0
//         InvokeMenuCommand(uiCmd, NULL);  return TRUE;
//     }
// select:                                                                     // 0xbe245
//     if (pButton == pSelButton) return bResult;
//     if (bNotify && !m_bInScrollMode) AccNotifyObjectFocusEvent(iIndex);      // vslot 0x4c8
//     if (!CMFCToolBar::m_bCustomizeMode) m_iSelected = iIndex;                // +0x113c
//     m_iHighlighted = iIndex;
//     CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);
//     CRect rectBtn = pButton->m_rect;
//     if (rectBtn.top >= rectClient.top && rectBtn.bottom <= rectClient.bottom) {   // 0xbe2b0 / 0xbe2c3
//         if (pSelButton != NULL) ::InvalidateRect(m_hWnd, &pSelButton->m_rect, TRUE);
//         ::InvalidateRect(m_hWnd, &rectBtn, TRUE);  ::UpdateWindow(m_hWnd);
//     }
//     if (pButton->m_nID != (UINT)-1) ShowCommandMessageString(pButton->m_nID);   // vslot 0x840 (slot 264)
//     return bResult;
// DEVIATIONS:
//   * The m_Buttons CObList is walked through GetCount()/GetButton() (side
//     table) with positions expressed as indices (see NextIdx / PrevIdx).
//   * The navigation loops in retail never terminate for a menu with no
//     navigable item and pos == NULL (VK_HOME / VK_END re-enter from the head /
//     tail for ever); the transcription gives up after one full wrap and
//     returns bResult.
//   * HasButton (vslot 0x1f0) is folded to its FALSE base body; OpenPopupMenu,
//     OnSendCommand, ShowCommandMessageString, AccNotifyObjectFocusEvent and
//     the OnKey recursion go through the exported thunks, non-virtually
//     (OpenPopupMenu is still a stub in featurepack/toolbar/CMFCToolBarMenuButton.cpp).
//   * m_HiddenItemsAccel (+0x13e8) lies outside OpenMFC's 0x13b0-byte object
//     and nothing in OpenMFC ever populates it; that lookup is not performed
//     (treated as "not found", i.e. the fallback returns FALSE).
//   * The ENSURE paths (0xbe472) call the exported AfxThrowInvalidArgException.
// Symbol: ?OnKey@CMFCPopupMenuBar@@MEAAHI@Z
extern "C" int MS_ABI impl__OnKey_CMFCPopupMenuBar__MEAAHI_Z(CMFCPopupMenuBar* pThis, unsigned int nChar)
{
    if (!pThis) return FALSE;
    CMFCToolBar* pBar = AsToolBar(pThis);
    const HWND hWnd = HWndOf(pThis);
    const int nCount = impl__GetCount_CMFCToolBar__QEBAHXZ(pBar);

    const int iSel = pBar->m_iHighlighted;
    int iPos = -1;                                    // POSITION of the highlighted item (-1 == NULL)
    CMFCToolBarButton* pSelButton = nullptr;
    if (iSel >= 0 && iSel < nCount) {
        iPos = iSel;
        pSelButton = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, iSel);
    }
    const int iPosOrig = iPos;                        // rdx: the pos the drop-down-list head/tail tests use

    if (nChar == VK_TAB) {
        nChar = (::GetKeyState(VK_SHIFT) & 0x80) ? VK_UP : VK_DOWN;
    }

    int bResult = FALSE;
    int iIndex = iSel;
    int bNotify = FALSE;
    CMFCToolBarButton* pButton = nullptr;

    switch (nChar) {
    case VK_HOME:
    case VK_DOWN: {
        if (At<int>(pThis, kOffDropDownListMode) != 0 && iPosOrig == nCount - 1 && nChar != VK_HOME) return TRUE;
        bResult = TRUE;
        if (nCount == 0) return bResult;
        const int iStart = (nChar == VK_HOME) ? -1 : iPos;
        const int iNext = (iStart != -1) ? NextIdx(iStart, nCount) : -1;
        int i = (iNext != -1) ? iNext : 0;
        iIndex = (iNext != -1) ? iSel + 1 : 0;
        if (i == iStart) return bResult;
        CMFCToolBarButton* pLast = nullptr;
        int nWraps = 0;
        for (;;) {
            CMFCToolBarButton* p = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, i);
            if (p == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return FALSE; }
            pLast = p;
            const int iNextNode = NextIdx(i, nCount);
            if (IsNavigableItem(p)) { pButton = p; break; }
            if (iNextNode == -1) {
                if (At<int>(pThis, kOffDropDownListMode) != 0) return TRUE;
                if (++nWraps > 1) return bResult;     // DEVIATION: retail loops for ever here
                i = 0;
                iIndex = 0;
            } else {
                i = iNextNode;
                ++iIndex;
            }
            if (i == iStart) { pButton = pLast; break; }
        }
        bNotify = TRUE;
        break;
    }
    case VK_END:
    case VK_UP: {
        if (At<int>(pThis, kOffDropDownListMode) != 0 && iPosOrig == (nCount > 0 ? 0 : -1) && nChar != VK_END) return TRUE;
        bResult = TRUE;
        if (nCount == 0) return bResult;
        const int iStart = (nChar == VK_END) ? -1 : iPos;
        const int iPrev = (iStart != -1) ? PrevIdx(iStart) : -1;
        int i;
        if (iPrev == -1) { i = nCount - 1; iIndex = nCount; }
        else             { i = iPrev;      iIndex = iSel;   }
        --iIndex;
        if (i == iStart) return bResult;
        CMFCToolBarButton* pLast = nullptr;
        int nWraps = 0;
        for (;;) {
            CMFCToolBarButton* p = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, i);
            if (p == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return FALSE; }
            pLast = p;
            const int iPrevNode = PrevIdx(i);
            if (IsNavigableItem(p)) { pButton = p; break; }
            if (iPrevNode == -1) {
                if (At<int>(pThis, kOffDropDownListMode) != 0) return TRUE;
                if (++nWraps > 1) return bResult;     // DEVIATION: retail loops for ever here
                i = nCount - 1;
                iIndex = nCount;
            } else {
                i = iPrevNode;
            }
            --iIndex;
            if (i == iStart) { pButton = pLast; break; }
        }
        bNotify = TRUE;
        break;
    }
    case VK_PRIOR:
    case VK_NEXT: {
        if (At<int>(pThis, kOffDropDownListMode) == 0) return FALSE;
        const int nPageSize = At<int>(pThis, kOffDropDownPageSize);
        if (nPageSize <= 0) return FALSE;
        const int iOld = iSel;
        At<int>(pThis, kOffInScrollMode) = TRUE;
        const unsigned int nKey = (nChar == VK_PRIOR) ? VK_UP : VK_DOWN;
        for (int k = 0; k < nPageSize; ++k) {
            impl__OnKey_CMFCPopupMenuBar__MEAAHI_Z(pThis, nKey);                 // vslot 0x810
        }
        At<int>(pThis, kOffInScrollMode) = FALSE;
        if (iOld != pBar->m_iHighlighted) {
            impl__AccNotifyObjectFocusEvent_CMFCToolBar__MEAAXH_Z(pBar, pBar->m_iHighlighted);   // vslot 0x4c8
        }
        return TRUE;
    }
    case VK_RETURN: {
        bResult = TRUE;
        if (!IsMenuButton(pSelButton)) return bResult;
        CMFCToolBarMenuButton* pMenuButton = static_cast<CMFCToolBarMenuButton*>(pSelButton);
        // HasButton() (vslot 0x1f0) folds to its FALSE base body, so OpenPopupMenu is always tried.
        if (impl__OpenPopupMenu_CMFCToolBarMenuButton__UEAAHPEAVCWnd___Z(pMenuButton, nullptr)) {   // vslot 0x1a8
            return bResult;
        }
        CWnd* pOwner = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(OwnerHwnd(pThis));
        if (pOwner != nullptr) {
            ::SendMessage(HWndOf(pOwner), kWmSetMessageString, kAfxIdsIdleMessage, 0);
        }
        impl__OnSendCommand_CMFCPopupMenuBar__MEAAHPEBVCMFCToolBarButton___Z(pThis, pMenuButton);   // vslot 0x7e0
        return bResult;
    }
    default: {
        if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0) return FALSE;
        if (::GetAsyncKeyState(VK_CONTROL) & 0x8000) return FALSE;
        unsigned int uiChar = nChar;
        bool bFound = false;
        if (impl__IsKeyPrintable_CKeyboardManager__SAHI_Z(nChar)) {
            uiChar = impl__TranslateCharToUpper_CKeyboardManager__SAII_Z(nChar);
            CMFCToolBarButton* pAccel = nullptr;
            if (AccelKeyLookup(static_cast<const char*>(static_cast<const void*>(pThis)) + kOffAccelKeys, uiChar, &pAccel)) {
                bFound = true;
                for (int i = 0; i < nCount; ++i) {
                    CMFCToolBarButton* p = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, i);
                    if (p == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return FALSE; }
                    if (p == pAccel) { iIndex = i; break; }
                }
                if (IsMenuButton(pAccel)) {
                    CMFCToolBarMenuButton* pMenuButton = static_cast<CMFCToolBarMenuButton*>(pAccel);
                    if (impl__OpenPopupMenu_CMFCToolBarMenuButton__UEAAHPEAVCWnd___Z(pMenuButton, nullptr)) {   // vslot 0x1a8
                        void* pPopup = At<void*>(pMenuButton, kOffMbPopupMenu);
                        if (pPopup != nullptr) {
                            ::SendMessage(HWndOf(pPopup), WM_KEYDOWN, VK_HOME, 0);
                        }
                    } else if (pMenuButton->m_nStyle & kTbbsDisabled) {
                        impl__InvokeMenuCommand_CMFCPopupMenuBar__IEAAXIPEBVCMFCToolBarButton___Z(pThis, 0, pMenuButton);
                        return TRUE;
                    } else {
                        bResult = impl__OnSendCommand_CMFCPopupMenuBar__MEAAHPEBVCMFCToolBarButton___Z(pThis, pMenuButton);   // vslot 0x7e0
                        if (bResult) return TRUE;
                    }
                }
                pButton = pAccel;
                bNotify = FALSE;
            }
        }
        if (!bFound) {
            if (impl__m_bRecentlyUsedMenus_CMFCMenuBar__1HA == 0) return FALSE;
            if (At<int>(pThis, kOffAreAllCommandsShown) != 0) return FALSE;
            // m_HiddenItemsAccel.Lookup(uiChar, uiCmd) -> InvokeMenuCommand(uiCmd, NULL): not performed (see DEVIATIONS).
            (void)uiChar;
            return FALSE;
        }
        if (pButton == nullptr) return bResult;       // retail would dereference NULL below (a NULL map value never occurs)
        break;
    }
    }

    // select:
    if (pButton == pSelButton) return bResult;
    if (bNotify && At<int>(pThis, kOffInScrollMode) == 0) {
        impl__AccNotifyObjectFocusEvent_CMFCToolBar__MEAAXH_Z(pBar, iIndex);          // vslot 0x4c8
    }
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA == 0) {
        pBar->m_iSelected = iIndex;
    }
    pBar->m_iHighlighted = iIndex;

    RECT rectClient = {};
    ::GetClientRect(hWnd, &rectClient);
    const RECT rectBtn = *static_cast<const RECT*>(pButton->m_rect);
    if (rectBtn.top >= rectClient.top && rectBtn.bottom <= rectClient.bottom) {
        if (pSelButton != nullptr) {
            ::InvalidateRect(hWnd, static_cast<const RECT*>(pSelButton->m_rect), TRUE);
        }
        ::InvalidateRect(hWnd, &rectBtn, TRUE);
        ::UpdateWindow(hWnd);
    }
    if (pButton->m_nID != static_cast<unsigned int>(-1)) {
        impl__ShowCommandMessageString_CMFCPopupMenuBar__MEAAXI_Z(pThis, pButton->m_nID);   // vslot 0x840
    }
    return bResult;
}

// Retail (RVA 0xbf810, mfc140u), fully transcribed:
//     int iHit = HitTest(point);                                                 // vslot 0x730
//     if (iHit >= 0) {
//         CMFCToolBarButton* p = GetButton(iHit);                                // 0x14fe00
//         if (p != NULL && p->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton)) &&
//             (p->m_nID == (UINT)-1 || p->IsDroppedDown())) {                    // +0x24, vslot 0xf0
//             CWnd::Default();  return;                                          // 0x28ac80: a double click on an open submenu is swallowed
//         }
//     }
//     CMFCToolBar::OnLButtonDblClk(nFlags, point);                              // 0x1560c0
// Symbol: ?OnLButtonDblClk@CMFCPopupMenuBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCPopupMenuBar__IEAAXIVCPoint___Z(CMFCPopupMenuBar* pThis, unsigned int nFlags, long long pointPacked)
{
    if (!pThis) return;
    CMFCToolBar* pBar = AsToolBar(pThis);
    const CPoint point = UnpackPoint(pointPacked);
    const int iHit = impl__HitTest_CMFCToolBar__UEAAHVCPoint___Z(pBar, point);
    if (iHit >= 0) {
        CMFCToolBarButton* p = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, iHit);
        if (IsMenuButton(p) &&
            (p->m_nID == static_cast<unsigned int>(-1) || At<void*>(p, kOffMbPopupMenu) != nullptr)) {
            impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
            return;
        }
    }
    impl__OnLButtonDblClk_CMFCToolBar__IEAAXIVCPoint___Z(pBar, nFlags, point);
}

// Retail (RVA 0xbe770, mfc140u), fully transcribed:
//     m_bFirstClick = FALSE;  m_bIsClickOutsideItem = TRUE;                      // +0x1378, +0x138c
//     CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);
//     if (!CMFCToolBar::m_bCustomizeMode && !rectClient.PtInRect(point)) {       // 0x3be35c
//         CMFCToolBar* pDest = FindDestintationToolBar(point);                   // 0xbec20 (point is client coords here;
//                                                                                 //   FindDestintationToolBar ScreenToClient's it again -- retail does exactly this)
//         if (pDest != NULL) {
//             CPoint ptDest = point;                                             // a COPY ([rsp+0x20], 0xbe7ff): the original
//             ::MapWindowPoints(m_hWnd, pDest->m_hWnd, &ptDest, 1);              //   in rbx is what the base call below receives
//             ::SendMessage(pDest->m_hWnd, WM_LBUTTONDOWN, nFlags, MAKELPARAM(ptDest.x, ptDest.y));
//         }
//     }
//     CMFCToolBar::OnLButtonDown(nFlags, point);                                // 0x150410, unmapped point (still a stub in CMFCToolBar.cpp)
// Symbol: ?OnLButtonDown@CMFCPopupMenuBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCPopupMenuBar__IEAAXIVCPoint___Z(CMFCPopupMenuBar* pThis, unsigned int nFlags, long long pointPacked)
{
    if (!pThis) return;
    const HWND hWnd = HWndOf(pThis);
    const CPoint point = UnpackPoint(pointPacked);
    At<int>(pThis, kOffFirstClick) = FALSE;
    At<int>(pThis, kOffIsClickOutsideItem) = TRUE;

    RECT rectClient = {};
    ::GetClientRect(hWnd, &rectClient);
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA == 0 && !::PtInRect(&rectClient, point)) {
        CMFCToolBar* pDest = impl__FindDestintationToolBar_CMFCPopupMenuBar__QEAAPEAVCMFCToolBar__VCPoint___Z(pThis, pointPacked);
        if (pDest != nullptr) {
            POINT pt = point;
            ::MapWindowPoints(hWnd, HWndOf(pDest), &pt, 1);
            ::SendMessage(HWndOf(pDest), WM_LBUTTONDOWN, nFlags, MAKELPARAM(static_cast<WORD>(pt.x), static_cast<WORD>(pt.y)));
        }
    }
    impl__OnLButtonDown_CMFCToolBar__IEAAXIVCPoint___Z(AsToolBar(pThis), nFlags, point);
}

// Retail (RVA 0xbe860, mfc140u), fully transcribed:
//     CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);
//     if (!m_bFirstClick && !CMFCToolBar::m_bCustomizeMode && !rectClient.PtInRect(point)) {   // +0x1378, 0x3be35c
//         CMFCToolBar* pDest = FindDestintationToolBar(point);                   // 0xbec20
//         if (pDest != NULL) {
//             ::MapWindowPoints(m_hWnd, pDest->m_hWnd, &point, 1);
//             ::SendMessage(pDest->m_hWnd, WM_LBUTTONUP, nFlags, MAKELPARAM(point.x, point.y));
//         }
//         AFXGetParentFrame(this)->DestroyWindow();                              // 0x6bd00, vslot 0xd0 (slot 26 = ?DestroyWindow@CWnd@@); unchecked in retail
//         return;
//     }
//     if (!CMFCToolBar::m_bCustomizeMode && m_iHighlighted >= 0) m_iButtonCapture = m_iHighlighted;   // +0x1138, +0x1134
//     BOOL bClickOutside = m_bIsClickOutsideItem;  m_bFirstClick = FALSE;         // +0x138c
//     if (bClickOutside) CMFCToolBar::OnLButtonUp(nFlags, point);               // 0x151260 (still a stub in CMFCToolBar.cpp)
// Symbol: ?OnLButtonUp@CMFCPopupMenuBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCPopupMenuBar__IEAAXIVCPoint___Z(CMFCPopupMenuBar* pThis, unsigned int nFlags, long long pointPacked)
{
    if (!pThis) return;
    const HWND hWnd = HWndOf(pThis);
    CMFCToolBar* pBar = AsToolBar(pThis);
    const CPoint point = UnpackPoint(pointPacked);

    RECT rectClient = {};
    ::GetClientRect(hWnd, &rectClient);
    if (At<int>(pThis, kOffFirstClick) == 0 && impl__m_bCustomizeMode_CMFCToolBar__1HA == 0 && !::PtInRect(&rectClient, point)) {
        CMFCToolBar* pDest = impl__FindDestintationToolBar_CMFCPopupMenuBar__QEAAPEAVCMFCToolBar__VCPoint___Z(pThis, pointPacked);
        if (pDest != nullptr) {
            POINT pt = point;
            ::MapWindowPoints(hWnd, HWndOf(pDest), &pt, 1);
            ::SendMessage(HWndOf(pDest), WM_LBUTTONUP, nFlags, MAKELPARAM(static_cast<WORD>(pt.x), static_cast<WORD>(pt.y)));
        }
        CFrameWnd* pFrame = impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(static_cast<const CWnd*>(pThis));
        if (pFrame != nullptr) {
            impl__DestroyWindow_CWnd__UEAAHXZ(pFrame);                                 // vslot 0xd0
        }
        return;
    }

    if (impl__m_bCustomizeMode_CMFCToolBar__1HA == 0 && pBar->m_iHighlighted >= 0) {
        pBar->m_iButtonCapture = pBar->m_iHighlighted;
    }
    const int bClickOutside = At<int>(pThis, kOffIsClickOutsideItem);
    At<int>(pThis, kOffFirstClick) = FALSE;
    if (bClickOutside) {
        impl__OnLButtonUp_CMFCToolBar__IEAAXIVCPoint___Z(pBar, nFlags, point);
    }
}

// Retail (RVA 0xbeaf0, mfc140u), fully transcribed:
//     if (m_bFirstMove) { m_bFirstMove = FALSE; return; }                        // +0x137c: the WM_MOUSEMOVE the window gets on creation
//     if (m_ptCursor != CPoint(-1, -1)) {                                        // +0x13a8
//         CPoint ptCursor; ::GetCursorPos(&ptCursor);
//         if (ptCursor == m_ptCursor) return;                                    // the mouse did not really move
//         m_ptCursor = ptCursor;
//     }
//     CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);
//     if (!CMFCToolBar::m_bCustomizeMode && !rectClient.PtInRect(point)) {       // 0x3be35c
//         CMFCToolBar* pDest = FindDestintationToolBar(point);                   // 0xbec20
//         if (pDest != NULL) {
//             ::MapWindowPoints(m_hWnd, pDest->m_hWnd, &point, 1);
//             ::SendMessage(pDest->m_hWnd, WM_MOUSEMOVE, nFlags, MAKELPARAM(point.x, point.y));
//         }
//         return;
//     }
//     CMFCToolBar::OnMouseMove(nFlags, point);                                  // 0x150ce0 (still a stub in CMFCToolBar.cpp)
// Symbol: ?OnMouseMove@CMFCPopupMenuBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCPopupMenuBar__IEAAXIVCPoint___Z(CMFCPopupMenuBar* pThis, unsigned int nFlags, long long pointPacked)
{
    if (!pThis) return;
    if (At<int>(pThis, kOffFirstMove) != 0) {
        At<int>(pThis, kOffFirstMove) = FALSE;
        return;
    }
    POINT& ptLast = At<POINT>(pThis, kOffPtCursor);
    if (ptLast.x != -1 || ptLast.y != -1) {
        POINT ptCursor = {};
        ::GetCursorPos(&ptCursor);
        if (ptCursor.x == ptLast.x && ptCursor.y == ptLast.y) return;
        ptLast = ptCursor;
    }

    const HWND hWnd = HWndOf(pThis);
    const CPoint point = UnpackPoint(pointPacked);
    RECT rectClient = {};
    ::GetClientRect(hWnd, &rectClient);
    if (impl__m_bCustomizeMode_CMFCToolBar__1HA == 0 && !::PtInRect(&rectClient, point)) {
        CMFCToolBar* pDest = impl__FindDestintationToolBar_CMFCPopupMenuBar__QEAAPEAVCMFCToolBar__VCPoint___Z(pThis, pointPacked);
        if (pDest != nullptr) {
            POINT pt = point;
            ::MapWindowPoints(hWnd, HWndOf(pDest), &pt, 1);
            ::SendMessage(HWndOf(pDest), WM_MOUSEMOVE, nFlags, MAKELPARAM(static_cast<WORD>(pt.x), static_cast<WORD>(pt.y)));
        }
        return;
    }
    impl__OnMouseMove_CMFCToolBar__IEAAXIVCPoint___Z(AsToolBar(pThis), nFlags, point);
}

// Retail: ordinal 10570 -> export address table -> RVA 0x27d0 (mfc140u), the
// bytes `c2 00 00` (`ret`) between int3 padding: the one-instruction body that
// COMDAT folding gives every empty function (158 mfc140u exports land on it,
// and the by-RVA symbol map labels it with the first of them,
// ?AddDockSite@CFrameWndEx@@, which is why `disas.py` by NAME reports this
// export as NOT FOUND -- resolve it with ures.py / the export table instead).
// A popup menu bar has no non-client area to lay out, so the base
// CMFCToolBar::OnNcCalcSize (m_Impl.CalcNcSize) is deliberately NOT called.
// The empty body IS the retail behaviour.
// Symbol: ?OnNcCalcSize@CMFCPopupMenuBar@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CMFCPopupMenuBar__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(
    CMFCPopupMenuBar* pThis, int bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
    (void)pThis; (void)bCalcValidRects; (void)lpncsp;
}

// Retail: ordinal 10641 -> export address table -> the same folded `ret` at
// RVA 0x27d0 (mfc140u) as OnNcCalcSize above (same NOT-FOUND-by-name caveat).
// The base CMFCToolBar::OnNcPaint (m_Impl.DrawNcArea) is deliberately NOT
// called.  The empty body IS the retail behaviour -- do not "implement" this
// because a stub counter flags a body with no statements.
// Symbol: ?OnNcPaint@CMFCPopupMenuBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_CMFCPopupMenuBar__IEAAXXZ(CMFCPopupMenuBar* pThis)
{
    (void)pThis;
}

// Retail (RVA 0xbc160, mfc140u), transcribed (deviations after the body):
//     if (pButton->m_nID == (UINT)-2) return TRUE;                              // +0x24: the "(Empty)" placeholder item
//     if (pButton->IsKindOf(RUNTIME_CLASS(CMFCCustomizeMenuButton))) {          // 0x2e3ab0
//         if (pButton->m_nStyle & TBBS_DISABLED) {                               // +0x28 & 0x40000
//             ((CMFCToolBarMenuButton*)pButton)->OnClickMenuItem();              // vslot 0x1e0 (slot 60 of 0x3184e8; base = FALSE)
//             return TRUE;
//         }
//     } else if (pButton->m_nStyle & TBBS_DISABLED) return FALSE;
//     if (pButton->m_nID == (UINT)-1) return FALSE;
//     if (pButton->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton))) {            // 0x3b1748
//         CMFCToolBarMenuButton* pMenuButton = (CMFCToolBarMenuButton*)pButton;
//         if (pMenuButton->HasButton()) {                                        // vslot 0x1f0 (slot 62; base = FALSE)
//             CPoint pt; ::GetCursorPos(&pt); ::ScreenToClient(m_hWnd, &pt);
//             if (pMenuButton->m_rectButton.PtInRect(pt)) return TRUE;           // +0x118
//             if (pMenuButton->m_pPopupMenu != NULL) {                           // +0xc0
//                 ::PostMessage(pMenuButton->m_pPopupMenu->m_hWnd, WM_CLOSE, 0, 0);
//                 return FALSE;
//             }
//             // m_pPopupMenu == NULL: 0xbc241 jumps to 0xbc25a, the common path below
//         }
//         if (pMenuButton->m_pPopupMenu != NULL) return FALSE;
//         if (pMenuButton->OnClickMenuItem()) return TRUE;                       // vslot 0x1e0
//         if (pMenuButton->IsKindOf(RUNTIME_CLASS(CMFCShowAllButton))) {         // 0x30f268
//             pMenuButton->OnClick(this, FALSE);  return TRUE;                   // vslot 0x50
//         }
//     }
//     InvokeMenuCommand(pButton->m_nID, pButton);                                // 0xbc2d0
//     return TRUE;
// DEVIATIONS: OnClickMenuItem / HasButton are CMFCToolBarMenuButton virtuals
// whose base bodies both return FALSE (0x71e0, `xor eax,eax`); OpenMFC cannot
// dispatch a client override, so they are folded to FALSE -- the split-button
// (HasButton) branch and the customized-item branch are therefore never taken.
// OnClick goes through the exported thunk (a stub today).
// Symbol: ?OnSendCommand@CMFCPopupMenuBar@@MEAAHPEBVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__OnSendCommand_CMFCPopupMenuBar__MEAAHPEBVCMFCToolBarButton___Z(
    CMFCPopupMenuBar* pThis, const CMFCToolBarButton* pButton)
{
    if (!pThis || !pButton) return FALSE;
    if (pButton->m_nID == static_cast<unsigned int>(-2)) return TRUE;

    if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pButton, impl__GetThisClass_CMFCCustomizeMenuButton__SAPEAUCRuntimeClass__XZ())) {
        if (pButton->m_nStyle & kTbbsDisabled) {
            // pMenuButton->OnClickMenuItem(): vslot 0x1e0, base body returns FALSE (result dropped here).
            return TRUE;
        }
    } else if (pButton->m_nStyle & kTbbsDisabled) {
        return FALSE;
    }
    if (pButton->m_nID == static_cast<unsigned int>(-1)) return FALSE;

    if (IsMenuButton(pButton)) {
        CMFCToolBarMenuButton* pMenuButton = const_cast<CMFCToolBarMenuButton*>(static_cast<const CMFCToolBarMenuButton*>(pButton));
        // HasButton() (vslot 0x1f0) folds to its FALSE base body: the split-button branch is not reachable here.
        if (At<void*>(pMenuButton, kOffMbPopupMenu) != nullptr) return FALSE;
        // OnClickMenuItem() (vslot 0x1e0) folds to its FALSE base body.
        if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pMenuButton, impl__GetThisClass_CMFCShowAllButton__SAPEAUCRuntimeClass__XZ())) {
            impl__OnClick_CMFCToolBarMenuButton__UEAAHPEAVCWnd__H_Z(pMenuButton, AsWnd(pThis), FALSE);   // vslot 0x50
            return TRUE;
        }
    }

    impl__InvokeMenuCommand_CMFCPopupMenuBar__IEAAXIPEBVCMFCToolBarButton___Z(pThis, pButton->m_nID, pButton);
    return TRUE;
}

// Retail (RVA 0xbe990, mfc140u), fully transcribed:
//     CMFCPopupMenu* pParentMenu = DYNAMIC_DOWNCAST(CMFCPopupMenu, CWnd::FromHandle(::GetParent(m_hWnd)));
//     if (pParentMenu != NULL) {
//         CMFCToolBar* pToolBar = pParentMenu->GetParentToolBar();               // 0xb75f0
//         if (pToolBar != NULL && pToolBar->OnSetDefaultButtonText(pButton))     // vslot 0x750 (slot 234)
//             return TRUE;
//     }
//     return CMFCToolBar::OnSetDefaultButtonText(pButton);                       // 0x1572f0
// The parent toolbar's virtual is reached through the exported CMFCToolBar
// thunk (DEVIATION for a client toolbar override).
// Symbol: ?OnSetDefaultButtonText@CMFCPopupMenuBar@@MEAAHPEAVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__OnSetDefaultButtonText_CMFCPopupMenuBar__MEAAHPEAVCMFCToolBarButton___Z(
    CMFCPopupMenuBar* pThis, CMFCToolBarButton* pButton)
{
    if (!pThis) return FALSE;
    CMFCPopupMenu* pParentMenu = ParentPopupMenu(pThis);
    if (pParentMenu != nullptr) {
        CMFCToolBar* pToolBar = impl__GetParentToolBar_CMFCPopupMenu__QEBAPEAVCMFCToolBar__XZ(pParentMenu);
        if (pToolBar != nullptr &&
            impl__OnSetDefaultButtonText_CMFCToolBar__UEAAHPEAVCMFCToolBarButton___Z(pToolBar, pButton)) {   // vslot 0x750
            return TRUE;
        }
    }
    return impl__OnSetDefaultButtonText_CMFCToolBar__UEAAHPEAVCMFCToolBarButton___Z(AsToolBar(pThis), pButton);
}

// Retail (RVA 0xbe480, mfc140u), fully transcribed:
//     CPoint pt; ::GetCursorPos(&pt); ::ScreenToClient(m_hWnd, &pt);
//     if (nIDEvent == 0xEC17) {                                                  // delayed submenu open
//         ::KillTimer(m_hWnd, 0xEC17);
//         if (m_pToolTip != NULL && m_pToolTip->m_hWnd != NULL) m_pToolTip->ShowWindow(SW_HIDE);   // +0x12e8, 0x2a9ad0
//         if (m_pDelayedClosePopupMenuButton != NULL &&                          // +0x13e0
//             m_pDelayedClosePopupMenuButton->m_rect.PtInRect(pt)) return;       // +0x68
//         CloseDelayedSubMenu();                                                 // vslot 0x890 (slot 274)
//         CMFCToolBarMenuButton* pBtn = m_pDelayedPopupMenuButton;               // +0x13d8
//         m_pDelayedPopupMenuButton = NULL;
//         if (pBtn == NULL) return;
//         if (m_iHighlighted < 0 || m_iHighlighted >= m_Buttons.GetCount() ||     // +0x1138, +0x11a0
//             GetButton(m_iHighlighted) != pBtn) return;                         // 0x14fe00
//         pBtn->OpenPopupMenu(this);                                             // vslot 0x1a8 (slot 53 of 0x3184e8)
//     } else if (nIDEvent == 0xEC18) {                                           // delayed submenu close
//         ::KillTimer(m_hWnd, 0xEC18);
//         if (m_pDelayedClosePopupMenuButton == NULL) return;
//         CMFCPopupMenu* pParentMenu = DYNAMIC_DOWNCAST(CMFCPopupMenu, CWnd::FromHandle(::GetParent(m_hWnd)));
//         if (m_pDelayedClosePopupMenuButton->m_rect.PtInRect(pt)) return;
//         m_pDelayedClosePopupMenuButton->OnCancelMode();                        // vslot 0xc0 (slot 24)
//         m_pDelayedClosePopupMenuButton = NULL;
//         if (pParentMenu != NULL)
//             CMFCPopupMenu::ActivatePopupMenu(g_pTopLevelFrame ? g_pTopLevelFrame : GetTopLevelFrame(), pParentMenu);   // 0x3be1b8, 0x28e490, 0xb7920
//     } else if (nIDEvent == 0xEC07) {                                           // accessibility hot item
//         ::KillTimer(m_hWnd, 0xEC07);
//         CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);
//         if (!rectClient.PtInRect(pt)) return;
//         int iHit = HitTest(pt);                                                // vslot 0x730
//         if (iHit == m_iAccHotItem && iHit != -1) AccNotifyObjectFocusEvent(iHit);   // +0x1154, vslot 0x4c8 (slot 153)
//     }
// (Any other timer id falls through: the base is NOT called.)  The virtuals go
// through the exported thunks / this file's CloseDelayedSubMenu (DEVIATION for
// client overrides); the GetCount() range test reads the side table.
// Symbol: ?OnTimer@CMFCPopupMenuBar@@IEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CMFCPopupMenuBar__IEAAX_K_Z(CMFCPopupMenuBar* pThis, unsigned __int64 nIDEvent)
{
    if (!pThis) return;
    const HWND hWnd = HWndOf(pThis);
    CMFCToolBar* pBar = AsToolBar(pThis);

    POINT pt = {};
    ::GetCursorPos(&pt);
    ::ScreenToClient(hWnd, &pt);

    if (nIDEvent == kTimerDelayedPopup) {
        ::KillTimer(hWnd, kTimerDelayedPopup);
        if (pBar->m_pToolTip != nullptr && HWndOf(pBar->m_pToolTip) != nullptr) {
            impl__ShowWindow_CWnd__QEAAHH_Z(reinterpret_cast<CWnd*>(pBar->m_pToolTip), SW_HIDE);   // CMFCToolTipCtrl is incomplete in afxmfc.h
        }
        CMFCToolBarMenuButton* pClose = At<CMFCToolBarMenuButton*>(pThis, kOffDelayedClosePopupMenuButton);
        if (pClose != nullptr && ::PtInRect(static_cast<const RECT*>(pClose->m_rect), pt)) return;

        impl__CloseDelayedSubMenu_CMFCPopupMenuBar__UEAAXXZ(pThis);                   // vslot 0x890
        CMFCToolBarMenuButton* pBtn = At<CMFCToolBarMenuButton*>(pThis, kOffDelayedPopupMenuButton);
        At<CMFCToolBarMenuButton*>(pThis, kOffDelayedPopupMenuButton) = nullptr;
        if (pBtn == nullptr) return;
        const int iHighlighted = pBar->m_iHighlighted;
        if (iHighlighted < 0 || iHighlighted >= impl__GetCount_CMFCToolBar__QEBAHXZ(pBar)) return;
        if (impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, iHighlighted) != pBtn) return;
        impl__OpenPopupMenu_CMFCToolBarMenuButton__UEAAHPEAVCWnd___Z(pBtn, AsWnd(pThis));   // vslot 0x1a8
    } else if (nIDEvent == kTimerDelayedClose) {
        ::KillTimer(hWnd, kTimerDelayedClose);
        CMFCToolBarMenuButton* pClose = At<CMFCToolBarMenuButton*>(pThis, kOffDelayedClosePopupMenuButton);
        if (pClose == nullptr) return;
        CMFCPopupMenu* pParentMenu = ParentPopupMenu(pThis);
        if (::PtInRect(static_cast<const RECT*>(pClose->m_rect), pt)) return;
        impl__OnCancelMode_CMFCToolBarMenuButton__UEAAXXZ(pClose);                     // vslot 0xc0
        At<CMFCToolBarMenuButton*>(pThis, kOffDelayedClosePopupMenuButton) = nullptr;
        if (pParentMenu != nullptr) {
            CFrameWnd* pFrame = static_cast<CFrameWnd*>(impl__g_pTopLevelFrame__3PEAVCFrameWnd__EA);
            if (pFrame == nullptr) pFrame = impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(static_cast<const CWnd*>(pThis));
            impl__ActivatePopupMenu_CMFCPopupMenu__SAHPEAVCFrameWnd__PEAV1__Z(pFrame, pParentMenu);
        }
    } else if (nIDEvent == kTimerAccHot) {
        ::KillTimer(hWnd, kTimerAccHot);
        RECT rectClient = {};
        ::GetClientRect(hWnd, &rectClient);
        if (!::PtInRect(&rectClient, pt)) return;
        const int iHit = impl__HitTest_CMFCToolBar__UEAAHVCPoint___Z(pBar, CPoint(pt));    // vslot 0x730
        if (iHit == pBar->m_iAccHotItem && iHit != -1) {
            impl__AccNotifyObjectFocusEvent_CMFCToolBar__MEAAXH_Z(pBar, iHit);              // vslot 0x4c8
        }
    }
}

// Retail (RVA 0xbf670, mfc140u), fully transcribed:
//     CMFCToolBarButton* pButton = GetButton(m_iSelected);  if (!pButton) AfxThrowInvalidArgException();   // +0x113c
//     if (pButton->GetImage() < 0) OnToolbarAppearance();                       // 0x154100
//     else afxCommandManager->EnableMenuItemImage(pButton->m_nID, TRUE,         // 0x3c1470 (GetCmdMgr()), 0x2f060
//                                                 pButton->m_bUserButton ? pButton->GetImage() : -1);
//     AdjustLayout();                                                            // vslot 0x428, tail jump
// EnableMenuItemImage is still a stub in featurepack/customize/CCommandManager.cpp
// (its definition carries an auto-generated parameter list; the declaration
// used here is the one derived from the mangled name).
// Symbol: ?OnToolbarImageAndText@CMFCPopupMenuBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolbarImageAndText_CMFCPopupMenuBar__IEAAXXZ(CMFCPopupMenuBar* pThis)
{
    if (!pThis) return;
    CMFCToolBar* pBar = AsToolBar(pThis);
    CMFCToolBarButton* pButton = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, pBar->m_iSelected);
    if (pButton == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }

    const int iImage = pButton->GetImage();
    if (iImage < 0) {
        impl__OnToolbarAppearance_CMFCToolBar__IEAAXXZ(pBar);
    } else {
        impl__EnableMenuItemImage_CCommandManager__QEAAXIHH_Z(
            impl__GetCmdMgr__YAPEAVCCommandManager__XZ(), pButton->m_nID, TRUE, pButton->m_bUserButton ? iImage : -1);
    }
    impl__AdjustLayout_CMFCPopupMenuBar__MEAAXXZ(pThis);
}

// Retail (RVA 0xbf6f0, mfc140u), fully transcribed:
//     CMFCToolBarButton* pButton = GetButton(m_iSelected);  if (!pButton) AfxThrowInvalidArgException();
//     afxCommandManager->EnableMenuItemImage(pButton->m_nID, FALSE, -1);        // inlined: 0x11f1c Find on m_lstCommandsWithoutImages (0x3c1520),
//                                                                                //   0x2f418 RemoveKey on m_mapMenuUserImages (0x3c1558),
//                                                                                //   0x12284 AddTail if not already listed
//     AdjustLayout();                                                            // vslot 0x428, tail jump
// The inlined body is byte-for-byte the bEnable == FALSE path of
// ?EnableMenuItemImage@CCommandManager@@QEAAXIHH@Z (0x2f060), so it is called
// as such (a stub in OpenMFC today, see OnToolbarImageAndText).
// Symbol: ?OnToolbarText@CMFCPopupMenuBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnToolbarText_CMFCPopupMenuBar__IEAAXXZ(CMFCPopupMenuBar* pThis)
{
    if (!pThis) return;
    CMFCToolBar* pBar = AsToolBar(pThis);
    CMFCToolBarButton* pButton = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, pBar->m_iSelected);
    if (pButton == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }

    impl__EnableMenuItemImage_CCommandManager__QEAAXIHH_Z(impl__GetCmdMgr__YAPEAVCCommandManager__XZ(), pButton->m_nID, FALSE, -1);
    impl__AdjustLayout_CMFCPopupMenuBar__MEAAXXZ(pThis);
}

// Retail (RVA 0xbef60, mfc140u), fully transcribed:
//     if (m_bPaletteMode) return CMFCToolBar::OnToolHitTest(point, pTI);         // +0x1390, 0x14ff80
//     int iHit = HitTest(point);  if (iHit == -1) return -1;                     // vslot 0x730
//     CMFCToolBarButton* pButton = GetButton(iHit);                              // 0x14fe00
//     if (pButton == NULL || !pButton->IsKindOf(RUNTIME_CLASS(CMFCToolBarButton))) return iHit;   // 0x3b1628
//         // NB: both tests (0xbefb4, 0xbefcb) jump to the `movslq %ebp` epilogue with
//         // ebp still holding iHit -- retail returns the HIT INDEX here, not -1.
//     if (pTI != NULL) { pTI->uId = pButton->m_nID; pTI->hwnd = m_hWnd; pTI->rect = pButton->m_rect; }
//     if (!pButton->OnToolHitTest(this, pTI)) return (int)pButton->m_nID;        // button vslot 0x108 (slot 33 of 0x3184e8 = ?OnToolHitTest@CMFCToolBarButton@@); movslq -> sign-extended
//     if (pTI != NULL && pTI->lpszText != NULL) {
//         CString strTipText;  strTipText.SetString(pTI->lpszText, wcslen(pTI->lpszText));   // 0x2e30
//         free(pTI->lpszText);                                                   // 0x2c74e8
//         CString strDescr;
//         CFrameWnd* pFrame = GetParentFrame();                                  // 0x28e200
//         if (pFrame != NULL && pFrame->m_hWnd != NULL && !pButton->IsKindOf(RUNTIME_CLASS(CMFCShowAllButton)))   // 0x30f268
//             pFrame->GetMessageString(pButton->m_nID, strDescr);                // frame vslot 0x2f8 (slot 95 = ?GetMessageString@CFrameWnd@@)
//         CTooltipManager::SetTooltipText(pTI, m_pToolTip, AFX_TOOLTIP_TYPE_TOOLBAR (2), strTipText, strDescr);   // +0x12e8, 0x182560
//     }
//     return iHit;
// DEVIATIONS: the button's OnToolHitTest is reached through the exported
// CMFCToolBarButton thunk, non-virtually -- and that thunk is still a
// `return 0` stub (featurepack/toolbar/CMFCToolBarButton.cpp, auto-generated
// parameter list), so today the text block is never entered and the button id
// is returned, exactly as retail does for a button that declines the tooltip.
// The frame's GetMessageString goes through the core/frame/CFrameWnd.cpp
// thunk, which is a real body: it empties the string and dispatches to the C++
// virtual, so an OpenMFC-built frame's override IS reached (a client-built
// frame's slot-95 override is not).  pTI->lpszText is freed with free() as
// retail does (the button allocated it with the CRT allocator).
// Symbol: ?OnToolHitTest@CMFCPopupMenuBar@@MEBA_JVCPoint@@PEAUtagTOOLINFOW@@@Z
extern "C" __int64 MS_ABI impl__OnToolHitTest_CMFCPopupMenuBar__MEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
    const CMFCPopupMenuBar* pThis, long long pointPacked, TOOLINFOW* pTI)
{
    if (!pThis) return -1;
    const CMFCToolBar* pBar = static_cast<const CMFCToolBar*>(pThis);
    const CPoint point = UnpackPoint(pointPacked);
    if (At<int>(pThis, kOffPaletteMode) != 0) {
        return impl__OnToolHitTest_CMFCToolBar__MEBA_JVCPoint__PEAUtagTOOLINFOW___Z(pBar, point, pTI);
    }

    const int iHit = impl__HitTest_CMFCToolBar__UEAAHVCPoint___Z(const_cast<CMFCToolBar*>(pBar), point);
    if (iHit == -1) return -1;
    CMFCToolBarButton* pButton = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, iHit);
    if (pButton == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pButton, impl__GetThisClass_CMFCToolBarButton__SAPEAUCRuntimeClass__XZ())) {
        return iHit;   // retail: the epilogue's ebp is still iHit (see above)
    }
    if (pTI != nullptr) {
        pTI->uId  = pButton->m_nID;
        pTI->hwnd = HWndOf(pThis);
        pTI->rect.left   = pButton->m_rect.left;
        pTI->rect.top    = pButton->m_rect.top;
        pTI->rect.right  = pButton->m_rect.right;
        pTI->rect.bottom = pButton->m_rect.bottom;
    }
    if (!impl__OnToolHitTest_CMFCToolBarButton__UEAAHPEBVCWnd__PEAUtagTOOLINFOW___Z(pButton, static_cast<const CWnd*>(pThis), pTI)) {   // vslot 0x108
        // `mov 0x24(%rdi),%ebp` then the shared `movslq %ebp,%rax` epilogue: the
        // UINT id is SIGN-extended, so a (UINT)-1 submenu item yields -1 ("no tool").
        return static_cast<__int64>(static_cast<int>(pButton->m_nID));
    }
    if (pTI != nullptr && pTI->lpszText != nullptr) {
        CString strTipText(pTI->lpszText);
        ::free(pTI->lpszText);
        CString strDescr;
        CFrameWnd* pFrame = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(static_cast<const CWnd*>(pThis));
        if (pFrame != nullptr && HWndOf(pFrame) != nullptr &&
            !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pButton, impl__GetThisClass_CMFCShowAllButton__SAPEAUCRuntimeClass__XZ())) {
            impl__GetMessageString_CFrameWnd__UEBAXIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
                pFrame, pButton->m_nID, &strDescr);                                        // frame vslot 0x2f8
        }
        impl__SetTooltipText_CTooltipManager__SAXPEAUtagTOOLINFOW__PEAVCToolTipCtrl__IV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(
            pTI, reinterpret_cast<CToolTipCtrl*>(pBar->m_pToolTip), 2 /*AFX_TOOLTIP_TYPE_TOOLBAR*/, &strTipText, static_cast<const wchar_t*>(strDescr));
    }
    return iHit;
}

// Retail (RVA 0xbf550, mfc140u), fully transcribed:
//     if (m_pDelayedClosePopupMenuButton == NULL) return;                         // +0x13e0
//     m_pDelayedClosePopupMenuButton->m_bToBeClosed = FALSE;                      // +0xdc
//     int iOldHighlighted = m_iHighlighted;                                       // +0x1138
//     SetHot(m_pDelayedClosePopupMenuButton);                                     // 0x157d80 (non-virtual)
//     m_iHighlighted = m_iHot;                                                    // +0x1140
//     m_pDelayedClosePopupMenuButton = NULL;
//     if (iOldHighlighted != m_iHighlighted) {
//         if (iOldHighlighted >= 0) InvalidateButton(iOldHighlighted);            // 0x14fe50
//         InvalidateButton(m_iHighlighted);
//         ::UpdateWindow(m_hWnd);
//     }
//     ::KillTimer(m_hWnd, 0xEC18);
// SetHot is still a stub in featurepack/toolbar/CMFCToolBar.cpp (its
// definition carries an auto-generated parameter list; the declaration used
// here is the one derived from the mangled name), so m_iHot is not moved yet.
// Symbol: ?RestoreDelayedSubMenu@CMFCPopupMenuBar@@UEAAXXZ
extern "C" void MS_ABI impl__RestoreDelayedSubMenu_CMFCPopupMenuBar__UEAAXXZ(CMFCPopupMenuBar* pThis)
{
    if (!pThis) return;
    CMFCToolBarMenuButton* pClose = At<CMFCToolBarMenuButton*>(pThis, kOffDelayedClosePopupMenuButton);
    if (pClose == nullptr) return;
    CMFCToolBar* pBar = AsToolBar(pThis);

    At<int>(pClose, kOffMbToBeClosed) = FALSE;
    const int iOldHighlighted = pBar->m_iHighlighted;
    impl__SetHot_CMFCToolBar__QEAAHPEAVCMFCToolBarButton___Z(pBar, pClose);
    pBar->m_iHighlighted = pBar->m_iHot;
    At<CMFCToolBarMenuButton*>(pThis, kOffDelayedClosePopupMenuButton) = nullptr;
    if (iOldHighlighted != pBar->m_iHighlighted) {
        if (iOldHighlighted >= 0) impl__InvalidateButton_CMFCToolBar__QEAAPEAVCMFCToolBarButton__H_Z(pBar, iOldHighlighted);
        impl__InvalidateButton_CMFCToolBar__QEAAPEAVCMFCToolBarButton__H_Z(pBar, pBar->m_iHighlighted);
        ::UpdateWindow(HWndOf(pThis));
    }
    ::KillTimer(HWndOf(pThis), kTimerDelayedClose);
}

// Retail (RVA 0xbf2c0, mfc140u), fully transcribed:
//     CMFCToolBarButton* pButton = GetButton(nIndex);  if (!pButton) AfxThrowInvalidArgException();   // 0x14fe00
//     UINT nOldStyle = pButton->m_nStyle;                                        // +0x28
//     if (nOldStyle == nStyle) return;
//     pButton->m_nStyle = nStyle;
//     if ((nOldStyle & nStyle) & TBBS_PRESSED) return;                            // 0x20000 set in both: nothing to repaint
//     CMFCToolBarMenuButton* pMenuButton = DYNAMIC_DOWNCAST(CMFCToolBarMenuButton, GetButton(nIndex));   // 0x3b1748
//     if (pMenuButton != NULL && ((nOldStyle ^ nStyle) & TBBS_CHECKED)) {         // bt $0x10 = 0x10000
//         CRect rectImage; pMenuButton->GetImageRect(rectImage);                 // 0x175ea0
//         rectImage.InflateRect(2 * afxData.cxBorder2, 2 * afxData.cyBorder2);   // 0x3c32d0 / 0x3c32d4
//         ::InvalidateRect(m_hWnd, &rectImage, TRUE);  ::UpdateWindow(m_hWnd);
//     } else if ((nOldStyle ^ nStyle) != 2) {                                     // `xor; cmp $0x2` -- a LITERAL 2
//         InvalidateButton(nIndex);                                              // 0x14fe50
//     }
// The final test compares the changed-bits mask against the literal 2 (which is
// TBBS_CHECKBOX's value, NOT TBBS_PRESSED = 0x20000); it is transcribed as the
// literal.  0x3c32c0 (mfc140u) is ?afxData@@3UAUX_DATA@@A, so 0x3c32d0 /
// 0x3c32d4 are AUX_DATA::cxBorder2 / cyBorder2 at +0x10 / +0x14 -- the same
// pair CMFCVisualManager.cpp reads for its checked-button deflate.  The
// AUX_DATA static initializer (RVA 0x2270, mfc140u: `mov $0x2,%eax` then two
// stores at 0x22c8 / 0x22ce) sets both to 2; OpenMFC's exported afxData is a
// zero-initialised blob (featurepack/CMFC_misc_stubs.cpp), so, as in
// CMFCVisualManager.cpp, the retail value is applied as a constant (inflate by
// 4 / 4).  GetImageRect is still a stub in
// featurepack/toolbar/CMFCToolBarMenuButton.cpp (definition with an
// auto-generated parameter list), so today the rect it returns is (0,0,0,0).
// Symbol: ?SetButtonStyle@CMFCPopupMenuBar@@UEAAXHI@Z
extern "C" void MS_ABI impl__SetButtonStyle_CMFCPopupMenuBar__UEAAXHI_Z(CMFCPopupMenuBar* pThis, int nIndex, unsigned int nStyle)
{
    if (!pThis) return;
    CMFCToolBar* pBar = AsToolBar(pThis);
    CMFCToolBarButton* pButton = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, nIndex);
    if (pButton == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }

    const unsigned int nOldStyle = pButton->m_nStyle;
    if (nOldStyle == nStyle) return;
    pButton->m_nStyle = nStyle;
    if ((nOldStyle & nStyle) & kTbbsPressed) return;

    CMFCToolBarButton* pAgain = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, nIndex);
    if (IsMenuButton(pAgain) && ((nOldStyle ^ nStyle) & kTbbsChecked)) {
        CRect rectImage(0, 0, 0, 0);
        impl__GetImageRect_CMFCToolBarMenuButton__QEAAXAEAVCRect___Z(static_cast<CMFCToolBarMenuButton*>(pAgain), &rectImage);
        ::InflateRect(static_cast<RECT*>(rectImage), 2 * kAuxDataCxBorder2, 2 * kAuxDataCyBorder2);   // 2 * afxData.cxBorder2 / cyBorder2
        ::InvalidateRect(HWndOf(pThis), static_cast<const RECT*>(rectImage), TRUE);
        ::UpdateWindow(HWndOf(pThis));
    } else if ((nOldStyle ^ nStyle) != 2u) {   // retail compares against the literal 2 (see above)
        impl__InvalidateButton_CMFCToolBar__QEAAPEAVCMFCToolBarButton__H_Z(pBar, nIndex);
    }
}

// Retail (RVA 0xbf5f0, mfc140u), fully transcribed:
//     m_bInCommand = bInCommand;                                                 // +0x1384
//     CMFCPopupMenu* pParentMenu = DYNAMIC_DOWNCAST(CMFCPopupMenu, CWnd::FromHandle(::GetParent(m_hWnd)));
//     if (pParentMenu == NULL) return;
//     for (CMFCPopupMenu* p = pParentMenu->GetParentPopupMenu(); p != NULL; p = p->GetParentPopupMenu()) {   // 0xb7550
//         CMFCPopupMenuBar* pBar = p->GetMenuBar();                              // vslot 0x3a0
//         if (pBar != NULL) pBar->SetInCommand(bInCommand);                      // recursion, 0xbf5f0
//     }
// GetMenuBar is folded to its retail base body (`this + 0x230`).
// Symbol: ?SetInCommand@CMFCPopupMenuBar@@IEAAXH@Z
extern "C" void MS_ABI impl__SetInCommand_CMFCPopupMenuBar__IEAAXH_Z(CMFCPopupMenuBar* pThis, int bInCommand)
{
    if (!pThis) return;
    At<int>(pThis, kOffInCommand) = bInCommand;
    CMFCPopupMenu* pParentMenu = ParentPopupMenu(pThis);
    if (pParentMenu == nullptr) return;
    for (void* p = impl__GetParentPopupMenu_CMFCPopupMenu__QEBAPEAV1_XZ(pParentMenu); p != nullptr;
         p = impl__GetParentPopupMenu_CMFCPopupMenu__QEBAPEAV1_XZ(p)) {
        impl__SetInCommand_CMFCPopupMenuBar__IEAAXH_Z(PopupMenuBarOf(p), bInCommand);   // vslot 0x3a0, never NULL
    }
}

// Retail (RVA 0xbfc60, mfc140u), fully transcribed:
//     if (m_bDropDownListMode) {                                                 // +0x1398
//         CWnd* pOwner = CWnd::FromHandle(m_hWndOwner ? m_hWndOwner : ::GetParent(m_hWnd));   // +0xa0
//         ::SendMessage(pOwner->m_hWnd, WM_SETMESSAGESTRING, AFX_IDS_IDLEMESSAGE, 0);   // uiCmdId is NOT used; pOwner unchecked in retail
//         return;
//     }
//     CMFCToolBar::ShowCommandMessageString(uiCmdId);                            // 0x1566f0, tail jump
// The owner lookup takes the ::GetParent branch (see OwnerHwnd).
// Symbol: ?ShowCommandMessageString@CMFCPopupMenuBar@@MEAAXI@Z
extern "C" void MS_ABI impl__ShowCommandMessageString_CMFCPopupMenuBar__MEAAXI_Z(CMFCPopupMenuBar* pThis, unsigned int uiCmdId)
{
    if (!pThis) return;
    if (At<int>(pThis, kOffDropDownListMode) != 0) {
        CWnd* pOwner = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(OwnerHwnd(pThis));
        if (pOwner != nullptr) {
            ::SendMessage(HWndOf(pOwner), kWmSetMessageString, kAfxIdsIdleMessage, 0);
        }
        return;
    }
    impl__ShowCommandMessageString_CMFCToolBar__MEAAXI_Z(AsToolBar(pThis), uiCmdId);
}

// Retail (RVA 0xbe6e0, mfc140u), fully transcribed:
//     if (m_pDelayedPopupMenuButton != NULL) ::KillTimer(m_hWnd, 0xEC17);         // +0x13d8
//     m_pDelayedPopupMenuButton = pMenuButton;
//     if (pMenuButton == NULL) return;
//     if (pMenuButton == m_pDelayedClosePopupMenuButton) {                       // +0x13e0: hovering back onto the submenu being closed
//         RestoreDelayedSubMenu();                                               // vslot 0x898 (slot 275)
//         m_pDelayedPopupMenuButton = NULL;
//         return;
//     }
//     ::SetTimer(m_hWnd, 0xEC17, nDelayFactor * m_uiPopupTimerDelay, NULL);      // 0x3b1b38
// RestoreDelayedSubMenu goes through this file's thunk (DEVIATION for a client
// override).
// Symbol: ?StartPopupMenuTimer@CMFCPopupMenuBar@@QEAAXPEAVCMFCToolBarMenuButton@@H@Z
extern "C" void MS_ABI impl__StartPopupMenuTimer_CMFCPopupMenuBar__QEAAXPEAVCMFCToolBarMenuButton__H_Z(
    CMFCPopupMenuBar* pThis, CMFCToolBarMenuButton* pMenuButton, int nDelayFactor)
{
    if (!pThis) return;
    const HWND hWnd = HWndOf(pThis);
    if (At<void*>(pThis, kOffDelayedPopupMenuButton) != nullptr) {
        ::KillTimer(hWnd, kTimerDelayedPopup);
    }
    At<CMFCToolBarMenuButton*>(pThis, kOffDelayedPopupMenuButton) = pMenuButton;
    if (pMenuButton == nullptr) return;
    if (pMenuButton == At<CMFCToolBarMenuButton*>(pThis, kOffDelayedClosePopupMenuButton)) {
        impl__RestoreDelayedSubMenu_CMFCPopupMenuBar__UEAAXXZ(pThis);                     // vslot 0x898
        At<CMFCToolBarMenuButton*>(pThis, kOffDelayedPopupMenuButton) = nullptr;
        return;
    }
    ::SetTimer(hWnd, kTimerDelayedPopup,
               static_cast<UINT>(static_cast<unsigned int>(nDelayFactor) * impl__m_uiPopupTimerDelay_CMFCPopupMenuBar__1IA), nullptr);
}
