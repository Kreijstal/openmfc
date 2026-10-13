// Native MFC message-map argument decoding (mfc140.dll OnWndMsg, 0x28b630).
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
#include <cstring>
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" {
CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND);
CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND);
CDC* MS_ABI impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z(HDC);
CMenu* MS_ABI impl__FromHandle_CMenu__SAPEAV1_PEAUHMENU_____Z(HMENU);
CGdiObject* MS_ABI impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z(HGDIOBJ);
void MS_ABI impl__AfxLockGlobals__YAXH_Z(int);
void MS_ABI impl__AfxUnlockGlobals__YAXH_Z(int);
}
namespace openmfc { namespace detail {
namespace {
template<class Result,class... Args> Result Call(void* method,CWnd* self,Args... args) {
    return reinterpret_cast<Result (MS_ABI*)(CWnd*,Args...)>(method)(self,args...);
}
CWnd* Window(UINT_PTR handle) { return impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(reinterpret_cast<HWND>(handle)); }
CMenu* Menu(UINT_PTR handle) { return impl__FromHandle_CMenu__SAPEAV1_PEAUHMENU_____Z(reinterpret_cast<HMENU>(handle)); }
CDC* DC(UINT_PTR handle) { return impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z(reinterpret_cast<HDC>(handle)); }
CPoint Point(LPARAM value) { return CPoint(static_cast<short>(LOWORD(value)),static_cast<short>(HIWORD(value))); }
struct ControlColor { HWND window;HDC dc;UINT type; };
struct BorrowedDC {
    CDC value;
    explicit BorrowedDC(HDC dc) { value.m_hDC=dc; }
    ~BorrowedDC() { value.m_hDC=nullptr; }
};
struct BorrowedWindow {
    CWnd value;
    explicit BorrowedWindow(HWND window) { value.m_hWnd=window; }
    ~BorrowedWindow() { value.m_hWnd=nullptr; }
};
struct CacheEntry { UINT message=0;const AFX_MSGMAP_ENTRY* entry=nullptr;const AFX_MSGMAP* map=nullptr; };
CacheEntry cache[512];
struct CacheLock {
    CacheLock() { impl__AfxLockGlobals__YAXH_Z(7); }
    ~CacheLock() { impl__AfxUnlockGlobals__YAXH_Z(7); }
};
const AFX_MSGMAP_ENTRY* Find(const AFX_MSGMAP* original,UINT message) {
    CacheLock lock;
    auto& cached=cache[(reinterpret_cast<UINT_PTR>(original)^message)&511];
    if(cached.map==original && cached.message==message)return cached.entry;
    cached={message,nullptr,original};
    for(auto* map=original;map && map->pfnGetBaseMap;map=map->pfnGetBaseMap()) {
        for(auto* entry=map->lpEntries;entry && entry->nSig;++entry) {
            if(entry->nCode || entry->nID)continue;
            const bool match=message<0xc000 ? entry->nMessage==message :
                entry->nMessage==0xc000 && *reinterpret_cast<const UINT*>(entry->nSig)==message;
            if(match)return cached.entry=entry;
        }
    }
    return nullptr;
}
}
void ResetWindowMessageCache() {
    CacheLock lock;
    for(auto& entry:cache)entry={};
}
BOOL DispatchWindowMessageMap(CWnd* self,UINT message,WPARAM w,LPARAM l,LRESULT* output) {
    using GetMap=const AFX_MSGMAP* (MS_ABI*)(const void*);
    const AFX_MSGMAP_ENTRY* found=Find(WindowSlot<GetMap>(self,12)(self),message);
    if(!found)return FALSE;
    void* method=nullptr;std::memcpy(&method,&found->pfn,sizeof(method));
    LRESULT result=0;
    if(message>=0xc000)result=Call<LRESULT>(method,self,w,l);
    else switch(found->nSig) {
    case AfxSig_b_D_v:result=Call<BOOL>(method,self,DC(w));break;
    case AfxSig_b_b_v:case AfxSig_b_u_v:case AfxSig_i_u_v:result=Call<int>(method,self,static_cast<UINT>(w));break;
    case AfxSig_b_h_v:result=Call<BOOL>(method,self,reinterpret_cast<HANDLE>(w));break;
    case AfxSig_b_W_uu:case AfxSig_i_W_uu:result=Call<int>(method,self,Window(w),UINT(LOWORD(l)),UINT(HIWORD(l)));break;
    case AfxSig_b_W_COPYDATASTRUCT:result=Call<BOOL>(method,self,Window(w),reinterpret_cast<COPYDATASTRUCT*>(l));break;
    case AfxSig_b_v_HELPINFO:case AfxSig_i_v_s:case AfxSig_i_v_S:result=Call<int>(method,self,reinterpret_cast<void*>(l));break;
    case AfxSig_CTLCOLOR: {
        auto* color=reinterpret_cast<ControlColor*>(l);BorrowedDC dc(color->dc);BorrowedWindow child(color->window);
        CWnd* window=impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(color->window);
        result=reinterpret_cast<LRESULT>(Call<HBRUSH>(method,self,&dc.value,window ? window : &child.value,color->type));break;
    }
    case AfxSig_CTLCOLOR_REFLECT: {
        auto* color=reinterpret_cast<ControlColor*>(l);BorrowedDC dc(color->dc);
        result=reinterpret_cast<LRESULT>(Call<HBRUSH>(method,self,&dc.value,color->type));break;
    }
    case AfxSig_i_u_W_u:result=Call<int>(method,self,UINT(LOWORD(w)),Window(static_cast<UINT_PTR>(l)),UINT(HIWORD(w)));break;
    case AfxSig_i_uu_v:result=Call<int>(method,self,UINT(LOWORD(w)),UINT(HIWORD(w)));break;
    case AfxSig_l_w_l:result=Call<LRESULT>(method,self,w,l);break;
    case AfxSig_l_uu_M:result=Call<LRESULT>(method,self,UINT(LOWORD(w)),UINT(HIWORD(w)),Menu(static_cast<UINT_PTR>(l)));break;
    case AfxSig_v_b_h:case AfxSig_v_u_hkl:Call<void>(method,self,static_cast<UINT>(w),reinterpret_cast<HANDLE>(l));break;
    case AfxSig_v_h_v:Call<void>(method,self,reinterpret_cast<HANDLE>(w));break;
    case AfxSig_v_h_h:Call<void>(method,self,reinterpret_cast<HANDLE>(w),reinterpret_cast<HANDLE>(l));break;
    case AfxSig_v_v_v:Call<void>(method,self);break;
    case AfxSig_v_u_v:Call<void>(method,self,static_cast<UINT>(w));break;
    case AfxSig_v_up_v:Call<void>(method,self,static_cast<UINT_PTR>(w));break;
    case AfxSig_v_u_u:Call<void>(method,self,static_cast<UINT>(w),static_cast<UINT>(l));break;
    case AfxSig_v_uu_v:Call<void>(method,self,UINT(LOWORD(w)),UINT(HIWORD(w)));break;
    case AfxSig_v_v_ii:Call<void>(method,self,int(static_cast<short>(LOWORD(l))),int(static_cast<short>(HIWORD(l))));break;
    case AfxSig_v_u_uu:case AfxSig_v_u_ii:Call<void>(method,self,static_cast<UINT>(w),UINT(LOWORD(l)),UINT(HIWORD(l)));break;
    case AfxSig_v_u_W:Call<void>(method,self,static_cast<UINT>(w),Window(static_cast<UINT_PTR>(l)));break;
    case AfxSig_u_u_v:result=Call<UINT>(method,self,static_cast<UINT>(w));break;
    case AfxSig_b_v_v:result=Call<BOOL>(method,self);break;
    case AfxSig_v_w_l:Call<void>(method,self,w,l);break;
    case AfxSig_MDIACTIVATE:Call<void>(method,self,BOOL(self->m_hWnd==reinterpret_cast<HWND>(l)),Window(static_cast<UINT_PTR>(l)),Window(w));break;
    case AfxSig_v_D_v:Call<void>(method,self,DC(w));break;
    case AfxSig_v_M_v:Call<void>(method,self,Menu(w));break;
    case AfxSig_v_M_ub:Call<void>(method,self,Menu(w),int(static_cast<short>(LOWORD(l))),int(static_cast<short>(HIWORD(l))));break;
    case AfxSig_v_W_v:Call<void>(method,self,Window(w));break;
    case AfxSig_v_v_W:Call<void>(method,self,Window(static_cast<UINT_PTR>(l)));break;
    case AfxSig_v_W_uu:Call<void>(method,self,Window(w),UINT(LOWORD(l)),UINT(HIWORD(l)));break;
    case AfxSig_v_W_p:Call<void>(method,self,Window(w),Point(l));break;
    case AfxSig_v_W_h:Call<void>(method,self,Window(w),reinterpret_cast<HANDLE>(l));break;
    case AfxSig_C_v_v:case AfxSig_h_v_v:result=reinterpret_cast<LRESULT>(Call<HANDLE>(method,self));break;
    case AfxSig_ACTIVATE:Call<void>(method,self,UINT(LOWORD(w)),Window(static_cast<UINT_PTR>(l)),BOOL(HIWORD(w)));break;
    case AfxSig_SCROLL:Call<void>(method,self,int(static_cast<short>(LOWORD(w))),int(static_cast<short>(HIWORD(w))),Window(static_cast<UINT_PTR>(l)));break;
    case AfxSig_SCROLL_REFLECT:Call<void>(method,self,int(static_cast<short>(LOWORD(w))),int(static_cast<short>(HIWORD(w))));break;
    case AfxSig_v_v_s:case AfxSig_v_v_WINDOWPOS:Call<void>(method,self,reinterpret_cast<void*>(l));break;
    case AfxSig_v_u_cs:case AfxSig_v_b_NCCALCSIZEPARAMS:case AfxSig_v_u_LPMDINEXTMENU:Call<void>(method,self,static_cast<UINT>(w),reinterpret_cast<void*>(l));break;
    case AfxSig_OWNERDRAW:case AfxSig_SIZING:Call<void>(method,self,static_cast<UINT>(w),reinterpret_cast<void*>(l));result=TRUE;break;
    case AfxSig_i_i_s:result=Call<int>(method,self,static_cast<UINT>(w),reinterpret_cast<void*>(l));break;
    case AfxSig_u_v_p:result=Call<UINT>(method,self,Point(l));break;
    case AfxSig_u_v_v:result=Call<UINT>(method,self);break;
    case AfxSig_v_uu_M:Call<void>(method,self,UINT(LOWORD(w)),UINT(HIWORD(w)),reinterpret_cast<HMENU>(l));break;
    case AfxSig_v_u_p:Call<void>(method,self,static_cast<UINT>(w),Point(l));break;
    case AfxSig_MOUSEWHEEL:result=Call<BOOL>(method,self,UINT(LOWORD(w)),short(HIWORD(w)),Point(l));if(!result)return FALSE;break;
    case AfxSig_MOUSEHWHEEL:Call<void>(method,self,UINT(LOWORD(w)),short(HIWORD(w)),Point(l));break;
    case AfxSig_l:result=Call<LRESULT>(method,self);if(result)return FALSE;break;
    case AfxSig_l_p:result=Call<LRESULT>(method,self,Point(l));break;
    case AfxSig_u_W_u:result=Call<UINT>(method,self,Window(w),static_cast<UINT>(l));break;
    case AfxSig_v_u_M:Call<void>(method,self,static_cast<UINT>(w),Menu(static_cast<UINT_PTR>(l)));break;
    case AfxSig_u_u_M:result=Call<UINT>(method,self,static_cast<UINT>(w),Menu(static_cast<UINT_PTR>(l)));break;
    case AfxSig_u_v_MENUGETOBJECTINFO:result=Call<UINT>(method,self,reinterpret_cast<void*>(l));break;
    case AfxSig_v_M_u:Call<void>(method,self,Menu(w),static_cast<UINT>(l));break;
    case AfxSig_APPCOMMAND:Call<void>(method,self,Window(w),UINT(HIWORD(l)&0xfff),UINT(HIWORD(l)&0xf000),UINT(LOWORD(l)));result=TRUE;break;
    case AfxSig_RAWINPUT:Call<void>(method,self,UINT(static_cast<BYTE>(w)),reinterpret_cast<HRAWINPUT>(l));break;
    case AfxSig_u_u_u:result=Call<UINT>(method,self,static_cast<UINT>(w),static_cast<UINT>(l));break;
    case AfxSig_MOUSE_XBUTTON:Call<void>(method,self,UINT(LOWORD(w)),UINT(HIWORD(w)),Point(l));result=TRUE;break;
    case AfxSig_MOUSE_NCXBUTTON:Call<void>(method,self,short(LOWORD(w)),UINT(HIWORD(w)),Point(l));result=TRUE;break;
    case AfxSig_INPUTLANGCHANGE:Call<void>(method,self,static_cast<UINT>(w),static_cast<UINT>(l));result=TRUE;break;
    case AfxSig_INPUTDEVICECHANGE:Call<void>(method,self,WORD(w),reinterpret_cast<HANDLE>(l));break;
    case AfxSig_l_D_u:result=Call<LRESULT>(method,self,DC(w),static_cast<UINT>(l));break;
    case AfxSig_v_F_b:Call<void>(method,self,reinterpret_cast<CFont*>(impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z(reinterpret_cast<HGDIOBJ>(w))),BOOL(l));break;
    case AfxSig_h_b_h:result=reinterpret_cast<LRESULT>(Call<HANDLE>(method,self,BOOL(w),reinterpret_cast<HANDLE>(l)));break;
    case AfxSig_b_v_ii:result=Call<BOOL>(method,self,int(static_cast<short>(HIWORD(l))),int(static_cast<short>(LOWORD(l))));break;
    case AfxSig_h_h_h:result=reinterpret_cast<LRESULT>(Call<HANDLE>(method,self,reinterpret_cast<HANDLE>(w),reinterpret_cast<HANDLE>(l)));break;
    case AfxSig_MDINext:Call<void>(method,self,Window(w),BOOL(l));break;
    case AfxSig_u_u_l:result=Call<UINT>(method,self,static_cast<UINT>(w),l);break;
    default:break;
    }
    if(output)*output=result;
    return TRUE;
}
} }
// Symbol: ?AfxResetMsgCache@@YAXXZ
extern "C" void MS_ABI impl__AfxResetMsgCache__YAXXZ() { openmfc::detail::ResetWindowMessageCache(); }
void AFXAPI AfxResetMsgCache() { openmfc::detail::ResetWindowMessageCache(); }
