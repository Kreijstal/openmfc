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
