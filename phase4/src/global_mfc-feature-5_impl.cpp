// CMFCToolBar Feature Pack exports, wave 5 ("mfc-feature-5").
//
// 20 methods + 10 static data exports transcribed from the retail mfc140u
// export (ordinal -> RVA -> objdump) against the member layout documented in
// include/openmfc/afxmfc.h (CMFCToolBar own members run 0x3F8..0x1350).
//
// Where the retail body depends on unmodeled state (visual-manager virtuals,
// the AFX_GLOBAL_DATA singleton, CMFCToolBarImages::OnSysColorChange is an
// auto-stub, CObList internals live in a side table) the implementation keeps
// the observable side effects (member writes, region creation, WM_COMMAND
// posts, VARIANT results) and marks the gaps with `// TODO(clean-room)`.
//
// The ten DATA exports (CMFCToolBar's static image lists plus the drop source
// and the default-command map) are defined at the top of this file with
// `extern "C"` storage; the `// Symbol:` comments exclude them from the
// auto-generated stubs.  The eight CMFCToolBarImages statics use the modeled
// 408-byte type; m_DefaultImages (retail CMap<UINT,UINT,int,int>, 56 bytes)
// and m_DropSource (retail CMFCToolBarDropSource, 144 bytes) are opaque local
// structs sized to the harvested retail layout because those classes are not
// declared in the clean-room headers.  UpdateImagesColor drives
// OnSysColorChange over 7 of the 8 statics (m_DisabledImages is skipped in
// retail) plus all 8 *_Locked lists.

#include "openmfc/afxmfc.h"

#include <cstdint>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// VT_EMPTY / VT_I4 and S_OK / S_FALSE / E_INVALIDARG from wtypes.h (via
// windows.h) are used below; keep the numeric spellings for the HRESULTs so
// the file compiles even when <oleauto.h> is not pulled in.
#ifndef S_OK
  #define S_OK 0L
#endif
#ifndef S_FALSE
  #define S_FALSE 1L
#endif
#ifndef E_INVALIDARG
  #define E_INVALIDARG 0x80070057L
#endif

// ---------------------------------------------------------------------------
// Cross-file impl_ helpers.
// ---------------------------------------------------------------------------

// CMFCToolBarImages::OnSysColorChange (auto-stub until a real impl lands).
extern "C" void MS_ABI impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(CMFCToolBarImages* pThis);

//=============================================================================
// Static data exports
//=============================================================================
// Retail sizes harvested from the .data section of mfc140u.dll:
//   m_DefaultImages (CMap<UINT,UINT,int,int>) = 56 bytes  (0x3b1f90)
//   m_DropSource     (CMFCToolBarDropSource)  = 144 bytes (0x3c2510)
//   eight CMFCToolBarImages (408 bytes each)  = 0x3c25a0..0x3c3100
// The OpenMFC CMap<UINT,UINT,int,int> template is 40 bytes (no CObject base),
// so m_DefaultImages uses an opaque 56-byte blob to keep the exported object
// at retail size; CMFCToolBarDropSource is not declared in the clean-room
// headers, so m_DropSource uses a local 144-byte blob.

struct alignas(8) CMap_IIHH_56Bytes {
    unsigned char data[56];
};

struct alignas(8) CToolBarDropSource_144Bytes {
    unsigned char data[144];
};

// Symbol: ?m_DefaultImages@CMFCToolBar@@1V?$CMap@IIHH@@A
extern "C" CMap_IIHH_56Bytes impl__m_DefaultImages_CMFCToolBar__1V__CMap_IIHH__A = {};

// Symbol: ?m_DropSource@CMFCToolBar@@1VCMFCToolBarDropSource@@A
extern "C" CToolBarDropSource_144Bytes impl__m_DropSource_CMFCToolBar__1VCMFCToolBarDropSource__A = {};

// The eight CMFCToolBarImages statics below are shared with other CMFCToolBar
// code through their exported symbols, so they keep plain (non-static)
// linkage; `extern "C"` plus an initializer in one declaration matches the
// house pattern in feature_static_data.cpp and keeps the object at the
// modeled 408-byte retail size.
// Symbol: ?m_Images@CMFCToolBar@@1VCMFCToolBarImages@@A
extern "C" CMFCToolBarImages impl__m_Images_CMFCToolBar__1VCMFCToolBarImages__A = CMFCToolBarImages();

