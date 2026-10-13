// Base window message dispatch, including Direct2D paint and resize handling.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" int MS_ABI impl__OnCommand_CWnd__MEAAH_K_J_Z(CWnd*, WPARAM, LPARAM);
extern "C" int MS_ABI impl__OnNotify_CWnd__MEAAH_K_JPEA_J_Z(CWnd*, WPARAM, LPARAM, LRESULT*);
extern "C" int MS_ABI impl__OnWndMsg_CWnd__MEAAHI_K_JPEA_J_Z(CWnd*, UINT, WPARAM, LPARAM, LRESULT*);

// Symbol: ?DefWindowProcW@CWnd@@MEAA_JI_K_J@Z
extern "C" LRESULT MS_ABI impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (pThis && pThis->m_hWnd) {
        return ::DefWindowProcW(pThis->m_hWnd, message, wParam, lParam);
    }
    return 0;
}

// Symbol: ?WindowProc@CWnd@@MEAA_JI_K_J@Z
extern "C" LRESULT MS_ABI impl__WindowProc_CWnd__MEAA_JI_K_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (!pThis || !pThis->m_hWnd) {
        return 0;
    }

    LRESULT result = 0;
    if (impl__OnWndMsg_CWnd__MEAAHI_K_JPEA_J_Z(pThis, message, wParam, lParam, &result)) {
        return result;
    }
    return impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(pThis, message, wParam, lParam);
}

// Symbol: ?OnWndMsg@CWnd@@MEAAHI_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnWndMsg_CWnd__MEAAHI_K_JPEA_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
    if (!pThis) {
        return FALSE;
    }
    if (pResult) {
        *pResult = 0;
    }
    // Objects constructed through the MSVC import library do not have a
    // cross-compiler-compatible CWnd vtable yet.  Dispatch the base handlers
    // directly instead of making a virtual call through an invalid vptr.
    switch (message) {
    case WM_PAINT:
        return pThis->m_pRenderTarget && pThis->DoD2DPaint();
    case WM_SIZE:
        if (pThis->m_pRenderTarget) pThis->ResizeD2DRenderTarget(LOWORD(lParam), HIWORD(lParam));
        return FALSE;
    case WM_COMMAND:
        return impl__OnCommand_CWnd__MEAAH_K_J_Z(pThis, wParam, lParam);
    case WM_NOTIFY:
        return impl__OnNotify_CWnd__MEAAH_K_JPEA_J_Z(pThis, wParam, lParam, pResult);
    default:
        return FALSE;
    }
}
