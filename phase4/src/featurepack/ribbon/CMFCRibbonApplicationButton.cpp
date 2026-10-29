// CMFCRibbonApplicationButton exports. The public OpenMFC headers do not
// declare this derived class, so this translation unit keeps its receiver
// opaque and records only retail-verified offsets needed by the bodies below.

#include <windows.h>

#include <cstddef>
#include <cstdint>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

class CDC;
class CRect;
class CWnd;
class CMFCRibbonButton;
class CMFCRibbonBar;
class CMFCToolBarImages;
struct CAccessibilityData;
struct IDispatch;

// Sibling exports used below. Their definitions were verified in:
// CMFCToolBarImages.cpp, CMFCRibbonButton.cpp, CMFCRibbonBar.cpp, and
// core/window/CWndHandleMap.cpp, respectively.
extern "C" void MS_ABI impl__Clear_CMFCToolBarImages__QEAAXXZ(CMFCToolBarImages* pThis);
extern "C" int MS_ABI impl__LoadStr_CMFCToolBarImages__QEAAHPEB_WPEAUHINSTANCE____H_Z(
    CMFCToolBarImages* pThis, const wchar_t* name, HINSTANCE hInst, int bAdd);
extern "C" void MS_ABI impl__SetSingleImage_CMFCToolBarImages__QEAAXXZ(CMFCToolBarImages* pThis);
extern "C" int MS_ABI impl__AddImage_CMFCToolBarImages__QEAAHPEAUHBITMAP____H_Z(
    CMFCToolBarImages* pThis, HBITMAP hBitmap, int bSet);
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    CMFCRibbonButton* pThis, CWnd* pWndParent, CAccessibilityData& data);
extern "C" IDispatch* MS_ABI impl__GetAccessibleDispatch_CMFCRibbonBar__QEAAPEAUIDispatch__XZ(
    CMFCRibbonBar* pThis);

