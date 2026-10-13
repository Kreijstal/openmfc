#pragma once
#include "detail/CMFCRibbonCmdUISupport.h"
extern "C" void* MS_ABI impl___0CMFCRibbonCmdUI__QEAA_XZ(void*);
extern "C" void MS_ABI impl__Enable_CMFCRibbonCmdUI__UEAAXH_Z(void*,int);
extern "C" void MS_ABI impl__SetCheck_CMFCRibbonCmdUI__UEAAXH_Z(void*,int);
extern "C" void MS_ABI impl__SetRadio_CMFCRibbonCmdUI__UEAAXH_Z(void*,int);
extern "C" void MS_ABI impl__SetText_CMFCRibbonCmdUI__UEAAXPEB_W_Z(void*,const wchar_t*);
namespace ribbon_ui_test {
template<class T> T& field(void* p,size_t offset) {return *reinterpret_cast<T*>(static_cast<char*>(p)+offset);}
struct Element {
    alignas(8) unsigned char bytes[0x278]{};
    void* slots[146]{};
    int enables=0,checks=0,texts=0,lastEnable=-1,lastCheck=-1;
    BOOL queryDisabled=FALSE,queryChecked=FALSE;
    bool customQueries=false;
    wchar_t caption[64]=L"before";
    static Element* self(const void* p) {return reinterpret_cast<Element*>(const_cast<void*>(p));}
    static BOOL MS_ABI disabled(const void* p) {auto* e=self(p);return e->customQueries?e->queryDisabled:field<BOOL>(e,0x148);}
    static BOOL MS_ABI checked(const void* p) {auto* e=self(p);return e->customQueries?e->queryChecked:field<BOOL>(e,0x14c);}
    static void MS_ABI enable(void* p,BOOL value) {auto* e=self(p);++e->enables;e->lastEnable=value;}
    static void MS_ABI checkState(void* p,BOOL value) {auto* e=self(p);++e->checks;e->lastCheck=value;}
    static void MS_ABI text(void* p,const wchar_t* value) {auto* e=self(p);++e->texts;wcscpy_s(e->caption,value);}
    Element() {
        field<void**>(this,0)=slots;field<wchar_t*>(this,0xa0)=caption;
        field<RECT>(this,0xc8)={10,12,60,42};
        slots[47]=reinterpret_cast<void*>(&text);slots[57]=reinterpret_cast<void*>(&disabled);
        slots[58]=reinterpret_cast<void*>(&checked);slots[144]=reinterpret_cast<void*>(&enable);slots[145]=reinterpret_cast<void*>(&checkState);
    }
};
static int paints,radioCalls,radioValue;
static LRESULT CALLBACK window(HWND h,UINT m,WPARAM w,LPARAM l) {
    if(m==WM_PAINT)++paints;
    return ::DefWindowProcW(h,m,w,l);
}
static void MS_ABI radioCheck(void*,BOOL value) {++radioCalls;radioValue=value;}
using State=void (MS_ABI*)(void*,int);
using Text=void (MS_ABI*)(void*,const wchar_t*);
void exercise(State enable,State setCheck,State radio,Text text) {
    Element e;S_Cmfcribboncmdui ui{};impl___0CMFCRibbonCmdUI__QEAA_XZ(&ui);ui.m_pUpdated=&e;
    CWnd owner;
    WNDCLASSW wc{};wc.lpfnWndProc=window;wc.hInstance=::GetModuleHandleW(nullptr);wc.lpszClassName=L"OpenMFCRibbonUIProbe";
    ::RegisterClassW(&wc);
    owner.m_hWnd=::CreateWindowExW(0,wc.lpszClassName,L"",WS_POPUP|WS_VISIBLE,0,0,100,80,nullptr,nullptr,wc.hInstance,nullptr);
    check(owner.m_hWnd!=nullptr,"ribbon command UI creates a real repaint target");ui.m_pOther=&owner;
    ::UpdateWindow(owner.m_hWnd);paints=0;
    enable(&ui,0);
    check(ui.m_bEnableChanged && field<BOOL>(&e,0x148)==TRUE && e.enables==1 && e.lastEnable==FALSE && paints>0,
          "ribbon disable writes native state, notifies virtual OnEnable and repaints");
    paints=0;enable(&ui,0);
    check(e.enables==1 && paints==0,"unchanged ribbon enable state skips callback and repaint");
    enable(&ui,37);
    check(!field<BOOL>(&e,0x148) && e.enables==2 && e.lastEnable==TRUE,"ribbon enable normalizes nonzero arguments");
    e.customQueries=true;e.queryDisabled=TRUE;field<BOOL>(&e,0x148)=FALSE;enable(&ui,0);
    check(e.enables==2 && !field<BOOL>(&e,0x148),"ribbon enable consults the derived virtual getter");
    e.customQueries=false;setCheck(&ui,2);
    check(field<BOOL>(&e,0x14c)==2 && e.checks==1 && e.lastCheck==2,"ribbon check preserves indeterminate state in native field and callback");
    paints=0;setCheck(&ui,2);
    check(e.checks==1 && paints==0,"unchanged ribbon check skips callback and repaint");
    e.customQueries=true;e.queryChecked=7;setCheck(&ui,7);
    check(e.checks==1 && field<BOOL>(&e,0x14c)==2,"ribbon check consults the derived virtual getter");
    e.customQueries=false;radio(&ui,9);
    check(field<BOOL>(&e,0x150)==9 && field<BOOL>(&e,0x14c)==1 && e.lastCheck==1,"ribbon radio stores original flag and checks with normalized state");
    void* uiSlots[4];memcpy(uiSlots,ui.vfptr,sizeof(uiSlots));uiSlots[1]=reinterpret_cast<void*>(&radioCheck);ui.vfptr=uiSlots;
    radioCalls=0;radio(&ui,0);
    check(!field<BOOL>(&e,0x150) && radioCalls==1 && radioValue==0,"ribbon radio dispatches an overridden command UI SetCheck");
    paints=0;text(&ui,L"before");
    check(e.texts==0 && paints==0,"unchanged ribbon text skips virtual setter and repaint");
    text(&ui,L"after");
    check(e.texts==1 && wcscmp(e.caption,L"after")==0 && paints>0,"ribbon text invokes the derived virtual setter and repaints");
    HWND hwnd=owner.m_hWnd;owner.m_hWnd=nullptr;::DestroyWindow(hwnd);
}
}
static void ribbonCommandUI() {
    using namespace ribbon_ui_test;
    exercise(impl__Enable_CMFCRibbonCmdUI__UEAAXH_Z,impl__SetCheck_CMFCRibbonCmdUI__UEAAXH_Z,
             impl__SetRadio_CMFCRibbonCmdUI__UEAAXH_Z,impl__SetText_CMFCRibbonCmdUI__UEAAXPEB_W_Z);
    wchar_t nativePath[1024];
    if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_RIBBON_DLL",nativePath,1024)) {
        // These RVAs are for the inspected MSVC 14.51.36231 mfc140u DLL.
        HMODULE native=::LoadLibraryW(nativePath);check(native!=nullptr,"load native MFC for ribbon command UI differential checks");
        if(native) {auto* base=reinterpret_cast<char*>(native);
            exercise(reinterpret_cast<State>(base+0xd79b0),reinterpret_cast<State>(base+0xd7a70),
                     reinterpret_cast<State>(base+0xd7b10),reinterpret_cast<Text>(base+0xd7b40));
            ::FreeLibrary(native);
        }
    }
}