// Symbol: ?m_ColdImages@CMFCToolBar@@1VCMFCToolBarImages@@A
extern "C" CMFCToolBarImages impl__m_ColdImages_CMFCToolBar__1VCMFCToolBarImages__A = CMFCToolBarImages();

// Symbol: ?m_DisabledImages@CMFCToolBar@@1VCMFCToolBarImages@@A
extern "C" CMFCToolBarImages impl__m_DisabledImages_CMFCToolBar__1VCMFCToolBarImages__A = CMFCToolBarImages();

// Symbol: ?m_DisabledMenuImages@CMFCToolBar@@1VCMFCToolBarImages@@A
extern "C" CMFCToolBarImages impl__m_DisabledMenuImages_CMFCToolBar__1VCMFCToolBarImages__A = CMFCToolBarImages();

// Symbol: ?m_LargeImages@CMFCToolBar@@1VCMFCToolBarImages@@A
extern "C" CMFCToolBarImages impl__m_LargeImages_CMFCToolBar__1VCMFCToolBarImages__A = CMFCToolBarImages();

// Symbol: ?m_LargeColdImages@CMFCToolBar@@1VCMFCToolBarImages@@A
extern "C" CMFCToolBarImages impl__m_LargeColdImages_CMFCToolBar__1VCMFCToolBarImages__A = CMFCToolBarImages();

// Symbol: ?m_LargeDisabledImages@CMFCToolBar@@1VCMFCToolBarImages@@A
extern "C" CMFCToolBarImages impl__m_LargeDisabledImages_CMFCToolBar__1VCMFCToolBarImages__A = CMFCToolBarImages();

// Symbol: ?m_MenuImages@CMFCToolBar@@1VCMFCToolBarImages@@A
extern "C" CMFCToolBarImages impl__m_MenuImages_CMFCToolBar__1VCMFCToolBarImages__A = CMFCToolBarImages();

//=============================================================================
// CMFCToolBar::SetRoundedRgn
//=============================================================================
// Retail (0x18015b760): if m_hWnd:
//     if !m_bRoundShape (or GetVisualManager()==NULL): SetWindowRgn(hwnd,NULL,FALSE);
//     else: GetWindowRect -> CreateRoundRectRgn(0,0,w+1,h+1,4,4)
//           -> SetWindowRgn(hwnd, rgn, FALSE).
// The visual-manager gate (vslot 0x350) is not modeled, so the create path is
// taken whenever m_bRoundShape is set.
// Symbol: ?SetRoundedRgn@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__SetRoundedRgn_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    const HWND hwnd = pThis->GetSafeHwnd();
    if (!hwnd) return;

    // TODO(clean-room): retail gates the create path on GetVisualManager()
    // (vslot 0x350) being non-NULL; not modeled, so this assumes the manager
    // exists whenever m_bRoundShape is set.
    if (!pThis->m_bRoundShape) {
        // Retail clears the region when the shape flag is off.
        ::SetWindowRgn(hwnd, nullptr, FALSE);
        return;
    }

    RECT rc;
    ::GetWindowRect(hwnd, &rc);
    const int cx = rc.right - rc.left + 1;
    const int cy = rc.bottom - rc.top + 1;
    HRGN hRgn = ::CreateRoundRectRgn(0, 0, cx, cy, 4, 4);
    // Retail passes the (possibly NULL) region straight to SetWindowRgn;
    // SetWindowRgn with NULL clears the region, matching retail on failure.
    ::SetWindowRgn(hwnd, hRgn, FALSE);
}

