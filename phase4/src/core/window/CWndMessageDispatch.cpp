// Base window message dispatch, including Direct2D paint and resize handling.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxlayout.h"
#include "detail/WindowMessageSupport.h"
#include "detail/WindowInputSupport.h"
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
    WNDPROC procedure=pThis->m_pfnSuper;
    if(!procedure) {
        using Getter=WNDPROC* (MS_ABI*)(void*);
        procedure=*openmfc::detail::WindowSlot<Getter>(pThis,65)(pThis);
    }
    return procedure ? CallWindowProcW(procedure,pThis->m_hWnd,message,wParam,lParam)
                     : ::DefWindowProcW(pThis->m_hWnd,message,wParam,lParam);
}

// Symbol: ?WindowProc@CWnd@@MEAA_JI_K_J@Z
extern "C" LRESULT MS_ABI impl__WindowProc_CWnd__MEAA_JI_K_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    using Handler=BOOL (MS_ABI*)(void*,UINT,WPARAM,LPARAM,LRESULT*);
    if (openmfc::detail::WindowSlot<Handler>(pThis,72)(pThis,message,wParam,lParam,&result)) {
        return result;
    }
    using Default=LRESULT (MS_ABI*)(void*,UINT,WPARAM,LPARAM);
    return openmfc::detail::WindowSlot<Default>(pThis,73)(pThis,message,wParam,lParam);
}

// Symbol: ?OnWndMsg@CWnd@@MEAAHI_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnWndMsg_CWnd__MEAAHI_K_JPEA_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
    if (!pThis) {
        return FALSE;
    }
    if(message==WM_COMMAND) {
        using Command=BOOL (MS_ABI*)(void*,WPARAM,LPARAM);
        if(!openmfc::detail::WindowSlot<Command>(pThis,63)(pThis,wParam,lParam))return FALSE;
        if(pResult)*pResult=TRUE;return TRUE;
    }
    if(message==WM_NOTIFY) {
        auto* notify=reinterpret_cast<NMHDR*>(lParam);
        if(!notify || !notify->hwndFrom)return FALSE;
        LRESULT result=0;using Notify=BOOL (MS_ABI*)(void*,WPARAM,LPARAM,LRESULT*);
        if(!openmfc::detail::WindowSlot<Notify>(pThis,64)(pThis,wParam,lParam,&result))return FALSE;
        if(pResult)*pResult=result;return TRUE;
    }
    if(message==WM_CREATE && pThis->m_pDynamicLayout) {
        if(pThis->m_hWnd) {
            pThis->m_pDynamicLayout->Create(pThis);pThis->InitDynamicLayout();
        } else pThis->EnableDynamicLayout(FALSE);
    }
    if(message==WM_SIZE) {
        using Resize=void (MS_ABI*)(void*);
        openmfc::detail::WindowSlot<Resize>(pThis,84)(pThis);
        if(pThis->m_pRenderTarget)pThis->ResizeD2DRenderTarget(LOWORD(lParam),HIWORD(lParam));
    }
    if(message==WM_PAINT && pThis->m_pRenderTarget && pThis->DoD2DPaint()) {
        if(pResult)*pResult=TRUE;return TRUE;
    }
    if(openmfc::detail::DispatchWindowMessageMap(pThis,message,wParam,lParam,pResult))return TRUE;
    if(message>=WM_CTLCOLORMSGBOX && message<=WM_CTLCOLORSTATIC) {
        struct Color { HWND window;HDC dc;UINT type; } color{reinterpret_cast<HWND>(lParam),reinterpret_cast<HDC>(wParam),message-WM_CTLCOLORMSGBOX};
        return openmfc::detail::DispatchWindowMessageMap(pThis,0x19,0,reinterpret_cast<LPARAM>(&color),pResult);
    }
    switch (message) {
    case WM_TOUCH:
        if(pResult)*pResult=impl__OnTouchMessage_CWnd__IEAA_J_K_J_Z(pThis,wParam,lParam);
        else impl__OnTouchMessage_CWnd__IEAA_J_K_J_Z(pThis,wParam,lParam);
        return TRUE;
    case WM_GESTURE:
        if(pResult)*pResult=impl__OnGesture_CWnd__IEAA_J_K_J_Z(pThis,wParam,lParam);
        else impl__OnGesture_CWnd__IEAA_J_K_J_Z(pThis,wParam,lParam);
        return TRUE;
    case WM_TABLET_QUERYSYSTEMGESTURESTATUS:
        if(pResult)*pResult=impl__OnTabletQuerySystemGestureStatus_CWnd__IEAA_J_K_J_Z(pThis,wParam,lParam);
        else impl__OnTabletQuerySystemGestureStatus_CWnd__IEAA_J_K_J_Z(pThis,wParam,lParam);
        return TRUE;
    case WM_SIZE:
        return FALSE;
    case WM_GETOBJECT:
        if (pResult) *pResult = static_cast<LRESULT>(pThis->OnGetObject(wParam, lParam));
        else pThis->OnGetObject(wParam, lParam);
        return TRUE;
    default:
        return FALSE;
    }
}
