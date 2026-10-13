// Native CCmdUI routing and menu/control operations (mfc140.dll 0x1dc830).
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <mutex>
#include <cstring>
#include <new>
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
namespace {
void Invalid() { impl__AfxThrowInvalidArgException__YAXXZ(); }
void Position(const CCmdUI* ui) { if(ui->m_nIndex>=ui->m_nIndexMax)Invalid(); }
struct RadioBitmap {
    std::mutex lock;
    HBITMAP bitmap=nullptr;
    ~RadioBitmap() { if(bitmap)::DeleteObject(bitmap); }
    HBITMAP Get() {
        std::lock_guard<std::mutex> guard(lock);
        if(bitmap)return bitmap;
        DWORD dimensions=::GetMenuCheckMarkDimensions();
        int width=static_cast<short>(LOWORD(dimensions)),height=static_cast<short>(HIWORD(dimensions));
        if(width<=4 || height<=5) { Invalid();return nullptr; }
        int words,shift=12;
        if(width>32) { width=32;words=2; }
        else {
            words=(width+15)/16;
            int candidate=(width-4)/2+words*16-width;
            if(candidate<=12)shift=candidate;
        }
        if(height>32)height=32;
        BYTE pixels[128];std::memset(pixels,0xff,sizeof(pixels));
        const BYTE pattern[]={6,15,15,15,6};
        BYTE* row=pixels+((height-6)/2)*words*2;
        for(BYTE value:pattern) {
            WORD bits=static_cast<WORD>(~(static_cast<WORD>(value)<<shift));
            row[0]=static_cast<BYTE>(bits>>8);row[1]=static_cast<BYTE>(bits);row+=words*2;
        }
        bitmap=::CreateBitmap(width,height,1,1,pixels);
        if(!bitmap)bitmap=::LoadBitmapW(nullptr,MAKEINTRESOURCEW(0x7fe3));
        return bitmap;
    }
};
RadioBitmap radio;
}
CCmdUI::CCmdUI() : m_nID(0),m_nIndex(0),m_pMenu(nullptr),m_pSubMenu(nullptr),m_pOther(nullptr),
    m_bEnableChanged(FALSE),m_bContinueRouting(FALSE),m_nIndexMax(0),m_pParentMenu(nullptr) {}
void CCmdUI::Enable(BOOL enabled) {
    if(m_pMenu) {
        if(m_pSubMenu)return;
        Position(this);
        ::EnableMenuItem(m_pMenu->m_hMenu,m_nIndex,MF_BYPOSITION|(enabled ? MF_ENABLED : MF_DISABLED|MF_GRAYED));
    } else {
        if(!m_pOther) { Invalid();return; }
        HWND window=m_pOther->m_hWnd;
        if(!enabled && ::GetFocus()==window)::SendMessageW(::GetParent(window),WM_NEXTDLGCTL,0,0);
        ::EnableWindow(window,enabled);
    }
    m_bEnableChanged=TRUE;
}
void CCmdUI::SetCheck(int check) {
    if(m_pMenu) {
        if(m_pSubMenu)return;
        Position(this);
        ::CheckMenuItem(m_pMenu->m_hMenu,m_nIndex,MF_BYPOSITION|(check ? MF_CHECKED : MF_UNCHECKED));
    } else {
        if(!m_pOther) { Invalid();return; }
        HWND window=m_pOther->m_hWnd;
        if(::SendMessageW(window,WM_GETDLGCODE,0,0)&DLGC_BUTTON)::SendMessageW(window,BM_SETCHECK,static_cast<WPARAM>(static_cast<INT_PTR>(check)),0);
    }
}
void CCmdUI::SetRadio(BOOL checked) {
    SetCheck(checked!=FALSE);
    if(!m_pMenu || m_pSubMenu)return;
    Position(this);
    HBITMAP bitmap=radio.Get();
    if(bitmap)::SetMenuItemBitmaps(m_pMenu->m_hMenu,m_nIndex,MF_BYPOSITION,nullptr,bitmap);
}
void CCmdUI::SetText(const wchar_t* text) {
    if(!text) { Invalid();return; }
    if(m_pMenu) {
        if(m_pSubMenu)return;
        Position(this);
        MENUITEMINFOW item{};item.cbSize=sizeof(item);item.fMask=MIIM_STRING;item.dwTypeData=const_cast<wchar_t*>(text);
        ::SetMenuItemInfoW(m_pMenu->m_hMenu,m_nIndex,TRUE,&item);
    } else {
        if(!m_pOther) { Invalid();return; }
        ::SetWindowTextW(m_pOther->m_hWnd,text);
    }
}
BOOL CCmdUI::DoUpdate(CCmdTarget* target,BOOL disableIfNoHandler) {
    if(m_nID==0 || static_cast<WORD>(m_nID)==0xffff)return TRUE;
    if(!target) { Invalid();return FALSE; }
    m_bEnableChanged=FALSE;
    BOOL handled=target->OnCmdMsg(m_nID,CN_UPDATE_COMMAND_UI,this,nullptr);
    if(disableIfNoHandler && !m_bEnableChanged) {
        AFX_CMDHANDLERINFO info{};
        BOOL command=target->OnCmdMsg(m_nID,CN_COMMAND,this,&info);
        Enable(command);
    }
    return handled;
}
// Symbol: ??0CCmdUI@@QEAA@XZ
extern "C" void* MS_ABI impl___0CCmdUI__QEAA_XZ(void* storage) { return new(storage) CCmdUI; }
// Symbol: ?DoUpdate@CCmdUI@@QEAAHPEAVCCmdTarget@@H@Z
extern "C" int MS_ABI impl__DoUpdate_CCmdUI__QEAAHPEAVCCmdTarget__H_Z(CCmdUI* self,CCmdTarget* target,int disable) { return self->CCmdUI::DoUpdate(target,disable); }
// Symbol: ?Enable@CCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__Enable_CCmdUI__UEAAXH_Z(CCmdUI* self,int enabled) { self->CCmdUI::Enable(enabled); }
// Symbol: ?SetCheck@CCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetCheck_CCmdUI__UEAAXH_Z(CCmdUI* self,int check) { self->CCmdUI::SetCheck(check); }
// Symbol: ?SetRadio@CCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetRadio_CCmdUI__UEAAXH_Z(CCmdUI* self,int checked) { self->CCmdUI::SetRadio(checked); }
// Symbol: ?SetText@CCmdUI@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetText_CCmdUI__UEAAXPEB_W_Z(CCmdUI* self,const wchar_t* text) { self->CCmdUI::SetText(text); }