//=============================================================================
// CMFCToolBar::SetSiblingToolBar
//=============================================================================
// Retail (0x18015b010) is exactly six stores; fully transcribed.
// Symbol: ?SetSiblingToolBar@CMFCToolBar@@QEAAXPEAV1@@Z
extern "C" void MS_ABI impl__SetSiblingToolBar_CMFCToolBar__QEAAXPEAV1__Z(
    CMFCToolBar* pThis, CMFCToolBar* pSibling)
{
    if (!pThis || !pSibling) return;

    pThis->m_pBrotherToolBar = pSibling;   // 0x12F8
    pThis->m_bHasBrother = 1;              // 0x110C
    pSibling->m_bHasBrother = 1;
    pSibling->m_pBrotherToolBar = pThis;
    pThis->m_bElderBrother = 1;            // 0x1110
    pSibling->m_bElderBrother = 0;
}

//=============================================================================
// CMFCToolBar::SetToolBarBtnText
//=============================================================================
// Retail (0x180158920):
//     pButton = GetButton(nIndex); if NULL return;
//     if (bShowText) {
//         if (szText == NULL) vslot0x750(OnSetDefaultButtonText, pButton);
//         else                SetButtonText(nIndex, szText);   // sets button text
//     }
//     pButton->m_bImage = bShowImage;   // 0x10
//     pButton->m_bText  = bShowText;    // 0x0C
// The two text virtuals are not modeled; the string write is done directly.
// Symbol: ?SetToolBarBtnText@CMFCToolBar@@QEAAXIPEB_WHH@Z
extern "C" void MS_ABI impl__SetToolBarBtnText_CMFCToolBar__QEAAXIPEB_WHH_Z(
    CMFCToolBar* pThis, unsigned int nIndex, const wchar_t* szText, int bShowText, int bShowImage)
{
    if (!pThis) return;

    CMFCToolBarButton* pButton = pThis->GetButton(static_cast<int>(nIndex));
    if (!pButton) return;

    if (bShowText) {
        if (szText) {
            // Retail delegates to CMFCToolBar::SetButtonText (0x18014f1c0),
            // which assigns the string to the button; reproduced inline.
            pButton->m_strText = szText;
        } else {
            // TODO(clean-room): retail calls vslot 0x750 (OnSetDefaultButtonText);
            // not modeled.
        }
    }
    pButton->m_bImage = bShowImage;   // @16 (retail stores the raw int)
    pButton->m_bText  = bShowText;    // @12
}

//=============================================================================
// CMFCToolBar::SetTwoRowsWithSibling
//=============================================================================
// Retail (0x18015b110) re-lays both toolbars into two rows using CMFCToolBar
// virtual layout slots (0x350, 0x4d0, 0x58) and CMFCControlBarImpl helpers,
// all unmodeled.
// Symbol: ?SetTwoRowsWithSibling@CMFCToolBar@@QEAAXXZ
extern "C" void MS_ABI impl__SetTwoRowsWithSibling_CMFCToolBar__QEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- the sibling two-row layout
    // (0x18015b110) walks virtual layout slots and CObList internals that
    // OpenMFC does not model.
}

//=============================================================================
// CMFCToolBar::SetUserImages (static)
//=============================================================================
// Retail (0x18014de40): if pImages && m_hbmImageWell && size matches the
// default button size globals -> store pImages to static m_pUserImages, return
// TRUE.  The size globals (0x1803b1cb8/0x1803b1cbc) and the m_pUserImages
// static (ordinal 8387, a different wave) are not modeled here.
// Symbol: ?SetUserImages@CMFCToolBar@@SAHPEAVCMFCToolBarImages@@@Z
extern "C" int MS_ABI impl__SetUserImages_CMFCToolBar__SAHPEAVCMFCToolBarImages___Z(
    CMFCToolBarImages* pImages)
{
    if (!pImages) return 0;
    // m_hbmImageWell (0x0A0) is protected in the header; read via the
    // documented offset.
    const void* pHbm = *reinterpret_cast<void* const*>(
        reinterpret_cast<const char*>(pImages) + 0xA0);
    if (!pHbm) return 0;
    // TODO(clean-room): retail compares m_sizeImage (0x68) against the default
    // button-size globals and stores pImages into the CMFCToolBar::m_pUserImages
    // static; neither is modeled in this wave.
    return 1;
}