namespace {
// CMFCRibbonApplicationButton::CreateObject allocates 0x5a0 bytes in the
// retail body at RVA 0xd88e0 (mfc140u). The embedded image objects begin at
// +0x270 and +0x408 in DrawImage at RVA 0xd8c60 (mfc140u) and SetImage at
// RVA 0xd9570 (mfc140, ANSI twin). The +0xd8 bar pointer and +0x108 opaque pointer are read or
// written by the inspected application-button methods.
struct RibbonApplicationButtonOffsetProbe {
    std::byte _toText[0xa0];
    std::byte m_strText[0x8];
    std::byte _toRibbonBar[0x30];
    void* m_pRibbonBar;
    std::byte _toOpaqueMenuPointer[0x28];
    void* m_opaqueMenuPointer;
    std::byte _toImage[0x160];
    std::byte m_Image[0x198];
    std::byte m_ImageWindows7[0x198];
};
static_assert(offsetof(RibbonApplicationButtonOffsetProbe, m_strText) == 0xa0,
              "Application-button text offset read by SetACCData");
static_assert(offsetof(RibbonApplicationButtonOffsetProbe, m_pRibbonBar) == 0xd8,
              "Application-button parent bar offset read by retail methods");
static_assert(offsetof(RibbonApplicationButtonOffsetProbe, m_opaqueMenuPointer) == 0x108,
              "Application-button opaque menu pointer written by ShowMainMenu");
static_assert(offsetof(RibbonApplicationButtonOffsetProbe, m_Image) == 0x270,
              "CMFCRibbonApplicationButton::m_Image");
static_assert(offsetof(RibbonApplicationButtonOffsetProbe, m_ImageWindows7) == 0x408,
              "CMFCRibbonApplicationButton::m_ImageWindows7");
static_assert(sizeof(RibbonApplicationButtonOffsetProbe) == 0x5a0,
              "Retail CMFCRibbonApplicationButton allocation size");

struct ToolBarImagesOffsetProbe {
    std::byte _toImageWell[0xa0];
    void* m_hbmImageWell;
};
static_assert(offsetof(ToolBarImagesOffsetProbe, m_hbmImageWell) == 0xa0,
              "CMFCToolBarImages::m_hbmImageWell offset used by retail setters");

struct CWndHandleOffsetProbe {
    std::byte _toHwnd[0x40];
    HWND m_hWnd;
};
static_assert(offsetof(CWndHandleOffsetProbe, m_hWnd) == 0x40,
              "CWnd::m_hWnd offset from include/openmfc/afxwin.h");

struct AccessibilityDataOffsetProbe {
    std::byte _toRole[0x30];
    std::uint32_t m_nAccRole;
    std::int32_t m_bAccState;
};
static_assert(offsetof(AccessibilityDataOffsetProbe, m_nAccRole) == 0x30,
              "CAccessibilityData::m_nAccRole offset from include/openmfc/afxmfc.h");
static_assert(offsetof(AccessibilityDataOffsetProbe, m_bAccState) == 0x34,
              "CAccessibilityData::m_bAccState offset from include/openmfc/afxmfc.h");

CMFCToolBarImages* ImagesAt(void* pThis, std::size_t offset) {
    return reinterpret_cast<CMFCToolBarImages*>(static_cast<std::byte*>(pThis) + offset);
}

bool HasImageWell(CMFCToolBarImages* images) {
    return *reinterpret_cast<void**>(reinterpret_cast<std::byte*>(images) + 0xa0) != nullptr;
}

void SetApplicationButtonImageResource(void* pThis, std::size_t offset, UINT resourceId) {
    if (pThis == nullptr) return;

    CMFCToolBarImages* images = ImagesAt(pThis, offset);
    if (HasImageWell(images)) {
        impl__Clear_CMFCToolBarImages__QEAAXXZ(images);
    }

    // Retail passes the low WORD as MAKEINTRESOURCE and ignores LoadStr's
    // result, then calls SetSingleImage regardless of whether loading worked.
    const auto* resourceName = reinterpret_cast<const wchar_t*>(
        static_cast<std::uintptr_t>(resourceId & 0xffffu));
    (void)impl__LoadStr_CMFCToolBarImages__QEAAHPEB_WPEAUHINSTANCE____H_Z(
        images, resourceName, nullptr, TRUE);
    impl__SetSingleImage_CMFCToolBarImages__QEAAXXZ(images);
}

void SetApplicationButtonBitmap(void* pThis, std::size_t offset, HBITMAP bitmap) {
    if (pThis == nullptr) return;

    CMFCToolBarImages* images = ImagesAt(pThis, offset);
    if (HasImageWell(images)) {
        impl__Clear_CMFCToolBarImages__QEAAXXZ(images);
    }
    if (bitmap != nullptr) {
        (void)impl__AddImage_CMFCToolBarImages__QEAAHPEAUHBITMAP____H_Z(
            images, bitmap, TRUE);
        impl__SetSingleImage_CMFCToolBarImages__QEAAXXZ(images);
    }
}
} // namespace

// The retail factory allocates 0x5a0 bytes, runs the CMFCRibbonButton
// constructor, installs the derived vftable, and constructs both embedded
// CMFCToolBarImages objects (RVA 0xd88e0 (mfc140u)). This file has no verified
// application-button vftable/placement-construction path, so returning a base
// button or a partially initialized allocation would be incorrect.
// Symbol: ?CreateObject@CMFCRibbonApplicationButton@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCRibbonApplicationButton__SAPEAVCObject__XZ() {
    return nullptr;
}

// DrawImage at RVA 0xd8c60 (mfc140u) selects between embedded images using
// the +0x4a8 flag, adjusts the rectangle for image scaling, consults ribbon/
// global state, and uses toolbar-image drawing helpers. Drawing state and members
// are not modeled here, so this stays a documented stub.
// Symbol: ?DrawImage@CMFCRibbonApplicationButton@@MEAAXPEAVCDC@@W4RibbonImageType@CMFCRibbonBaseElement@@VCRect@@@Z
extern "C" void MS_ABI impl__DrawImage_CMFCRibbonApplicationButton__MEAAXPEAVCDC__W4RibbonImageType_CMFCRibbonBaseElement__VCRect___Z(
    void* /*pThis*/, CDC* /*pDC*/, int /*imageType*/, const CRect* /*rectImage*/) {
}

