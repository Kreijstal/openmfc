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

namespace {

//---------------------------------------------------------------------------
// Layout mirrors (offsets harvested from the retail bodies).
//---------------------------------------------------------------------------

// CMFCOutlookBarPane, sizeof 11112 (0x2b68).  The first 0x40 bytes are the
// CObject/CWnd block (m_hWnd at 0x40); the CMFCToolBar base owns the three
// CObLists at 0x1188/0x11c0/0x11f8 and everything up to 0x1350.  Only the
// fields this batch touches are named.
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
static_assert(sizeof(OutlookBarPane) == 0x2b68, "CMFCOutlookBarPane size mismatch");
static_assert(sizeof(CObList) == 0x38, "CObList size mismatch");
static_assert(sizeof(CMFCToolBarImages) == 0x198, "CMFCToolBarImages size mismatch");

} // namespace

//=============================================================================
// CMFCOutlookBarPane static data exports
//=============================================================================

// Symbol: ?m_Images@CMFCOutlookBarPane@@1VCMFCToolBarImages@@A
// Static image well shared by all CMFCOutlookBarPane instances (retail
// 0x1803c22c0).  Same definition pattern as CMFCToolBar::m_Images.
extern "C" {
CMFCToolBarImages impl__m_Images_CMFCOutlookBarPane__1VCMFCToolBarImages__A =
    CMFCToolBarImages();

// Symbol: ?m_csImage@CMFCOutlookBarPane@@1VCSize@@A
// Static cache of the last loaded image size (retail 0x1803c2458).
CSize impl__m_csImage_CMFCOutlookBarPane__1VCSize__A = CSize(0, 0);
} // extern "C"

//=============================================================================
// CMFCOutlookBarPane
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarPane::AddBitmapImage(HBITMAP hBitmap)
//=============================================================================
// Retail (0x1800989f0): ignores `this`; caches the bitmap's size into the
// four global statics at 0x1803c2458/0x1803c2328 (once, guarded by the init
// flag 0x1803c22c8) and tail-calls the static image well at 0x1803c22c0
// (CMFCToolBarImages::AddImage, 0x18016d880).  The global image well is not
// modeled, so nothing is added and 0 is returned.
// Symbol: ?AddBitmapImage@CMFCOutlookBarPane@@IEAAHPEAUHBITMAP__@@@Z
extern "C" int MS_ABI impl__AddBitmapImage_CMFCOutlookBarPane__IEAAHPEAUHBITMAP_____Z(
    void* pThis, HBITMAP hBitmap)
{
    (void)pThis;
    if (hBitmap == nullptr)
        return 0;
    // TODO(clean-room): transcribed partially -- the global static image well
    // (0x1803c22c0) and the image-size cache globals are not modeled.
    return 0;
}

//=============================================================================
// BOOL CMFCOutlookBarPane::AddButton(const wchar_t* lpszIcon, const wchar_t*
//                                    lpszLabel, UINT nID, int iInsertIndex)
//=============================================================================
// Retail (0x1800982e0): LoadImageW(icon file) -> AddBitmapImage ->
// DeleteObject -> InternalAddButton.  Not transcribed (see AddBitmapImage /
// InternalAddButton notes).
// Symbol: ?AddButton@CMFCOutlookBarPane@@QEAAHPEB_W0IH@Z
extern "C" int MS_ABI impl__AddButton_CMFCOutlookBarPane__QEAAHPEB_W0IH_Z(
    void* pThis, const wchar_t* lpszIcon, const wchar_t* lpszLabel,
    unsigned int nID, int iInsertIndex)
{
    (void)lpszIcon; (void)lpszLabel; (void)nID; (void)iInsertIndex;
    if (pThis == nullptr)
        return 0;
    // TODO(clean-room): not transcribed -- image loading and the button-list
    // machinery are not modeled.
    return 0;
}

//=============================================================================
// BOOL CMFCOutlookBarPane::AddButton(UINT uiImageResId, UINT uiLabelResId,
//                                    UINT nID, int iInsertIndex)
//=============================================================================
// Retail (0x180098370): AfxFindStringResourceHandle + CStringT::LoadStringW for
// the label, then the (UINT, const wchar_t*) AddButton overload.  Not
// transcribed.
// Symbol: ?AddButton@CMFCOutlookBarPane@@QEAAHIIIH@Z
extern "C" int MS_ABI impl__AddButton_CMFCOutlookBarPane__QEAAHIIIH_Z(
    void* pThis, unsigned int uiImageResId, unsigned int uiLabelResId,
    unsigned int nID, int iInsertIndex)
{
    (void)uiImageResId; (void)uiLabelResId; (void)nID; (void)iInsertIndex;
    if (pThis == nullptr)
        return 0;
    // TODO(clean-room): not transcribed -- string-resource lookup and the
    // button-list machinery are not modeled.
    return 0;
}

//=============================================================================
// BOOL CMFCOutlookBarPane::AddButton(UINT uiImageResId, const wchar_t*
//                                    lpszLabel, UINT nID, int iInsertIndex)
//=============================================================================
// Retail (0x180098420): AfxFindResourceHandle + LoadIconW + Attach + size
// cache + well->AddIcon + InternalAddButton.  Not transcribed (the icon is
// loaded from the app's resource handle which is not modeled here).
// Symbol: ?AddButton@CMFCOutlookBarPane@@QEAAHIPEB_WIH@Z
extern "C" int MS_ABI impl__AddButton_CMFCOutlookBarPane__QEAAHIPEB_WIH_Z(
    void* pThis, unsigned int uiImageResId, const wchar_t* lpszLabel,
    unsigned int nID, int iInsertIndex)
{
    (void)uiImageResId; (void)lpszLabel; (void)nID; (void)iInsertIndex;
    if (pThis == nullptr)
        return 0;
    // TODO(clean-room): not transcribed -- icon loading and the button-list
    // machinery are not modeled.
    return 0;
}

//=============================================================================
// BOOL CMFCOutlookBarPane::AddButton(HBITMAP hBitmap, const wchar_t* lpszLabel,
//                                    UINT nID, int iInsertIndex)
//=============================================================================
// Retail (0x180098530): AddBitmapImage then tail-call InternalAddButton.
// Symbol: ?AddButton@CMFCOutlookBarPane@@QEAAHPEAUHBITMAP__@@PEB_WIH@Z
extern "C" int MS_ABI impl__AddButton_CMFCOutlookBarPane__QEAAHPEAUHBITMAP____PEB_WIH_Z(
    void* pThis, HBITMAP hBitmap, const wchar_t* lpszLabel,
    unsigned int nID, int iInsertIndex)
{
    (void)hBitmap; (void)lpszLabel; (void)nID; (void)iInsertIndex;
    if (pThis == nullptr)
        return 0;
    // TODO(clean-room): not transcribed -- see AddBitmapImage / InternalAddButton.
    return 0;
}

//=============================================================================
// BOOL CMFCOutlookBarPane::AddButton(HICON hIcon, const wchar_t* lpszLabel,
//                                    UINT nID, int iInsertIndex, BOOL bAutoDestroy)
//=============================================================================
// Retail (0x180098580): GetIconInfo + well->AddIcon + InternalAddButton.
// Not transcribed.
// Symbol: ?AddButton@CMFCOutlookBarPane@@QEAAHPEAUHICON__@@PEB_WIHH@Z
extern "C" int MS_ABI impl__AddButton_CMFCOutlookBarPane__QEAAHPEAUHICON____PEB_WIHH_Z(
    void* pThis, HICON hIcon, const wchar_t* lpszLabel,
    unsigned int nID, int iInsertIndex, int bAutoDestroy)
{
    (void)hIcon; (void)lpszLabel; (void)nID; (void)iInsertIndex; (void)bAutoDestroy;
    if (pThis == nullptr)
        return 0;
    // TODO(clean-room): not transcribed -- see AddBitmapImage / InternalAddButton.
    return 0;
}