//=============================================================================
// CMFCToolBar::ShowCommandMessageString
//=============================================================================
// Retail (0x1801566f0) maps the command ID to status-bar text via the
// status-bar manager singleton (0x1803be360) and AFX_GLOBAL_DATA, then posts
// WM_SETMESSAGESTRING to the frame; all unmodeled.
// Symbol: ?ShowCommandMessageString@CMFCToolBar@@MEAAXI@Z
extern "C" void MS_ABI impl__ShowCommandMessageString_CMFCToolBar__MEAAXI_Z(
    CMFCToolBar* pThis, unsigned int nID)
{
    (void)nID;
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- the command -> string table
    // lookup and WM_SETMESSAGESTRING post depend on AFX_GLOBAL_DATA and the
    // status-bar manager singleton; not modeled.
}

//=============================================================================
// CMFCToolBar::SizeToolBar
//=============================================================================
// Retail (0x1801555b0) computes the toolbar size through the visual manager
// (vslot 0x338/0x3b20), CalcLayout and CMFCToolBarImages locking; unmodeled.
// Symbol: ?SizeToolBar@CMFCToolBar@@IEAAXHH@Z
extern "C" void MS_ABI impl__SizeToolBar_CMFCToolBar__IEAAXHH_Z(
    CMFCToolBar* pThis, int cx, int cy)
{
    (void)cx; (void)cy;
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- layout math needs the visual
    // manager and CalcLayout; not modeled.
}

//=============================================================================
// CMFCToolBar::SmartUpdate
//=============================================================================
// Retail (0x180159950) walks the CObList comparing against m_OrigButtons and
// refreshes m_bResourceWasChanged / button styles; CObList contents live in a
// side table and the comparison helpers are unmodeled.
// Symbol: ?SmartUpdate@CMFCToolBar@@MEAAHAEBVCObList@@@Z
extern "C" int MS_ABI impl__SmartUpdate_CMFCToolBar__MEAAHAEBVCObList___Z(
    CMFCToolBar* pThis, const CObList& lstButtons)
{
    (void)lstButtons;
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- the retail list comparison
    // (0x180159950) is not modeled.
    return 0;
}

//=============================================================================
// CMFCToolBar::StretchPane
//=============================================================================
// Retail (0x180154c40) computes the size via SizeToolBar and CanBeResized.
// CSize is returned through the hidden return pointer used by MSVC for the
// exported ?AVCSize@@ methods (see cbarcore.cpp).
// Symbol: ?StretchPane@CMFCToolBar@@UEAA?AVCSize@@HH@Z
extern "C" void MS_ABI impl__StretchPane_CMFCToolBar__UEAA_AVCSize__HH_Z(
    CSize* pRet, CMFCToolBar* pThis, int cx, int cy)
{
    if (!pRet) return;
    new(pRet) CSize(0, 0);
    if (!pThis) return;
    (void)cx; (void)cy;
    // TODO(clean-room): transcribed partially -- retail sizes the pane through
    // SizeToolBar and CanBeResized (vslot 0x2d8); not modeled.
}

//=============================================================================
// CMFCToolBar::ToolBarMouseHookProc (static)
//=============================================================================
// Retail (0x180156cb0) is a WH_MOUSE hook proc that forwards WM_MOUSEMOVE to
// the currently hooked toolbar (global 0x1803be360) and installs/removes the
// hook; the hook-global and the mouse handling are unmodeled.
// Symbol: ?ToolBarMouseHookProc@CMFCToolBar@@KA_JH_K_J@Z
extern "C" __int64 MS_ABI impl__ToolBarMouseHookProc_CMFCToolBar__KA_JH_K_J_Z(
    int nCode, unsigned __int64 wParam, __int64 lParam)
{
    (void)wParam; (void)lParam;
    if (nCode < 0) return 0;
    // TODO(clean-room): transcribed partially -- the global hook handle and
    // toolbar pointer (0x1803be360) are not modeled.
    return 0;
}

//=============================================================================
// CMFCToolBar::TranslateChar
//=============================================================================
// Retail (0x180159570) looks the character up in the m_AccelKeys map and runs
// the matching command; the CMap internals (0x1318) are not modeled.
// Symbol: ?TranslateChar@CMFCToolBar@@UEAAHI@Z
extern "C" int MS_ABI impl__TranslateChar_CMFCToolBar__UEAAHI_Z(
    CMFCToolBar* pThis, unsigned int nChar)
{
    (void)nChar;
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- m_AccelKeys map lookup not
    // modeled.
    return 0;
}