// The ANSI twin's body at RVA 0xd9e40 (mfc140; no Unicode symbol-map entry)
// clears ribbon key state conditionally, then selects the base-key or popup
// path and may dispatch a menu message. The popup/key state is not modeled.
// Symbol: ?OnKey@CMFCRibbonApplicationButton@@MEAAHH@Z
extern "C" int MS_ABI impl__OnKey_CMFCRibbonApplicationButton__MEAAHH_Z(
    void* /*pThis*/, int /*bIsMenuKey*/) {
    return 0;
}

// The ANSI twin at RVA 0xd97f0 (mfc140; no Unicode symbol-map entry) gets the
// parent of the ribbon bar HWND, wraps it with CWnd::FromHandle, and posts
// WM_SYSCOMMAND/SC_CLOSE. The IAT slots were resolved as USER32 GetParent and
// PostMessageA; UNICODE selects PostMessageW in this build.
// Symbol: ?OnLButtonDblClk@CMFCRibbonApplicationButton@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCRibbonApplicationButton__MEAAXVCPoint___Z(
    void* pThis, long long /*point*/) {
    auto* ribbonBar = *reinterpret_cast<void**>(static_cast<std::byte*>(pThis) + 0xd8);
    HWND ribbonHwnd = *reinterpret_cast<HWND*>(static_cast<std::byte*>(ribbonBar) + 0x40);
    CWnd* parent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(ribbonHwnd));
    HWND parentHwnd = *reinterpret_cast<HWND*>(reinterpret_cast<std::byte*>(parent) + 0x40);
    ::PostMessage(parentHwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
}

// The ANSI twin at RVA 0xd9830 (mfc140; no Unicode symbol-map entry) tries the
// main-menu path when the parent bar has a main panel and otherwise delegates
// to the base button handler. The main-panel/popup state is not modeled.
// Symbol: ?OnLButtonDown@CMFCRibbonApplicationButton@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCRibbonApplicationButton__MEAAXVCPoint___Z(
    void* /*pThis*/, long long /*point*/) {
}

// The ANSI twin at RVA 0xd9ed0 (mfc140; no Unicode symbol-map entry) calls the
// base SetACCData thunk and always writes role 0x3a and state 0x40100000. It
// also sets the accessible name from the button text or a fallback string;
// that CString/fallback path is not represented by the current shadow model.
// The two scalar fields and the base call are transcribed; name population is
// an explicit deviation until that string path is modeled.
// Symbol: ?SetACCData@CMFCRibbonApplicationButton@@MEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonApplicationButton__MEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, CWnd* pWndParent, CAccessibilityData& data) {
    if (pThis == nullptr) return FALSE;
    (void)impl__SetACCData_CMFCRibbonButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
        reinterpret_cast<CMFCRibbonButton*>(pThis), pWndParent, data);
    auto* bytes = reinterpret_cast<std::byte*>(&data);
    *reinterpret_cast<std::uint32_t*>(bytes + 0x30) = 0x3a;
    *reinterpret_cast<std::int32_t*>(bytes + 0x34) = 0x40100000;
    return TRUE;
}

// SetImage(UINT) at RVA 0xd9570 (mfc140; no Unicode symbol-map entry) clears an
// existing image well, loads the low-WORD resource via LoadStr, and marks it
// single-image. Retail's conditional ConvertTo32Bits tail also reads global
// image/color settings and base-button state not modeled here; that tail is an
// explicit deviation and is not guessed.
// Symbol: ?SetImage@CMFCRibbonApplicationButton@@QEAAXI@Z
extern "C" void MS_ABI impl__SetImage_CMFCRibbonApplicationButton__QEAAXI_Z(
    void* pThis, unsigned int resourceId) {
    SetApplicationButtonImageResource(pThis, 0x270, resourceId);
}

// SetImage(HBITMAP) at RVA 0xd9610 (mfc140; no Unicode symbol-map entry) clears
// an existing well, adds a non-null bitmap, and marks it single-image. The
// conditional ConvertTo32Bits tail is omitted for the same unmodeled settings
// described above.
// Symbol: ?SetImage@CMFCRibbonApplicationButton@@QEAAXPEAUHBITMAP__@@@Z
extern "C" void MS_ABI impl__SetImage_CMFCRibbonApplicationButton__QEAAXPEAUHBITMAP_____Z(
    void* pThis, HBITMAP bitmap) {
    SetApplicationButtonBitmap(pThis, 0x270, bitmap);
}