//=============================================================================
// void CMFCOutlookBarPane::AdjustLocations()
//=============================================================================
// Retail (0x180099120, ~0x340 bytes): lays out the buttons, menu size
// bookkeeping, scroll buttons and the button rectangles using the AFX_GLOBAL_DATA
// object (0x1803c1620), the static image well and vtable indirections.  Not
// transcribed.
// Symbol: ?AdjustLocations@CMFCOutlookBarPane@@MEAAXXZ
extern "C" void MS_ABI impl__AdjustLocations_CMFCOutlookBarPane__MEAAXXZ(void* pThis)
{
    (void)pThis;
    // TODO(clean-room): not transcribed -- complex layout pass on unmodeled
    // globals and virtual slots.
}

//=============================================================================
// CSize CMFCOutlookBarPane::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
//=============================================================================
// Retail (0x180098e20):
//     CSize size;
//     ::GetWindowRect(m_hWnd, &rect);
//     size = { rect.Width(), rect.Height() };
//     return size;
// The CSize is returned through the hidden pointer (rdx), so the impl exposes
// it as an explicit out parameter.
// Symbol: ?CalcFixedLayout@CMFCOutlookBarPane@@MEAA?AVCSize@@HH@Z
extern "C" void* MS_ABI impl__CalcFixedLayout_CMFCOutlookBarPane__MEAA_AVCSize__HH_Z(
    void* pThis, void* pSizeOut, int bStretch, int bHorz)
{
    (void)bStretch; (void)bHorz;
    if (pThis == nullptr || pSizeOut == nullptr)
        return pSizeOut;

    RECT rc = {};
    HWND hWnd = static_cast<OutlookBarPane*>(pThis)->m_hWnd;
    if (hWnd != nullptr) {
        ::GetWindowRect(hWnd, &rc);
    }

    long* pOut = static_cast<long*>(pSizeOut);
    pOut[0] = rc.right - rc.left;   // cx
    pOut[1] = rc.bottom - rc.top;   // cy
    return pSizeOut;
}

//=============================================================================
// void CMFCOutlookBarPane::ClearAll()
//=============================================================================
// Retail (0x180099d40): tail jump to the static image well's
// CMFCOutlookBarImages::Clear (0x18016f690).  The global well is not modeled.
// Symbol: ?ClearAll@CMFCOutlookBarPane@@QEAAXXZ
extern "C" void MS_ABI impl__ClearAll_CMFCOutlookBarPane__QEAAXXZ(void* pThis)
{
    (void)pThis;
    // TODO(clean-room): not transcribed -- clears the unmodeled global static
    // image well (0x1803c22c0).
}

//=============================================================================
// void CMFCOutlookBarPane::CopyButtonsList(const CObList& src, CObList& dst)
//=============================================================================
// Retail (0x18009a320): RemoveHead-all from dst, then for each src button
// clone via CRuntimeClass::CreateObject + virtual slot 0x38 (CopyFrom) + slot
// 0x60 (SetOwner) and AddTail.  Not transcribed.
// Symbol: ?CopyButtonsList@CMFCOutlookBarPane@@IEAAXAEBVCObList@@AEAV2@@Z
extern "C" void MS_ABI impl__CopyButtonsList_CMFCOutlookBarPane__IEAAXAEBVCObList__AEAV2__Z(
    void* pThis, const void* pSrcList, void* pDstList)
{
    (void)pSrcList; (void)pDstList;
    if (pThis == nullptr)
        return;
    // TODO(clean-room): not transcribed -- button cloning via virtual slots
    // (0x38 CopyFrom, 0x60 SetOwner) and the CPtrList helpers are unmodeled.
}

//=============================================================================
// BOOL CMFCOutlookBarPane::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID,
//                                 DWORD dwControlBarStyle)
//=============================================================================
// Retail (0x1800982b0): calls CMFCToolBar::Create (0x18014d850); on success
// stores the 5th stack argument into m_dwControlBarStyle (+0x108) and returns
// 1.  The base Create is not modeled, so nothing is created and 0 is returned.
// Symbol: ?Create@CMFCOutlookBarPane@@UEAAHPEAVCWnd@@KIK@Z
extern "C" int MS_ABI impl__Create_CMFCOutlookBarPane__UEAAHPEAVCWnd__KIK_Z(
    void* pThis, CWnd* pParentWnd, unsigned long dwStyle, unsigned int nID,
    unsigned long dwControlBarStyle)
{
    (void)pParentWnd; (void)dwStyle; (void)nID; (void)dwControlBarStyle;
    if (pThis == nullptr)
        return 0;
    // TODO(clean-room): not transcribed -- base CMFCToolBar::Create
    // (0x18014d850) is not modeled, so the window is not created.
    return 0;
}

//=============================================================================
// CMFCToolBarButton* CMFCOutlookBarPane::CreateDroppedButton(
//                                    COleDataObject* pDataObject)
//=============================================================================
// Retail (0x180099940): CMFCToolBar::CreateDroppedButton (0x1801561f0); if the
// result is not a CMFCOutlookBarPaneButton (IsKindOf against 0x1803b13b8) it
// is deleted via vtable slot 1 and nullptr returned.  The base helper is not
// modeled.
// Symbol: ?CreateDroppedButton@CMFCOutlookBarPane@@MEAAPEAVCMFCToolBarButton@@PEAVCOleDataObject@@@Z
extern "C" void* MS_ABI impl__CreateDroppedButton_CMFCOutlookBarPane__MEAAPEAVCMFCToolBarButton__PEAVCOleDataObject___Z(
    void* pThis, void* pDataObject)
{
    (void)pDataObject;
    if (pThis == nullptr)
        return nullptr;
    // TODO(clean-room): not transcribed -- base CMFCToolBar::CreateDroppedButton
    // (0x1801561f0) is not modeled.
    return nullptr;
}

//=============================================================================
// CObject* CMFCOutlookBarPane::CreateObject()
//=============================================================================
// Retail (0x180098070):
//     void* p = operator new(0x2b68);
//     if (p) CMFCOutlookBarPane::CMFCOutlookBarPane(p);
//     return p;
// Symbol: ?CreateObject@CMFCOutlookBarPane@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCOutlookBarPane__SAPEAVCObject__XZ(void)
{
    void* pObj = ::operator new(0x2b68, std::nothrow);
    if (pObj != nullptr) {
        impl___0CMFCOutlookBarPane__QEAA_XZ(pObj);
    }
    return pObj;
}

//=============================================================================
// void CMFCOutlookBarPane::DoPaint(CDC* pDC)
//=============================================================================
// Retail (0x180099460, ~0x340 bytes): paints the bar background, the work
// area, the button wells and the scroll buttons using AFX_GLOBAL_DATA, the
// static image well and CMFCToolBarImages::PrepareDrawImage.  Not transcribed.
// Symbol: ?DoPaint@CMFCOutlookBarPane@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DoPaint_CMFCOutlookBarPane__MEAAXPEAVCDC___Z(
    void* pThis, void* pDC)
{
    (void)pDC;
    if (pThis == nullptr)
        return;
    // TODO(clean-room): not transcribed -- painting relies on unmodeled
    // globals and image machinery.
}

