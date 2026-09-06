// COleFrameHook — OpenMFC implementation.
// Sources: ole_olectors_exports.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

























// Symbol: ??0COleFrameHook@@QEAA@PEAVCFrameWnd@@PEAVCOleClientItem@@@Z
// COleFrameHook::COleFrameHook(CFrameWnd* pFrameWnd, COleClientItem* pItem)
extern "C" void* MS_ABI impl___0COleFrameHook__QEAA_PEAVCFrameWnd__PEAVCOleClientItem___Z(
    CFrameWnd* pFrameWnd, COleClientItem* pItem) {
    return new COleFrameHook(pFrameWnd, pItem);
}
COleFrameHook::COleFrameHook(CFrameWnd* pFrameWnd, COleClientItem* pItem)
    : m_pFrameWnd(pFrameWnd), m_pActiveItem(pItem), m_lpActiveUIWindow(nullptr) {
    memset(_oleframehook_padding, 0, sizeof(_oleframehook_padding));
}
COleFrameHook::~COleFrameHook() {
    m_pFrameWnd = nullptr;
    m_pActiveItem = nullptr;
    m_lpActiveUIWindow = nullptr;
}
BOOL COleFrameHook::OnDocActivate(BOOL bActivate) {
    return TRUE;
}
BOOL COleFrameHook::OnContextHelp() {
    return FALSE;
}
BOOL COleFrameHook::OnResizeBorder(LPCRECT lpRect, LPOLEINPLACEUIWINDOW lpUIWindow, BOOL bMainWindow) {
    return FALSE;
}
BOOL COleFrameHook::OnGetClipboardData(COleDataSource* pDataSource) {
    return FALSE;
}

// === Moved from ManualThunks.cpp ===
// Symbol: ?OnContextHelp@COleFrameHook@@UEAAHH@Z
extern "C" int MS_ABI impl__OnContextHelp_COleFrameHook__UEAAHH_Z(void* pThis, void* p0) {
    (void)pThis;
    (void)p0;
    return 0;
}

// Symbol: ?DoContextSensitiveHelp@COleFrameHook@@QEAAHH@Z
extern "C" int MS_ABI impl__DoContextSensitiveHelp_COleFrameHook__QEAAHH_Z(int p0) {
    return 0;
}

// Symbol: ?DoEnableModeless@COleFrameHook@@QEAAHH@Z
extern "C" int MS_ABI impl__DoEnableModeless_COleFrameHook__QEAAHH_Z(int p0) {
    return 0;
}

// Symbol: ?NotifyAllInPlace@COleFrameHook@@QEAAHHP81@EAAHH@Z@Z
extern "C" int MS_ABI impl__NotifyAllInPlace_COleFrameHook__QEAAHHP81_EAAHH_Z_Z(int p0, void* /*fnptr*/ p1) {
    return 0;
}

// Symbol: ?OnActivate@COleFrameHook@@UEAAXH@Z
extern "C" void MS_ABI impl__OnActivate_COleFrameHook__UEAAXH_Z(int p0) {}

// Symbol: ?OnEnableModeless@COleFrameHook@@UEAAXH@Z
extern "C" void MS_ABI impl__OnEnableModeless_COleFrameHook__UEAAXH_Z(int p0) {}

// Symbol: ?OnInitMenu@COleFrameHook@@UEAAXPEAVCMenu@@@Z
extern "C" void MS_ABI impl__OnInitMenu_COleFrameHook__UEAAXPEAVCMenu___Z(void* /*class*/* p0) {}

// Symbol: ?OnInitMenuPopup@COleFrameHook@@UEAAHPEAVCMenu@@HH@Z
extern "C" int MS_ABI impl__OnInitMenuPopup_COleFrameHook__UEAAHPEAVCMenu__HH_Z(void* /*class*/* p0, int p1, int p2) {
    return 0;
}

// Symbol: ?OnMenuSelect@COleFrameHook@@UEAAHIIPEAUHMENU__@@@Z
extern "C" int MS_ABI impl__OnMenuSelect_COleFrameHook__UEAAHIIPEAUHMENU_____Z(unsigned int p0, unsigned int p1, void* /*struct*/* p2) {
    return 0;
}

// Symbol: ?OnPaletteChanged@COleFrameHook@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnPaletteChanged_COleFrameHook__UEAAXPEAVCWnd___Z(void* /*class*/* p0) {}

// Symbol: ?OnPreTranslateMessage@COleFrameHook@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__OnPreTranslateMessage_COleFrameHook__UEAAHPEAUtagMSG___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?OnQueryNewPalette@COleFrameHook@@UEAAHXZ
extern "C" int MS_ABI impl__OnQueryNewPalette_COleFrameHook__UEAAHXZ() {
    return 0;
}

// Symbol: ?OnRecalcLayout@COleFrameHook@@UEAAXXZ
extern "C" void MS_ABI impl__OnRecalcLayout_COleFrameHook__UEAAXXZ() {}