//=============================================================================
// CMFCToolBar::UpdateButton
//=============================================================================
// Retail (0x180151e40) validates the button style range and refreshes the
// button window; internal button-state helpers are unmodeled.
// Symbol: ?UpdateButton@CMFCToolBar@@QEAAXH@Z
extern "C" void MS_ABI impl__UpdateButton_CMFCToolBar__QEAAXH_Z(
    CMFCToolBar* pThis, int iIndex)
{
    (void)iIndex;
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail (0x180151e40) redraws
    // the button through unmodeled state helpers.
}

//=============================================================================
// CMFCToolBar::UpdateImagesColor
//=============================================================================
// Retail (0x180151890) calls CMFCToolBarImages::OnSysColorChange for seven
// static image lists (m_DisabledImages is skipped) and the eight embedded
// *_Locked lists, in this exact order.  Transcribed 1:1 against the static
// data exports defined at the top of this file.
// Symbol: ?UpdateImagesColor@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateImagesColor_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;

    // Static image lists (retail .data 0x3c25a0..0x3c3100).
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&impl__m_Images_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&impl__m_ColdImages_CMFCToolBar__1VCMFCToolBarImages__A);

    // Embedded locked lists.
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&pThis->m_ImagesLocked);              // 0x3F8
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&pThis->m_ColdImagesLocked);          // 0x590

    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&impl__m_MenuImages_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&impl__m_DisabledMenuImages_CMFCToolBar__1VCMFCToolBarImages__A);

    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&pThis->m_MenuImagesLocked);          // 0xD88
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&pThis->m_DisabledImagesLocked);      // 0x728
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&pThis->m_DisabledMenuImagesLocked);  // 0xF20

    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&impl__m_LargeImages_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&impl__m_LargeColdImages_CMFCToolBar__1VCMFCToolBarImages__A);
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&impl__m_LargeDisabledImages_CMFCToolBar__1VCMFCToolBarImages__A);

    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&pThis->m_LargeImagesLocked);         // 0x8C0
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&pThis->m_LargeColdImagesLocked);     // 0xA58
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(&pThis->m_LargeDisabledImagesLocked); // 0xBF0
    // NB: retail does NOT touch m_DisabledImages here -- 7 static lists + 8
    // locked lists = 15 calls at 0x180151890.
}

//=============================================================================
// CMFCToolBar::UpdateTooltips
//=============================================================================
// Retail (0x180159ce0) posts tooltip-add messages to m_pToolTip for every
// button with text; the tooltip manager and button walk are unmodeled.
// Symbol: ?UpdateTooltips@CMFCToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateTooltips_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially -- retail (0x180159ce0) walks
    // m_Buttons and drives m_pToolTip via TTM_ADDTOOL; not modeled.
}

//=============================================================================
// CMFCToolBar::WrapToolBar
//=============================================================================
// Retail (0x180155270) wraps buttons across rows using CalcLayout and a
// "wrap" font; the layout machinery is unmodeled.
// Symbol: ?WrapToolBar@CMFCToolBar@@QEAAHHHPEAVCDC@@HH@Z
extern "C" int MS_ABI impl__WrapToolBar_CMFCToolBar__QEAAHHHPEAVCDC__HH_Z(
    CMFCToolBar* pThis, int cx, int cy, CDC* pDC, int nMaxRows, int nMaxCols)
{
    (void)cx; (void)cy; (void)pDC; (void)nMaxRows; (void)nMaxCols;
    if (!pThis) return 0;
    // TODO(clean-room): transcribed partially -- retail wrap layout (0x180155270)
    // is not modeled.
    return 0;
}