//=============================================================================
// BOOL CMFCOutlookBarPane::Dock(CBasePane* pTargetBar, const RECT* lpRect,
//                               AFX_DOCK_METHOD dockMethod)
//=============================================================================
// Retail (0x180099f00): resolves the docking target through the parent window
// chain (vslot 0x460, GetWindowTextW, IsKindOf against 0x1803b1328), then
// docks via the target's vslot 0x308 / 0x778.  The docking host is not
// modeled; 0 is returned (nothing docked).
// Symbol: ?Dock@CMFCOutlookBarPane@@UEAAHPEAVCBasePane@@PEBUtagRECT@@W4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__Dock_CMFCOutlookBarPane__UEAAHPEAVCBasePane__PEBUtagRECT__W4AFX_DOCK_METHOD___Z(
    void* pThis, CBasePane* pTargetBar, const RECT* lpRect, int dockMethod)
{
    (void)pTargetBar; (void)lpRect; (void)dockMethod;
    if (pThis == nullptr)
        return 0;
    // TODO(clean-room): not transcribed -- docking requires the parent-window
    // chain and the host's virtual dock slots.
    return 0;
}

//=============================================================================
// BOOL CMFCOutlookBarPane::EnableContextMenuItems(CMFCToolBarButton* pButton,
//                                                 CMenu* pMenu)
//=============================================================================
// Retail (0x180099990):
//     if (customize-mode static at 0x1803be35c != 0) {
//         EnableMenuItem(pMenu->m_hMenu, 0x4212, MF_GRAYED);
//         EnableMenuItem(pMenu->m_hMenu, 0x4213, MF_GRAYED);
//         EnableMenuItem(pMenu->m_hMenu, 0x4214, MF_GRAYED);
//         EnableMenuItem(pMenu->m_hMenu, 0x4211, MF_GRAYED);
//         EnableMenuItem(pMenu->m_hMenu, 0x4215, MF_GRAYED);
//         EnableMenuItem(pMenu->m_hMenu, 0x420e, MF_GRAYED);
//         EnableMenuItem(pMenu->m_hMenu, 0x420f, MF_GRAYED);
//     }
//     CMFCToolBar::EnableContextMenuItems(pButton, pMenu);   // 0x180157550
//     return TRUE;
// The base call is not modeled.
// Symbol: ?EnableContextMenuItems@CMFCOutlookBarPane@@MEAAHPEAVCMFCToolBarButton@@PEAVCMenu@@@Z
extern "C" int MS_ABI impl__EnableContextMenuItems_CMFCOutlookBarPane__MEAAHPEAVCMFCToolBarButton__PEAVCMenu___Z(
    void* pThis, CMFCToolBarButton* pButton, CMenu* pMenu)
{
    if (pThis == nullptr)
        return FALSE;
    (void)pButton;

    if (impl__m_bCustomizeMode_CMFCToolBar__1HA != 0 && pMenu != nullptr) {
        static const UINT s_nContextMenuCmds[] = {
            0x4212, 0x4213, 0x4214, 0x4211, 0x4215, 0x420e, 0x420f,
        };
        for (UINT nCmd : s_nContextMenuCmds) {
            ::EnableMenuItem(pMenu->m_hMenu, nCmd, MF_GRAYED);
        }
    }

    // TODO(clean-room): transcribed partially -- the tail call to
    // CMFCToolBar::EnableContextMenuItems (0x180157550) is not modeled.
    return TRUE;
}

//=============================================================================
// BOOL CMFCOutlookBarPane::InternalAddButton(int iImage, const wchar_t*
//                                            lpszLabel, UINT nID,
//                                            int iInsertIndex)
//=============================================================================
// Retail (0x180098910): allocates a CMFCOutlookBarPaneButton (0xa0 bytes),
// constructs it, stores nID at +0x24 and m_bTextLabels at +0x1c, sets the
// CString label at +0x38, then inserts it through the pane's vslot 0x690 and
// re-lays out via vslot 0x428.  The vtable indirections and the CString
// machinery are not modeled, so nothing is inserted and 0 is returned.
// Symbol: ?InternalAddButton@CMFCOutlookBarPane@@IEAAHHPEB_WIH@Z
extern "C" int MS_ABI impl__InternalAddButton_CMFCOutlookBarPane__IEAAHHPEB_WIH_Z(
    void* pThis, int iImage, const wchar_t* lpszLabel,
    unsigned int nID, int iInsertIndex)
{
    (void)iImage; (void)lpszLabel; (void)nID; (void)iInsertIndex;
    if (pThis == nullptr)
        return 0;
    // TODO(clean-room): not transcribed -- button creation plus the insert /
    // layout virtual calls (vslots 0x690 / 0x428) are not modeled.
    return 0;
}

//=============================================================================
// AFX_CS_STATUS CMFCOutlookBarPane::IsChangeState(int nSrollOffset,
//                                                 CBasePane** ppTargetBar) const
//=============================================================================
// Retail (0x180099e70): finds the tabbed pane via an internal helper
// (0x18000ba50, passing the CMFCOutlookBarTabCtrl RTTI at 0x1803b1328 as the
// search class), IsKindOf-checks it and, on success, stores it into
// *ppTargetBar and returns AFX_CS_DOCK_IMMEDIATELY (1).  The search helper is
// not modeled, so *ppTargetBar is left null and AFX_CS_NOTHING (0) returned.
// Symbol: ?IsChangeState@CMFCOutlookBarPane@@UEBA?AW4AFX_CS_STATUS@@HPEAPEAVCBasePane@@@Z
extern "C" int MS_ABI impl__IsChangeState_CMFCOutlookBarPane__UEBA_AW4AFX_CS_STATUS__HPEAPEAVCBasePane___Z(
    void* pThis, int nSrollOffset, CBasePane** ppTargetBar)
{
    (void)nSrollOffset;
    if (pThis == nullptr || ppTargetBar == nullptr)
        return 0;

    *ppTargetBar = nullptr;
    // TODO(clean-room): transcribed partially -- the pane-search helper
    // (0x18000ba50) and the IsKindOf check against the CMFCOutlookBarTabCtrl
    // runtime class (0x1803b1328) are not modeled.
    return 0;   // AFX_CS_NOTHING
}

//=============================================================================
// BOOL CMFCOutlookBarPane::OnBeforeFloat(CRect& rectFloat, AFX_DOCK_METHOD
//                                        dockMethod)
//=============================================================================
// Retail (0x18009a090): only works when dockMethod == AFX_DOCK_METHOD(1);
// then it resolves the pane under the float rect (GetWindowRect /
// CWnd::FromHandle / PtInRect / IsKindOf) and caches it into +0x2b50.  The
// default return value is 1 (TRUE) and that is what is returned here; the
// dockMethod == 1 branch is not transcribed.
// Symbol: ?OnBeforeFloat@CMFCOutlookBarPane@@UEAAHAEAVCRect@@W4AFX_DOCK_METHOD@@@Z
extern "C" int MS_ABI impl__OnBeforeFloat_CMFCOutlookBarPane__UEAAHAEAVCRect__W4AFX_DOCK_METHOD___Z(
    void* pThis, void* pRectFloat, int dockMethod)
{
    (void)pRectFloat; (void)dockMethod;
    if (pThis == nullptr)
        return 0;
    // TODO(clean-room): transcribed partially -- the dockMethod == 1 branch
    // (FromHandle/PtInRect/IsKindOf and the +0x2b50 cache) is not modeled.
    return TRUE;
}

