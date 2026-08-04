// COleControlSiteOrWnd — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp, ole_olectors_exports.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/OlecoreSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

























// Implemented in thunks.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetStyle@COleControlSiteOrWnd@@QEBAKXZ
// Symbol: ??0COleControlSiteOrWnd@@QEAA@PEAUHWND__@@H@Z
// COleControlSiteOrWnd::COleControlSiteOrWnd(HWND hWnd, BOOL bNeedFont)
extern "C" void* MS_ABI impl___0COleControlSiteOrWnd__QEAA_PEAUHWND____H_Z(void* hWnd, int bNeedFont) {
    (void)hWnd;
    (void)bNeedFont;
    return new COleControlSiteOrWnd();
}
// Symbol: ??0COleControlSiteOrWnd@@QEAA@PEAVCOleControlSite@@@Z
// COleControlSiteOrWnd::COleControlSiteOrWnd(COleControlSite* pSite)
extern "C" void* MS_ABI impl___0COleControlSiteOrWnd__QEAA_PEAVCOleControlSite___Z(COleControlSite* pSite) {
    auto* pThis = new COleControlSiteOrWnd();
    pThis->m_pSite = pSite;
    pThis->m_pWnd = nullptr;
    return pThis;
}
HWND COleControlSiteOrWnd::GetSafeHwnd() const {
    if (m_pSite) {
        return m_pSite->m_hWnd;
    }
    return m_pWnd ? m_pWnd->GetSafeHwnd() : nullptr;
}
DWORD COleControlSiteOrWnd::GetStyle() const {
    if (m_pSite) {
        return m_pSite->m_dwStyle;
    }
    HWND hWnd = GetSafeHwnd();
    if (!hWnd) {
        return 0;
    }
    return static_cast<DWORD>(GetWindowLongPtrW(hWnd, GWL_STYLE));
}
DWORD COleControlSiteOrWnd::GetExStyle() const {
    HWND hWnd = GetSafeHwnd();
    if (!hWnd) {
        return 0;
    }
    return static_cast<DWORD>(GetWindowLongPtrW(hWnd, GWL_EXSTYLE));
}