// SetWindows7Image(UINT) at RVA 0xd96b0 (mfc140; no Unicode symbol-map entry)
// follows the same resource-load/SetSingleImage sequence at the second embedded
// image object (+0x408). Its conditional ConvertTo32Bits tail is not modeled.
// Symbol: ?SetWindows7Image@CMFCRibbonApplicationButton@@QEAAXI@Z
extern "C" void MS_ABI impl__SetWindows7Image_CMFCRibbonApplicationButton__QEAAXI_Z(
    void* pThis, unsigned int resourceId) {
    SetApplicationButtonImageResource(pThis, 0x408, resourceId);
}

// SetWindows7Image(HBITMAP) at RVA 0xd9750 (mfc140; no Unicode symbol-map entry)
// follows the same bitmap-add sequence at the second embedded image object
// (+0x408). Its conditional ConvertTo32Bits tail is not modeled.
// Symbol: ?SetWindows7Image@CMFCRibbonApplicationButton@@QEAAXPEAUHBITMAP__@@@Z
extern "C" void MS_ABI impl__SetWindows7Image_CMFCRibbonApplicationButton__QEAAXPEAUHBITMAP_____Z(
    void* pThis, HBITMAP bitmap) {
    SetApplicationButtonBitmap(pThis, 0x408, bitmap);
}

// ShowMainMenu (ANSI twin, RVA 0xd9bf0 (mfc140); no Unicode symbol-map entry)
// creates and positions the popup menu, clones menu items, and stores popup
// state. Those menu/panel structures and their lifecycle are not modeled.
// Symbol: ?ShowMainMenu@CMFCRibbonApplicationButton@@IEAAHXZ
extern "C" int MS_ABI impl__ShowMainMenu_CMFCRibbonApplicationButton__IEAAHXZ(
    void* /*pThis*/) {
    return 0;
}

// accNavigate at RVA 0xd9320 (mfc140u) sets the result VARIANT's vt to
// VT_EMPTY, requires VT_I4 input, a live parent-bar HWND, NAVDIR_NEXT/PREVIOUS,
// child ID 0, and a nonzero bar field at +0x14dc. Its success path sets
// VT_DISPATCH and queries
// the existing QAToolbar subobject at bar +0x1380 via CCmdTarget
// ExternalAddRef/GetInterface; it does not create a ribbon-menu object. This
// file does not model that embedded object's interface path, so keep it stubbed.
// Symbol: ?accNavigate@CMFCRibbonApplicationButton@@MEAAJJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__accNavigate_CMFCRibbonApplicationButton__MEAAJJUtagVARIANT__PEAU2__Z(
    void* /*pThis*/, long /*navDir*/, VARIANT /*varStart*/, VARIANT* /*pvarEnd*/) {
    return S_FALSE;
}

// get_accParent at RVA 0xd8470 (mfc140u) rejects a null output, clears it, and
// returns S_FALSE when no parent ribbon bar/window exists. With a valid bar
// HWND it gets the bar's accessible dispatch, assigns it only if non-null, and
// returns S_OK. GetAccessibleDispatch is called through its verified impl__
// thunk; OpenMFC currently models that helper as returning nullptr.
// Symbol: ?get_accParent@CMFCRibbonApplicationButton@@MEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accParent_CMFCRibbonApplicationButton__MEAAJPEAPEAUIDispatch___Z(
    void* pThis, IDispatch** ppdispParent) {
    if (ppdispParent == nullptr) return E_INVALIDARG;
    *ppdispParent = nullptr;

    auto* ribbonBar = *reinterpret_cast<void**>(static_cast<std::byte*>(pThis) + 0xd8);
    if (ribbonBar == nullptr) return S_FALSE;
    HWND ribbonHwnd = *reinterpret_cast<HWND*>(static_cast<std::byte*>(ribbonBar) + 0x40);
    if (ribbonHwnd == nullptr) return S_FALSE;

    *ppdispParent = impl__GetAccessibleDispatch_CMFCRibbonBar__QEAAPEAUIDispatch__XZ(
        reinterpret_cast<CMFCRibbonBar*>(ribbonBar));
    return S_OK;
}