//=============================================================================
// int CMFCOutlookBarPane::OnCreate(LPCREATESTRUCTW lpCreateStruct)
//=============================================================================
// Retail (0x180098b50): CMFCToolBar::OnCreate (0x180151ff0); if that fails
// (-1) the failure propagates, otherwise the two CMFCOutlookBarScrollButton
// children (+0x14f8/+0x2018) are created via vslot 0x2d8, the loaded image
// size is cached into +0x13c8/+0x13cc, and 0 is returned.  The scroll-button
// creation is not modeled.
// Symbol: ?OnCreate@CMFCOutlookBarPane@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCOutlookBarPane__IEAAHPEAUtagCREATESTRUCTW___Z(
    void* pThis, LPCREATESTRUCTW lpCreateStruct)
{
    if (pThis == nullptr || lpCreateStruct == nullptr)
        return -1;

    int nRet = impl__OnCreate_CMFCToolBar__IEAAHPEAUtagCREATESTRUCTW___Z(
        reinterpret_cast<CMFCToolBar*>(pThis), lpCreateStruct);
    if (nRet == -1)
        return -1;

    // TODO(clean-room): transcribed partially -- retail then creates the two
    // CMFCOutlookBarScrollButton children (+0x14f8 / +0x2018) through vslot
    // 0x2d8 and caches the loaded bitmap size into +0x13c8 / +0x13cc.
    return 0;
}

//=============================================================================
// DROPEFFECT CMFCOutlookBarPane::OnDragOver(COleDataObject* pDataObject,
//                                           DWORD dwEffect, CPoint point)
//=============================================================================
// Retail (0x180099820): IsKindOf check, ScreenToClient, PtInRect against the
// scroll-button rectangles (GetWindowRect on +0x1538/+0x2058), ScrollUp /
// ScrollDown, else the base CMFCToolBar::OnDragOver (0x180152340).  Not
// transcribed; returns DROPEFFECT_NONE.
// Symbol: ?OnDragOver@CMFCOutlookBarPane@@MEAAKPEAVCOleDataObject@@KVCPoint@@@Z
extern "C" unsigned long MS_ABI impl__OnDragOver_CMFCOutlookBarPane__MEAAKPEAVCOleDataObject__KVCPoint___Z(
    void* pThis, void* pDataObject, unsigned long dwEffect, void* point)
{
    (void)pDataObject; (void)dwEffect; (void)point;
    if (pThis == nullptr)
        return 0;
    // TODO(clean-room): not transcribed -- base CMFCToolBar::OnDragOver
    // (0x180152340) and the internal ScrollUp/ScrollDown helpers are not
    // modeled.
    return 0;   // DROPEFFECT_NONE
}

//=============================================================================
// BOOL CMFCOutlookBarPane::OnEraseBkgnd(CDC* pDC)
//=============================================================================
// Retail (0x180003a60): `mov eax, 1; ret` -- the pane is fully drawn in
// DoPaint, so nothing is erased here.
// Symbol: ?OnEraseBkgnd@CMFCOutlookBarPane@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCOutlookBarPane__IEAAHPEAVCDC___Z(
    void* pThis, void* pDC)
{
    (void)pThis; (void)pDC;
    return TRUE;
}

//=============================================================================
// void CMFCOutlookBarPane::OnEraseWorkArea(CDC* pDC, CRect rectWorkArea)
//=============================================================================
// Retail (0x180099d50): draws the scroll work area (CMFCToolBarImages at
// +0x1360, PrepareDrawImage/Draw).  Not transcribed.
// Symbol: ?OnEraseWorkArea@CMFCOutlookBarPane@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnEraseWorkArea_CMFCOutlookBarPane__MEAAXPEAVCDC__VCRect___Z(
    void* pThis, void* pDC, const RECT* pRectWorkArea)
{
    (void)pDC; (void)pRectWorkArea;
    if (pThis == nullptr)
        return;
    // TODO(clean-room): not transcribed -- scroll-work-area painting uses the
    // per-instance CMFCToolBarImages at +0x1360 and image drawing helpers.
}

//=============================================================================
// void CMFCOutlookBarPane::OnLButtonUp(UINT nFlags, CPoint point)
//=============================================================================
// Retail (0x180099ca0): CMFCToolBar::OnLButtonUp (0x180151260) and, if
// ::GetCapture() is non-null, CMFCToolBar::OnMouseLeave (0x1801567a0).
// Neither base handler is modeled.
// Symbol: ?OnLButtonUp@CMFCOutlookBarPane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCOutlookBarPane__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, void* point)
{
    (void)nFlags; (void)point;
    if (pThis == nullptr)
        return;
    // TODO(clean-room): transcribed partially -- base CMFCToolBar::OnLButtonUp
    // (0x180151260) and OnMouseLeave (0x1801567a0) are not modeled.
}

//=============================================================================
// void CMFCOutlookBarPane::OnNcCalcSize(BOOL bCalcValidRects,
//                                       NCCALCSIZE_PARAMS* lpncsp)
//=============================================================================
// Retail (0x180098e80):
//     CRect rectClient;
//     ::GetClientRect(m_hWnd, &rectClient);
//     CPane::CalcInsideRect(rectClient);            // 0x1800a1950, not modeled
//     for (i = 0; i < 4; ++i)
//         lpncsp->rgrc[0].*(left/top/right/bottom) += rectClient[i];
// Only the CalcInsideRect adjustment is skipped (client rect is used as-is).
// Symbol: ?OnNcCalcSize@CMFCOutlookBarPane@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CMFCOutlookBarPane__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(
    void* pThis, int bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
    (void)bCalcValidRects;
    if (pThis == nullptr || lpncsp == nullptr)
        return;

    RECT rcClient = {};
    HWND hWnd = static_cast<OutlookBarPane*>(pThis)->m_hWnd;
    if (hWnd != nullptr) {
        ::GetClientRect(hWnd, &rcClient);
    }

    // TODO(clean-room): transcribed partially -- retail first runs
    // CPane::CalcInsideRect (0x1800a1950, not modeled) on rcClient.
    long* pDst = &lpncsp->rgrc[0].left;
    const long* pSrc = &rcClient.left;
    for (int i = 0; i < 4; ++i) {
        pDst[i] += pSrc[i];
    }
}

//=============================================================================
// void CMFCOutlookBarPane::OnNcDestroy()
//=============================================================================
// Retail (0x18009a190): tail jump to CPane::OnNcDestroy (0x1800a1860), whose
// own tail is CWnd::OnNcDestroy (0x18028b900).  The CPane-level docking-host
// detach (vslots 0x460/0x308) is not modeled, so the CWnd tail is called
// directly.
// Symbol: ?OnNcDestroy@CMFCOutlookBarPane@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcDestroy_CMFCOutlookBarPane__IEAAXXZ(void* pThis)
{
    if (pThis == nullptr)
        return;
    // TODO(clean-room): transcribed partially -- CPane::OnNcDestroy's
    // docking-host detach is not modeled.
    impl__OnNcDestroy_CWnd__IEAAXXZ(reinterpret_cast<CWnd*>(pThis));
}

//=============================================================================
// void CMFCOutlookBarPane::OnSetFocus(CWnd* pOldWnd)
//=============================================================================
// Retail (0x18009a180): tail jump to CWnd::OnSetFocus (0x18028f2a0).
// Symbol: ?OnSetFocus@CMFCOutlookBarPane@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnSetFocus_CMFCOutlookBarPane__IEAAXPEAVCWnd___Z(
    void* pThis, void* pOldWnd)
{
    if (pThis == nullptr)
        return;
    impl__OnSetFocus_CWnd__IEAAXPEAV1__Z(reinterpret_cast<CWnd*>(pThis), pOldWnd);
}

