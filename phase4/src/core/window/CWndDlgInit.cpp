// Native DLGINIT records are packed WORD ID / WORD message / DWORD length.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxctrlcontainer.h"
#include "detail/WindowMessageSupport.h"
#include <cstring>
#include <new>
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" {
HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(const wchar_t*,const wchar_t*);
void* MS_ABI impl___2_YAPEAX_K_Z(size_t);
void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND);
LRESULT MS_ABI impl__AfxCallWndProc__YA_JPEAVCWnd__PEAUHWND____I_K_J_Z(CWnd*,HWND,UINT,WPARAM,LPARAM);
}
void CWnd::SendMessageToDescendants(HWND parent,UINT message,ULONGLONG wparam,LONGLONG lparam,int deep,int permanent) {
    for(HWND child=::GetTopWindow(parent);child;child=::GetWindow(child,GW_HWNDNEXT)) {
        if(permanent) {
            CWnd* window=impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(child);
            if(window)impl__AfxCallWndProc__YA_JPEAVCWnd__PEAUHWND____I_K_J_Z(window,window->m_hWnd,message,wparam,lparam);
        } else ::SendMessageW(child,message,static_cast<WPARAM>(wparam),static_cast<LPARAM>(lparam));
        if(deep && ::GetTopWindow(child))SendMessageToDescendants(child,message,wparam,lparam,deep,permanent);
    }
}
int CWnd::ExecuteDlgInit(void* data) {
    auto* record=static_cast<BYTE*>(data);
    if(record)for(;;) {
        WORD id,message;DWORD size;
        std::memcpy(&id,record,2);if(!id)break;
        std::memcpy(&message,record+2,2);std::memcpy(&size,record+4,4);
        BYTE* payload=record+8;
        if(message==0x401)message=LB_ADDSTRING;else if(message==0x403)message=CB_ADDSTRING;
        BOOL success=TRUE;
        if(message==LB_ADDSTRING || message==CB_ADDSTRING)success=::SendDlgItemMessageA(m_hWnd,id,message,0,reinterpret_cast<LPARAM>(payload))!=-1;
        else if(message==0x37c) {
            if(!m_pMFCCtrlContainer) {
                void* storage=impl___2_YAPEAX_K_Z(sizeof(CMFCControlContainer));
                if(!storage)impl__AfxThrowMemoryException__YAXXZ();
                m_pMFCCtrlContainer=new(storage) CMFCControlContainer(this);
                m_pMFCCtrlContainer->SubclassDlgControls();
            }
            success=::SendDlgItemMessageA(m_hWnd,id,message,size,reinterpret_cast<LPARAM>(payload))!=-1;
            m_pMFCCtrlContainer->SetControlData(id,size,payload);
        }
        if(!success)return FALSE;
        record=payload+size;
    }
    SendMessageToDescendants(m_hWnd,0x364,0,0,FALSE,FALSE);return TRUE;
}
int CWnd::ExecuteDlgInit(const wchar_t* name) {
    void* data=nullptr;
    if(name) {
        HINSTANCE module=impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(name,MAKEINTRESOURCEW(240));
        HRSRC resource=::FindResourceW(module,name,MAKEINTRESOURCEW(240));
        if(resource) {HGLOBAL loaded=::LoadResource(module,resource);if(!loaded)return FALSE;data=::LockResource(loaded);}
    }
    return ExecuteDlgInit(data);
}
// Symbol: ?ExecuteDlgInit@CWnd@@QEAAHPEAX@Z
extern "C" int MS_ABI impl__ExecuteDlgInit_CWnd__QEAAHPEAX_Z(CWnd* self,void* data) {return self->ExecuteDlgInit(data);}
// Symbol: ?ExecuteDlgInit@CWnd@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__ExecuteDlgInit_CWnd__QEAAHPEB_W_Z(CWnd* self,const wchar_t* name) {return self->ExecuteDlgInit(name);}
// Symbol: ?SendMessageToDescendants@CWnd@@SAXPEAUHWND__@@I_K_JHH@Z
extern "C" void MS_ABI impl__SendMessageToDescendants_CWnd__SAXPEAUHWND____I_K_JHH_Z(HWND parent,UINT message,ULONGLONG wparam,LONGLONG lparam,int deep,int permanent) {CWnd::SendMessageToDescendants(parent,message,wparam,lparam,deep,permanent);}