//=============================================================================
// CMFCToolBar::accDoDefaultAction (IAccessible)
//=============================================================================
// Retail (0x18015a4a0): validate vt==VT_I4, resolve the button by child id,
// then for m_nID outside {0, 0xFFFFFFFF} run the button's OnClick virtual and
// post WM_COMMAND to the frame when it returns 0; for the two special ids call
// vslot 0x50.
// The child-id resolution (AccGetButtonByChildId, 0x18015ba90) numbers
// buttons 1-based among those passing the button vslot 0x188 gate; with that
// gate unmodeled the child id maps to GetButton(nChild - 1).
// Symbol: ?accDoDefaultAction@CMFCToolBar@@MEAAJUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accDoDefaultAction_CMFCToolBar__MEAAJUtagVARIANT___Z(
    CMFCToolBar* pThis, VARIANT varChild)
{
    if (!pThis) return S_OK;
    if (varChild.vt != 3 /*VT_I4*/) return E_INVALIDARG;

    const LONG nChild = varChild.lVal;
    if (nChild == 0) return S_OK;

    // Retail resolves the button through AccGetButtonByChildId (0x18015ba90);
    // child ids are 1-based, so index = nChild - 1.
    CMFCToolBarButton* pButton = pThis->GetButton(static_cast<int>(nChild - 1));
    if (!pButton) return S_OK;

    // Retail (0x18015a4db..): `dec m_nID; cmp $0xFFFFFFFD; ja` — the vslot-0x50
    // path is taken only for m_nID == 0 or m_nID == 0xFFFFFFFF; every other id
    // runs the OnClick + WM_COMMAND path.
    if (pButton->m_nID != 0 && pButton->m_nID != 0xFFFFFFFFu) {
        // TODO(clean-room): retail invokes the button OnClick virtual (vslot
        // 0x58) and posts WM_COMMAND (0x111) only when it returns 0; the
        // virtual is not modeled.  Retail resolves the owner as
        // m_pWndParent@0xA0, falling back to GetParent(m_hWnd); m_pWndParent
        // is not modeled either, so the fallback is used here.
        HWND hOwner = ::GetParent(pThis->GetSafeHwnd());
        if (hOwner) {
            ::SendMessageW(hOwner, WM_COMMAND, pButton->m_nID, 0);
        }
    } else {
        // TODO(clean-room): retail calls button vslot 0x50; not modeled.
    }
    return S_OK;
}

//=============================================================================
// CMFCToolBar::accHitTest (IAccessible)
//=============================================================================
// Retail (0x18015a250): null pVarOut -> E_INVALIDARG; ScreenToClient the point,
// walk the buttons, and on the first m_rect hit store the 1-based child id.
// Symbol: ?accHitTest@CMFCToolBar@@MEAAJJJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accHitTest_CMFCToolBar__MEAAJJJPEAUtagVARIANT___Z(
    CMFCToolBar* pThis, long x, long y, VARIANT* pVarOut)
{
    if (!pThis) return S_OK;
    if (!pVarOut) return E_INVALIDARG;

    pVarOut->vt = 3;   // VT_I4
    pVarOut->lVal = 0;

    POINT pt;
    pt.x = x;
    pt.y = y;
    if (pThis->GetSafeHwnd()) {
        ::ScreenToClient(pThis->GetSafeHwnd(), &pt);
    }

    const int nCount = pThis->GetCount();
    for (int i = 0; i < nCount; ++i) {
        CMFCToolBarButton* pButton = pThis->GetButton(i);
        if (!pButton) continue;
        // TODO(clean-room): retail first calls the button's vslot 0x188
        // (rect/visibility check) and only tests m_rect when it returns > 0.
        if (pButton->m_rect.PtInRect(CPoint(pt))) {
            pVarOut->lVal = i + 1;   // 1-based child id
            // TODO(clean-room): retail then calls the button's vslot 0x180 to
            // fill accessibility data; not modeled.
            return S_OK;
        }
    }
    return S_OK;
}