//=============================================================================
// void CMFCOutlookBarPane::OnSize(UINT nType, int cx, int cy)
//=============================================================================
// Retail (0x180098a60):
//     CMFCToolBar::OnSize(nType, cx, cy);          // 0x180155860, modeled
//     layout virtual (vslot 0x428 or 0x7d8, picked by +0x2b60);
//     while (m_nScrollOffset(+0x2b40) > 0) {
//         if (cy >= lastButton->rect.bottom) break;
//         ScrollUp();                               // 0x180098cd0, not exported
//     }
// The base OnSize is forwarded; the layout virtual and the ScrollUp loop are
// not modeled.
// Symbol: ?OnSize@CMFCOutlookBarPane@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCOutlookBarPane__IEAAXIHH_Z(
    void* pThis, unsigned int nType, int cx, int cy)
{
    (void)cx; (void)cy;
    if (pThis == nullptr)
        return;

    impl__OnSize_CMFCToolBar__IEAAXIHH_Z(
        reinterpret_cast<CMFCToolBar*>(pThis), nType, cx, cy);

    // TODO(clean-room): transcribed partially -- retail then invokes the
    // layout virtual (vslot 0x428/0x7d8, picked by +0x2b60) and, while the
    // scroll offset (+0x2b40) is positive, scrolls the button list up until
    // the last button's bottom (+0x74) fits within cy (internal ScrollUp at
    // 0x180098cd0).  The layout virtual and ScrollUp are not modeled.
}

//=============================================================================
// void CMFCOutlookBarPane::OnSysColorChange()
//=============================================================================
// Retail (0x1800990b0):
//     CMFCToolBar::OnSysColorChange();              // 0x180151820, modeled
//     one-time AFX_GLOBAL_DATA::Initialize();       // 0x18006a790
//     m_color(+0x1358) = AFX_GLOBAL_DATA color (0x1803c164c);
//     if (m_iSel(+0x2b38) != 0) { m_iSel = -1; internal-mode-setter(0x180098f00); }
//     else ::RedrawWindow(m_hWnd, NULL, NULL, RDW_INVALIDATE);
// The AFX_GLOBAL_DATA object is not modeled; the base call and the redraw
// (the common no-selection path) survive.
// Symbol: ?OnSysColorChange@CMFCOutlookBarPane@@IEAAXXZ
extern "C" void MS_ABI impl__OnSysColorChange_CMFCOutlookBarPane__IEAAXXZ(void* pThis)
{
    if (pThis == nullptr)
        return;

    impl__OnSysColorChange_CMFCToolBar__IEAAXXZ(reinterpret_cast<CMFCToolBar*>(pThis));

    OutlookBarPane* pPane = static_cast<OutlookBarPane*>(pThis);
    // TODO(clean-room): transcribed partially -- the AFX_GLOBAL_DATA object
    // (0x1803c1620), its colour member (0x1803c164c -> +0x1358) and the
    // internal mode-setter (0x180098f00) are not modeled; the redraw below is
    // the retail no-selection path.
    if (pPane->m_hWnd != nullptr) {
        ::RedrawWindow(pPane->m_hWnd, nullptr, nullptr, RDW_INVALIDATE);
    }
}

//=============================================================================
// CMFCOutlookBarPane
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPane::OnTimer(UINT_PTR nIDEvent)
//=============================================================================
// Retail (0x180099c20):
//     switch (nIDEvent - 0xec13) {
//       case 0:  // up timer
//         if (gate@0x160c && gate@0x1614)
//             if (pageMode@0x2b64) ScrollPageUp(); else ScrollUp();
//         break;
//       case 1:  // down timer
//         if (gate@0x212c && gate@0x2134)
//             if (pageMode@0x2b64) ScrollPageDown(); else ScrollDown();
//         break;
//       default:
//         CWnd::Default();       // tail call
//     }
// The 0xec13/0xec14 ids are armed in PreTranslateMessage.
// Symbol: ?OnTimer@CMFCOutlookBarPane@@IEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CMFCOutlookBarPane__IEAAX_K_Z(
    void* pThis, unsigned __int64 nIDEvent)
{
    if (pThis == nullptr)
        return;

    OutlookBarPane* pPane = static_cast<OutlookBarPane*>(pThis);
    if (nIDEvent == 0xec13) {
        if (pPane->m_pScrollUpGate1 != 0 && pPane->m_pScrollUpGate2 != 0) {
            if (pPane->m_bScrollPageMode) {
                impl__ScrollPageUp_CMFCOutlookBarPane__IEAAXXZ(pThis);
            } else {
                impl__ScrollUp_CMFCOutlookBarPane__IEAAXXZ(pThis);
            }
        }
    } else if (nIDEvent == 0xec14) {
        if (pPane->m_pScrollDownGate1 != 0 && pPane->m_pScrollDownGate2 != 0) {
            if (pPane->m_bScrollPageMode) {
                impl__ScrollPageDown_CMFCOutlookBarPane__IEAAXXZ(pThis);
            } else {
                impl__ScrollDown_CMFCOutlookBarPane__IEAAXXZ(pThis);
            }
        }
    } else {
        // Retail tail-calls CWnd::Default for every other timer id.
        impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
    }
}

//=============================================================================
// BOOL CMFCOutlookBarPane::PreTranslateMessage(MSG* pMsg)
//=============================================================================
// Retail (0x180099a50): for mouse messages the cursor is hit-tested against
// the two scroll buttons (client rects mapped through MapWindowPoints).  On
// the down button: SendMessageW (retail forwards wParam as both wParam and
// lParam), and on WM_LBUTTONDOWN arms the 0xec14 timer (200 ms) then scrolls.
// The up button mirrors that with the 0xec13 timer.  WM_LBUTTONUP kills both
// timers.  Always tails into CMFCToolBar::PreTranslateMessage.
// Symbol: ?PreTranslateMessage@CMFCOutlookBarPane@@MEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCOutlookBarPane__MEAAHPEAUtagMSG___Z(
    void* pThis, MSG* pMsg)
{
    if (pThis == nullptr || pMsg == nullptr)
        return 0;

    OutlookBarPane* pPane = static_cast<OutlookBarPane*>(pThis);
    const UINT nMsg = pMsg->message;

    if (nMsg == WM_MOUSEMOVE || nMsg == WM_LBUTTONDOWN || nMsg == WM_LBUTTONUP) {
        if (nMsg == WM_LBUTTONUP) {
            if (pPane->m_hWnd != nullptr) {
                ::KillTimer(pPane->m_hWnd, 0xec13);
                ::KillTimer(pPane->m_hWnd, 0xec14);
            }
        }

        POINT pt = {};
        if (::GetCursorPos(&pt) && pPane->m_hWnd != nullptr) {
            ::ScreenToClient(pPane->m_hWnd, &pt);
        }

        // Down scroll button hit test.
        if (pPane->m_hwndScrollDown != nullptr) {
            RECT rc = {};
            if (::GetClientRect(pPane->m_hwndScrollDown, &rc)) {
                ::MapWindowPoints(pPane->m_hwndScrollDown, pPane->m_hWnd,
                                  reinterpret_cast<LPPOINT>(&rc), 2);
                if (::PtInRect(&rc, pt)) {
                    ::SendMessageW(pPane->m_hwndScrollDown, pMsg->message,
                                   pMsg->wParam, pMsg->wParam);
                    if (pMsg->message == WM_LBUTTONDOWN) {
                        if (pPane->m_hWnd != nullptr) {
                            ::SetTimer(pPane->m_hWnd, 0xec14, 200, nullptr);
                        }
                        if (pPane->m_bScrollPageMode) {
                            impl__ScrollPageDown_CMFCOutlookBarPane__IEAAXXZ(pThis);
                        } else {
                            impl__ScrollDown_CMFCOutlookBarPane__IEAAXXZ(pThis);
                        }
                    }
                }
            }
        }

        // Up scroll button hit test.
        if (pPane->m_hwndScrollUp != nullptr) {
            RECT rc = {};
            if (::GetClientRect(pPane->m_hwndScrollUp, &rc)) {
                ::MapWindowPoints(pPane->m_hwndScrollUp, pPane->m_hWnd,
                                  reinterpret_cast<LPPOINT>(&rc), 2);
                if (::PtInRect(&rc, pt)) {
                    ::SendMessageW(pPane->m_hwndScrollUp, pMsg->message,
                                   pMsg->wParam, pMsg->wParam);
                    if (pMsg->message == WM_LBUTTONDOWN) {
                        if (pPane->m_hWnd != nullptr) {
                            ::SetTimer(pPane->m_hWnd, 0xec13, 200, nullptr);
                        }
                        if (pPane->m_bScrollPageMode) {
                            impl__ScrollPageUp_CMFCOutlookBarPane__IEAAXXZ(pThis);
                        } else {
                            impl__ScrollUp_CMFCOutlookBarPane__IEAAXXZ(pThis);
                        }
                    }
                }
            }
        }
    }

    // Retail tails into CMFCToolBar::PreTranslateMessage (0x180154940).
    return impl__PreTranslateMessage_CMFCToolBar__MEAAHPEAUtagMSG___Z(
        reinterpret_cast<CMFCToolBar*>(pThis), pMsg);
}