// Symbol: ?OnUpdateFrameTitle@COleFrameHook@@UEAAHXZ
extern "C" int MS_ABI impl__OnUpdateFrameTitle_COleFrameHook__UEAAHXZ() {
    return 0;
}

// Symbol: ?ContextSensitiveHelp@XOleInPlaceFrame@COleFrameHook@@UEAAJH@Z
extern "C" long MS_ABI impl__ContextSensitiveHelp_XOleInPlaceFrame_COleFrameHook__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?EnableModeless@XOleInPlaceFrame@COleFrameHook@@UEAAJH@Z
extern "C" long MS_ABI impl__EnableModeless_XOleInPlaceFrame_COleFrameHook__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?Exec@XOleCommandTarget@COleFrameHook@@UEAAJPEBU_GUID@@KKPEAUtagVARIANT@@1@Z
extern "C" long MS_ABI impl__Exec_XOleCommandTarget_COleFrameHook__UEAAJPEBU_GUID__KKPEAUtagVARIANT__1_Z(const void* /*struct*/* p0, unsigned long p1, unsigned long p2, void* /*struct*/* p3, unsigned long p4) {
    return 0;
}

// Symbol: ?GetBorder@XOleInPlaceFrame@COleFrameHook@@UEAAJPEAUtagRECT@@@Z
extern "C" long MS_ABI impl__GetBorder_XOleInPlaceFrame_COleFrameHook__UEAAJPEAUtagRECT___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?GetWindow@XOleInPlaceFrame@COleFrameHook@@UEAAJPEAPEAUHWND__@@@Z
extern "C" long MS_ABI impl__GetWindow_XOleInPlaceFrame_COleFrameHook__UEAAJPEAPEAUHWND_____Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?InsertMenus@XOleInPlaceFrame@COleFrameHook@@UEAAJPEAUHMENU__@@PEAUtagOleMenuGroupWidths@@@Z
extern "C" long MS_ABI impl__InsertMenus_XOleInPlaceFrame_COleFrameHook__UEAAJPEAUHMENU____PEAUtagOleMenuGroupWidths___Z(void* /*struct*/* p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?QueryStatus@XOleCommandTarget@COleFrameHook@@UEAAJPEBU_GUID@@KQEAU_tagOLECMD@@PEAU_tagOLECMDTEXT@@@Z
extern "C" long MS_ABI impl__QueryStatus_XOleCommandTarget_COleFrameHook__UEAAJPEBU_GUID__KQEAU_tagOLECMD__PEAU_tagOLECMDTEXT___Z(const void* /*struct*/* p0, unsigned long p1, void* p2, unsigned char p3, void* /*struct*/* p4, void* /*struct*/* p5) {
    return 0;
}

// Symbol: ?RemoveMenus@XOleInPlaceFrame@COleFrameHook@@UEAAJPEAUHMENU__@@@Z
extern "C" long MS_ABI impl__RemoveMenus_XOleInPlaceFrame_COleFrameHook__UEAAJPEAUHMENU_____Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?RequestBorderSpace@XOleInPlaceFrame@COleFrameHook@@UEAAJPEBUtagRECT@@@Z
extern "C" long MS_ABI impl__RequestBorderSpace_XOleInPlaceFrame_COleFrameHook__UEAAJPEBUtagRECT___Z(const void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?SetActiveObject@XOleInPlaceFrame@COleFrameHook@@UEAAJPEAUIOleInPlaceActiveObject@@PEB_W@Z
extern "C" long MS_ABI impl__SetActiveObject_XOleInPlaceFrame_COleFrameHook__UEAAJPEAUIOleInPlaceActiveObject__PEB_W_Z(void* /*struct*/* p0, const wchar_t* p1) {
    return 0;
}

// Symbol: ?SetBorderSpace@XOleInPlaceFrame@COleFrameHook@@UEAAJPEBUtagRECT@@@Z
extern "C" long MS_ABI impl__SetBorderSpace_XOleInPlaceFrame_COleFrameHook__UEAAJPEBUtagRECT___Z(const void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?SetMenu@XOleInPlaceFrame@COleFrameHook@@UEAAJPEAUHMENU__@@PEAXPEAUHWND__@@@Z
extern "C" long MS_ABI impl__SetMenu_XOleInPlaceFrame_COleFrameHook__UEAAJPEAUHMENU____PEAXPEAUHWND_____Z(void* /*struct*/* p0, void* p1, void* /*struct*/* p2) {
    return 0;
}

// Symbol: ?SetStatusText@XOleInPlaceFrame@COleFrameHook@@UEAAJPEB_W@Z
extern "C" long MS_ABI impl__SetStatusText_XOleInPlaceFrame_COleFrameHook__UEAAJPEB_W_Z(const wchar_t* p0) {
    return 0;
}

// Symbol: ?TranslateAcceleratorW@XOleInPlaceFrame@COleFrameHook@@UEAAJPEAUtagMSG@@G@Z
extern "C" long MS_ABI impl__TranslateAcceleratorW_XOleInPlaceFrame_COleFrameHook__UEAAJPEAUtagMSG__G_Z(void* /*struct*/* p0, unsigned short p1) {
    return 0;
}