//=============================================================================
// CMFCToolBar::accNavigate (IAccessible)
//=============================================================================
// Retail (0x18015a3e0), fully transcribed.  NAVDIR ids: LEFT=3, RIGHT=4,
// NEXT=5, PREVIOUS=6, FIRSTCHILD=7, LASTCHILD=8 (oleacc.h).  LEFT shares a
// body with PREVIOUS, RIGHT with NEXT; out-of-range navigation clears vt to
// VT_EMPTY and returns S_FALSE; FIRSTCHILD/LASTCHILD reject a non-zero start
// child with S_FALSE.
// Symbol: ?accNavigate@CMFCToolBar@@MEAAJJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__accNavigate_CMFCToolBar__MEAAJJUtagVARIANT__PEAU2__Z(
    CMFCToolBar* pThis, long navDir, VARIANT varStart, VARIANT* pvarEnd)
{
    if (!pvarEnd) return E_INVALIDARG;

    pvarEnd->vt = 0;   // VT_EMPTY
    if (varStart.vt != 3 /*VT_I4*/) return E_INVALIDARG;

    const long nCount = pThis ? static_cast<long>(pThis->GetCount()) : 0;

    switch (navDir) {
    case 3:  // NAVDIR_LEFT
    case 6:  // NAVDIR_PREVIOUS
        {
            const long n = varStart.lVal;
            if (n == 0) return S_FALSE;
            const long next = n - 1;
            pvarEnd->vt = 3;
            pvarEnd->lVal = next;
            if (next > 0) return S_OK;
            pvarEnd->vt = 0;   // out of range -> empty + S_FALSE
        }
        return S_FALSE;

    case 4:  // NAVDIR_RIGHT
    case 5:  // NAVDIR_NEXT
        {
            const long n = varStart.lVal;
            if (n == 0) return S_FALSE;
            const long next = n + 1;
            pvarEnd->vt = 3;
            pvarEnd->lVal = next;
            if (next <= nCount) return S_OK;
            pvarEnd->vt = 0;   // out of range -> empty + S_FALSE
        }
        return S_FALSE;

    case 7:  // NAVDIR_FIRSTCHILD
        if (varStart.lVal != 0) return S_FALSE;
        pvarEnd->vt = 3;
        pvarEnd->lVal = 1;
        return S_OK;

    case 8:  // NAVDIR_LASTCHILD
        if (varStart.lVal != 0) return S_FALSE;
        pvarEnd->vt = 3;
        pvarEnd->lVal = nCount;
        return S_OK;

    default:
        // Retail (0x18015a488) returns S_FALSE for any other navDir, leaving
        // pvarEnd->vt at VT_EMPTY (set at entry).
        return S_FALSE;
    }
}

//=============================================================================
// CMFCToolBar::get_accChild (IAccessible)
//=============================================================================
// Retail (0x18015a350): null ppDispChild -> E_INVALIDARG; otherwise zero the
// out-param and return S_FALSE unless the child resolves through the RTTI and
// IAccessibleFromWindow plumbing, which is unmodeled.
// Symbol: ?get_accChild@CMFCToolBar@@MEAAJUtagVARIANT@@PEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accChild_CMFCToolBar__MEAAJUtagVARIANT__PEAPEAUIDispatch___Z(
    CMFCToolBar* pThis, VARIANT varChild, IDispatch** ppDispChild)
{
    if (!pThis) return S_FALSE;
    if (!ppDispChild) return E_INVALIDARG;
    *ppDispChild = nullptr;

    if (varChild.vt != 3 /*VT_I4*/) return S_FALSE;
    if (varChild.lVal == 0) return S_FALSE;

    // TODO(clean-room): retail resolves the button, checks IsKindOf(
    // CMFCToolBarButton) and calls IAccessibleFromWindow on its host window
    // (0x1803e9148); not modeled, so children are reported as not accessible.
    return S_FALSE;
}

//=============================================================================
// CMFCToolBar::get_accChildCount (IAccessible)
//=============================================================================
// Retail (0x18015a320), fully transcribed: *pCountChildren = GetCount().
// Symbol: ?get_accChildCount@CMFCToolBar@@MEAAJPEAJ@Z
extern "C" long MS_ABI impl__get_accChildCount_CMFCToolBar__MEAAJPEAJ_Z(
    CMFCToolBar* pThis, long* pCountChildren)
{
    if (!pThis) return S_OK;
    if (!pCountChildren) return E_INVALIDARG;
    *pCountChildren = static_cast<long>(pThis->GetCount());
    return S_OK;
}