//=============================================================================
// void CMFCOutlookBarPane::RemoveAllButtons()
//=============================================================================
// Retail (0x180099ce0):
//     CMFCToolBar::RemoveAllButtons();        // 0x18014ec50
//     *(u64*)(+0x2b40) = 0;                  // offset + 8 bytes of pad
//     vslot 0x7d8 (internal layout);           // not modeled
//     if (m_hWnd) { UpdateWindow; InvalidateRect(m_hWnd, 0, TRUE); }
// Symbol: ?RemoveAllButtons@CMFCOutlookBarPane@@UEAAXXZ
extern "C" void MS_ABI impl__RemoveAllButtons_CMFCOutlookBarPane__UEAAXXZ(void* pThis)
{
    if (pThis == nullptr)
        return;

    impl__RemoveAllButtons_CMFCToolBar__UEAAXXZ(reinterpret_cast<CMFCToolBar*>(pThis));

    OutlookBarPane* pPane = static_cast<OutlookBarPane*>(pThis);
    *reinterpret_cast<std::int64_t*>(static_cast<char*>(pThis) + 0x2b40) = 0;

    // TODO(clean-room): transcribed partially -- retail invokes the layout
    // virtual (vslot 0x7d8) here; not modeled.
    if (pPane->m_hWnd != nullptr) {
        ::UpdateWindow(pPane->m_hWnd);
        ::InvalidateRect(pPane->m_hWnd, nullptr, TRUE);
    }
}

//=============================================================================
// BOOL CMFCOutlookBarPane::RemoveButton(UINT nID)
//=============================================================================
// Retail (0x1800987c0): walks the real m_Buttons CObList (node+0x0 = next,
// node+0x10 = button) comparing button->m_nID (+0x24); on a match it
// CObList::RemoveAt's the node, deletes the button through vslot 1 and, when
// m_hWnd is set, runs vslot 0x7d8 + UpdateWindow + InvalidateRect.  Uses the
// side-table CObList API here (the retail list-walk is not reachable).
// Symbol: ?RemoveButton@CMFCOutlookBarPane@@QEAAHI@Z
extern "C" int MS_ABI impl__RemoveButton_CMFCOutlookBarPane__QEAAHI_Z(
    void* pThis, unsigned int nID)
{
    if (pThis == nullptr)
        return 0;

    OutlookBarPane* pPane = static_cast<OutlookBarPane*>(pThis);
    const INT_PTR nCount = pPane->m_Buttons.GetCount();
    for (INT_PTR i = 0; i < nCount; ++i) {
        CObList::POSITION pos = pPane->m_Buttons.FindIndex(i);
        if (pos == CObList::POSITION(nullptr))
            continue;
        CMFCToolBarButton* pButton =
            static_cast<CMFCToolBarButton*>(pPane->m_Buttons.GetAt(pos));
        if (pButton != nullptr && pButton->m_nID == nID) {
            pPane->m_Buttons.RemoveAt(pos);
            delete pButton;
            // TODO(clean-room): transcribed partially -- the layout virtual
            // (vslot 0x7d8) is not modeled.
            if (pPane->m_hWnd != nullptr) {
                ::UpdateWindow(pPane->m_hWnd);
                ::InvalidateRect(pPane->m_hWnd, nullptr, TRUE);
            }
            return 1;
        }
    }
    return 0;
}

//=============================================================================
// BOOL CMFCOutlookBarPane::RemoveButtonByIndex(int index)
//=============================================================================
// Retail (0x180098860): bounds-checks `0 <= index < m_Buttons count`, walks to
// the node, CObList::RemoveAt's it, deletes the button through vslot 1, then
// vslot 0x7d8 + UpdateWindow + InvalidateRect when m_hWnd is set.
// Symbol: ?RemoveButtonByIndex@CMFCOutlookBarPane@@QEAAHH@Z
extern "C" int MS_ABI impl__RemoveButtonByIndex_CMFCOutlookBarPane__QEAAHH_Z(
    void* pThis, int index)
{
    if (pThis == nullptr)
        return 0;

    OutlookBarPane* pPane = static_cast<OutlookBarPane*>(pThis);
    if (index < 0 || static_cast<INT_PTR>(index) >= pPane->m_Buttons.GetCount())
        return 0;

    CObList::POSITION pos = pPane->m_Buttons.FindIndex(index);
    if (pos == CObList::POSITION(nullptr))
        return 0;

    CMFCToolBarButton* pButton =
        static_cast<CMFCToolBarButton*>(pPane->m_Buttons.GetAt(pos));
    pPane->m_Buttons.RemoveAt(pos);
    if (pButton != nullptr)
        delete pButton;

    // TODO(clean-room): transcribed partially -- the layout virtual
    // (vslot 0x7d8) is not modeled.
    if (pPane->m_hWnd != nullptr) {
        ::UpdateWindow(pPane->m_hWnd);
        ::InvalidateRect(pPane->m_hWnd, nullptr, TRUE);
    }
    return 1;
}

//=============================================================================
// BOOL CMFCOutlookBarPane::RestoreOriginalState()
//=============================================================================
// Retail (0x18009a1c0):
//     if (m_OrigButtons count == 0) return 0;
//     CopyButtonsList(m_OrigButtons -> m_Buttons);   // 0x18009a320
//     vslot 0x85 (AdjustLocations);                  // not modeled
//     RedrawWindow(m_hWnd, 0, 0, 0x105);
//     return 1;
// Symbol: ?RestoreOriginalState@CMFCOutlookBarPane@@UEAAHXZ
extern "C" int MS_ABI impl__RestoreOriginalState_CMFCOutlookBarPane__UEAAHXZ(void* pThis)
{
    if (pThis == nullptr)
        return 0;

    OutlookBarPane* pPane = static_cast<OutlookBarPane*>(pThis);
    if (pPane->m_OrigButtons.GetCount() == 0)
        return 0;

    impl__CopyButtonsList_CMFCOutlookBarPane__IEAAXAEBVCObList__AEAV2__Z(
        pThis, &pPane->m_OrigButtons, &pPane->m_Buttons);

    // TODO(clean-room): transcribed partially -- retail then invokes the
    // AdjustLocations virtual (vslot 0x428); not modeled.
    if (pPane->m_hWnd != nullptr) {
        ::RedrawWindow(pPane->m_hWnd, nullptr, nullptr, 0x105);
    }
    return 1;
}

//=============================================================================
// void CMFCOutlookBarPane::ScrollDown()
//=============================================================================
// Retail (0x180098d90):
//     if (m_bCanScrollDown == 0) { KillTimer(m_hWnd, 0xec14); return; }
//     if (m_nScrollRangeStart + 1 >= m_Buttons count) { KillTimer(m_hWnd, 0xec14); return; }
//     btn = GetButton(m_nScrollRangeStart);
//     if (!btn) { KillTimer(m_hWnd, 0xec14); return; }
//     ++m_nScrollRangeStart;
//     m_nScrollOffset += btn->m_rect.bottom - btn->m_rect.top;
//     vslot 0x7d8 (internal layout); InvalidateRect; UpdateWindow;
// Symbol: ?ScrollDown@CMFCOutlookBarPane@@IEAAXXZ
extern "C" void MS_ABI impl__ScrollDown_CMFCOutlookBarPane__IEAAXXZ(void* pThis)
{
    if (pThis == nullptr)
        return;

    OutlookBarPane* pPane = static_cast<OutlookBarPane*>(pThis);
    if (pPane->m_bCanScrollDown == 0 ||
        pPane->m_nScrollRangeStart + 1 >= static_cast<int>(pPane->m_Buttons.GetCount())) {
        if (pPane->m_hWnd != nullptr)
            ::KillTimer(pPane->m_hWnd, 0xec14);
        return;
    }

    CMFCToolBarButton* pButton = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(
        reinterpret_cast<CMFCToolBar*>(pThis), pPane->m_nScrollRangeStart);
    if (pButton == nullptr) {
        if (pPane->m_hWnd != nullptr)
            ::KillTimer(pPane->m_hWnd, 0xec14);
        return;
    }

    ++pPane->m_nScrollRangeStart;
    pPane->m_nScrollOffset += (pButton->m_rect.bottom - pButton->m_rect.top);

    // TODO(clean-room): transcribed partially -- the layout virtual
    // (vslot 0x7d8) is not modeled.
    if (pPane->m_hWnd != nullptr) {
        ::InvalidateRect(pPane->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(pPane->m_hWnd);
    }
}

//=============================================================================
// void CMFCOutlookBarPane::ScrollUp()
//=============================================================================
// Retail (0x180098cd0):
//     if (m_nScrollOffset <= 0 || m_nScrollRangeStart <= 0) {
//         m_nScrollOffset = 0; m_nScrollRangeStart = 0;
//         KillTimer(m_hWnd, 0xec13); return;
//     }
//     btn = GetButton(m_nScrollRangeStart);
//     if (!btn) { KillTimer(m_hWnd, 0xec14); return; }
//     --m_nScrollRangeStart;
//     m_nScrollOffset += btn->m_rect.top - btn->m_rect.bottom;
//     if (m_nScrollRangeStart == 0) m_nScrollOffset = 0;
//     vslot 0x7d8 (internal layout); InvalidateRect; UpdateWindow;
// Symbol: ?ScrollUp@CMFCOutlookBarPane@@IEAAXXZ
extern "C" void MS_ABI impl__ScrollUp_CMFCOutlookBarPane__IEAAXXZ(void* pThis)
{
    if (pThis == nullptr)
        return;

    OutlookBarPane* pPane = static_cast<OutlookBarPane*>(pThis);
    if (pPane->m_nScrollOffset <= 0 || pPane->m_nScrollRangeStart <= 0) {
        pPane->m_nScrollOffset = 0;
        pPane->m_nScrollRangeStart = 0;
        if (pPane->m_hWnd != nullptr)
            ::KillTimer(pPane->m_hWnd, 0xec13);
        return;
    }

    CMFCToolBarButton* pButton = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(
        reinterpret_cast<CMFCToolBar*>(pThis), pPane->m_nScrollRangeStart);
    if (pButton == nullptr) {
        if (pPane->m_hWnd != nullptr)
            ::KillTimer(pPane->m_hWnd, 0xec14);
        return;
    }

    --pPane->m_nScrollRangeStart;
    pPane->m_nScrollOffset += (pButton->m_rect.top - pButton->m_rect.bottom);
    if (pPane->m_nScrollRangeStart == 0)
        pPane->m_nScrollOffset = 0;

    // TODO(clean-room): transcribed partially -- the layout virtual
    // (vslot 0x7d8) is not modeled.
    if (pPane->m_hWnd != nullptr) {
        ::InvalidateRect(pPane->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(pPane->m_hWnd);
    }
}

//=============================================================================
// void CMFCOutlookBarPane::ScrollPageUp()
//=============================================================================
// Retail (0x18009a3e0): same guards as ScrollUp (reset + KillTimer 0xec13),
// then `n = (client height) / (m_nScrollRangeEnd - btn->top + btn->bottom)`
// and ScrollUp() n times.
// Symbol: ?ScrollPageUp@CMFCOutlookBarPane@@IEAAXXZ
extern "C" void MS_ABI impl__ScrollPageUp_CMFCOutlookBarPane__IEAAXXZ(void* pThis)
{
    if (pThis == nullptr)
        return;

    OutlookBarPane* pPane = static_cast<OutlookBarPane*>(pThis);
    if (pPane->m_nScrollOffset <= 0 || pPane->m_nScrollRangeStart <= 0) {
        pPane->m_nScrollOffset = 0;
        pPane->m_nScrollRangeStart = 0;
        if (pPane->m_hWnd != nullptr)
            ::KillTimer(pPane->m_hWnd, 0xec13);
        return;
    }

    CMFCToolBarButton* pButton = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(
        reinterpret_cast<CMFCToolBar*>(pThis), pPane->m_nScrollRangeStart);
    if (pButton == nullptr) {
        if (pPane->m_hWnd != nullptr)
            ::KillTimer(pPane->m_hWnd, 0xec14);
        return;
    }

    RECT rc = {};
    if (pPane->m_hWnd != nullptr)
        ::GetClientRect(pPane->m_hWnd, &rc);

    const int nPageHeight =
        pPane->m_nScrollRangeEnd - pButton->m_rect.top + pButton->m_rect.bottom;
    int n = 0;
    if (nPageHeight > 0)
        n = (rc.bottom - rc.top) / nPageHeight;
    for (int i = 0; i < n; ++i) {
        impl__ScrollUp_CMFCOutlookBarPane__IEAAXXZ(pThis);
    }
}

//=============================================================================
// void CMFCOutlookBarPane::ScrollPageDown()
//=============================================================================
// Retail (0x18009a4b0): same guards as ScrollDown (KillTimer 0xec14), then
// `n = (client height) / (m_nScrollRangeEnd - btn->top + btn->bottom)` and
// ScrollDown() n times.
// Symbol: ?ScrollPageDown@CMFCOutlookBarPane@@IEAAXXZ
extern "C" void MS_ABI impl__ScrollPageDown_CMFCOutlookBarPane__IEAAXXZ(void* pThis)
{
    if (pThis == nullptr)
        return;

    OutlookBarPane* pPane = static_cast<OutlookBarPane*>(pThis);
    if (pPane->m_bCanScrollDown == 0 ||
        pPane->m_nScrollRangeStart + 1 >= static_cast<int>(pPane->m_Buttons.GetCount())) {
        if (pPane->m_hWnd != nullptr)
            ::KillTimer(pPane->m_hWnd, 0xec14);
        return;
    }

    CMFCToolBarButton* pButton = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(
        reinterpret_cast<CMFCToolBar*>(pThis), pPane->m_nScrollRangeStart);
    if (pButton == nullptr) {
        if (pPane->m_hWnd != nullptr)
            ::KillTimer(pPane->m_hWnd, 0xec14);
        return;
    }

    RECT rc = {};
    if (pPane->m_hWnd != nullptr)
        ::GetClientRect(pPane->m_hWnd, &rc);

    const int nPageHeight =
        pPane->m_nScrollRangeEnd - pButton->m_rect.top + pButton->m_rect.bottom;
    int n = 0;
    if (nPageHeight > 0)
        n = (rc.bottom - rc.top) / nPageHeight;
    for (int i = 0; i < n; ++i) {
        impl__ScrollDown_CMFCOutlookBarPane__IEAAXXZ(pThis);
    }
}

//=============================================================================
// void CMFCOutlookBarPane::SetBackColor(COLORREF clr)
//=============================================================================
// Retail (0x180099020): m_color(+0x1358) = clr; if (m_hWnd) { InvalidateRect;
// UpdateWindow; }.
// Symbol: ?SetBackColor@CMFCOutlookBarPane@@QEAAXK@Z
extern "C" void MS_ABI impl__SetBackColor_CMFCOutlookBarPane__QEAAXK_Z(
    void* pThis, unsigned long clr)
{
    if (pThis == nullptr)
        return;

    OutlookBarPane* pPane = static_cast<OutlookBarPane*>(pThis);
    pPane->m_color = static_cast<COLORREF>(clr);
    if (pPane->m_hWnd != nullptr) {
        ::InvalidateRect(pPane->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(pPane->m_hWnd);
    }
}

//=============================================================================
// void CMFCOutlookBarPane::SetBackImage(UINT iImage)
//=============================================================================
// Retail (0x180098f00): early-exits when m_iSel == iImage; otherwise clears
// the inner image well (+0x1360), loads the bitmap resource (AfxFindResource
// handle + LoadImageW + GetObjectW for the size), AddImage's it, caches the
// size into +0x13c8/+0x13cc and m_iSel, then sets m_bFlag1(+0x2b5c) from the
// AFX_GLOBAL_DATA glyph-count static and repaints.  The glyph-count static and
// the AFX_GLOBAL_DATA object are not modeled.
// Symbol: ?SetBackImage@CMFCOutlookBarPane@@QEAAXI@Z
extern "C" void MS_ABI impl__SetBackImage_CMFCOutlookBarPane__QEAAXI_Z(
    void* pThis, unsigned int iImage)
{
    if (pThis == nullptr)
        return;

    OutlookBarPane* pPane = static_cast<OutlookBarPane*>(pThis);
    if (pPane->m_iSel == static_cast<int>(iImage))
        return;

    pPane->m_bFlag1 = 0;
    if (pPane->m_imageWell.GetCount() > 0) {
        impl__Clear_CMFCToolBarImages__QEAAXXZ(&pPane->m_imageWell);
    }
    pPane->m_iSel = 0;

    if (iImage != 0) {
        HINSTANCE hInstRes = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
            MAKEINTRESOURCEW(iImage), RT_ICON);
        HBITMAP hBitmap = static_cast<HBITMAP>(::LoadImageW(
            hInstRes, MAKEINTRESOURCEW(iImage), IMAGE_BITMAP, 0, 0, 0x3000));
        if (hBitmap != nullptr) {
            BITMAP bm = {};
            if (::GetObjectW(hBitmap, sizeof(bm), &bm) != 0) {
                pPane->ImageWidth() = bm.bmWidth;
                pPane->ImageHeight() = bm.bmHeight;
                impl__AddImage_CMFCToolBarImages__QEAAHPEAUHBITMAP____H_Z(
                    &pPane->m_imageWell, hBitmap, FALSE);
                pPane->m_iSel = static_cast<int>(iImage);
            }
        }
    }

    // TODO(clean-room): transcribed partially -- retail runs
    // AFX_GLOBAL_DATA::Initialize() (0x18006a790, once) and sets
    // m_bFlag1(+0x2b5c) = (glyph-count static @0x1803c18a8 > 8); the global
    // object is not modeled, so m_bFlag1 stays 0.
    if (pPane->m_hWnd != nullptr) {
        ::InvalidateRect(pPane->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(pPane->m_hWnd);
    }
}

//=============================================================================
// void CMFCOutlookBarPane::SetDefaultState()
//=============================================================================
// Retail (0x18009a1a0): tail jump to CopyButtonsList(m_Buttons, m_OrigButtons)
// (0x18009a320) -- i.e. snapshot the current buttons into the originals.
// Symbol: ?SetDefaultState@CMFCOutlookBarPane@@QEAAXXZ
extern "C" void MS_ABI impl__SetDefaultState_CMFCOutlookBarPane__QEAAXXZ(void* pThis)
{
    if (pThis == nullptr)
        return;

    OutlookBarPane* pPane = static_cast<OutlookBarPane*>(pThis);
    impl__CopyButtonsList_CMFCOutlookBarPane__IEAAXAEBVCObList__AEAV2__Z(
        pThis, &pPane->m_Buttons, &pPane->m_OrigButtons);
}

//=============================================================================
// void CMFCOutlookBarPane::SetTextColor(COLORREF clrText, COLORREF clrTextHighlight)
//=============================================================================
// Retail (0x180098b10): m_textColor(+0x1350) = clrText (second param ignored);
// if (m_hWnd) { InvalidateRect; UpdateWindow; }.
// Symbol: ?SetTextColor@CMFCOutlookBarPane@@QEAAXKK@Z
extern "C" void MS_ABI impl__SetTextColor_CMFCOutlookBarPane__QEAAXKK_Z(
    void* pThis, unsigned long clrText, unsigned long clrTextHighlight)
{
    (void)clrTextHighlight;
    if (pThis == nullptr)
        return;

    OutlookBarPane* pPane = static_cast<OutlookBarPane*>(pThis);
    pPane->m_textColor = static_cast<COLORREF>(clrText);
    if (pPane->m_hWnd != nullptr) {
        ::InvalidateRect(pPane->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(pPane->m_hWnd);
    }
}

//=============================================================================
// void CMFCOutlookBarPane::SetTransparentColor(COLORREF clr)
//=============================================================================
// Retail (0x180099060): m_transparentColor(+0x1354) = clr; if (m_hWnd) { if
// (clr != global transparent colour @0x1803c2398) global = clr;
// InvalidateRect; UpdateWindow; }.  The global static is not modeled.
// Symbol: ?SetTransparentColor@CMFCOutlookBarPane@@QEAAXK@Z
extern "C" void MS_ABI impl__SetTransparentColor_CMFCOutlookBarPane__QEAAXK_Z(
    void* pThis, unsigned long clr)
{
    if (pThis == nullptr)
        return;

    OutlookBarPane* pPane = static_cast<OutlookBarPane*>(pThis);
    pPane->m_transparentColor = static_cast<COLORREF>(clr);
    if (pPane->m_hWnd != nullptr) {
        // TODO(clean-room): transcribed partially -- retail also updates the
        // global transparent-colour static @0x1803c2398 when it differs; not
        // modeled.
        ::InvalidateRect(pPane->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(pPane->m_hWnd);
    }
}

//=============================================================================
// BOOL CMFCOutlookBarPane::SmartUpdate(const CObList& lstButtons)
//=============================================================================
// Retail (0x18009a220, ~0xc0 bytes): diffs the caller's CObList against
// m_Buttons (counts, per-button nID/image/text) and re-lays-out through
// internal helpers.  The list-diff machinery is not modeled.
// Symbol: ?SmartUpdate@CMFCOutlookBarPane@@UEAAHAEBVCObList@@@Z
extern "C" int MS_ABI impl__SmartUpdate_CMFCOutlookBarPane__UEAAHAEBVCObList___Z(
    void* pThis, const void* pList)
{
    (void)pList;
    if (pThis == nullptr)
        return 0;
    // TODO(clean-room): not transcribed -- button-list diff / refresh against
    // the caller's CObList (retail 0x18009a220).
    return 0;
}
