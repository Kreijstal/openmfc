#pragma once
#include "openmfc/afxpriv.h"
#include "openmfc/afxole.h"
extern "C" void* MS_ABI impl___0CCmdUI__QEAA_XZ(void*);
extern "C" void* MS_ABI impl___0CTestCmdUI__QEAA_XZ(void*);
extern "C" void* MS_ABI impl___0COleCmdUI__QEAA_PEAU_tagOLECMD__KPEBU_GUID___Z(void*,OLECMD*,ULONG,const GUID*);
extern "C" int MS_ABI impl__DoUpdate_CCmdUI__QEAAHPEAVCCmdTarget__H_Z(CCmdUI*,CCmdTarget*,int);
namespace cmdui_tests {
template<class T> T& field(void* object,size_t offset) { return *reinterpret_cast<T*>(static_cast<char*>(object)+offset); }
class Target : public CCmdTarget {
public:
    int updates=0,queries=0,mappings=0;
    BOOL mapped=TRUE,handled=FALSE,command=FALSE,explicitEnable=FALSE,enabled=FALSE;
    UINT expected=17;
    BOOL OnCmdMsg(UINT id,int code,void* extra,AFX_CMDHANDLERINFO* info) override {
        check(id==expected,"command UI routes the current native command ID");
        auto* ui=static_cast<CCmdUI*>(extra);
        if(code==CN_OLECOMMAND) { ++mappings;if(mapped)ui->m_nID=expected;return mapped; }
        if(code==CN_UPDATE_COMMAND_UI) { ++updates;check(!info,"update routing has no handler query record");if(explicitEnable)ui->Enable(enabled);return handled; }
        if(code==CN_COMMAND) { ++queries;check(info && info->pTarget==nullptr,"fallback probes use a zero-initialized native handler-info record");return command; }
        std::abort();
    }
};
class Probe : public CCmdUI {
public:
    int enables=0,checks=0,texts=0;
    BOOL enabled=FALSE;
    int check=0;
    void Enable(BOOL value) override { ++enables;enabled=value;m_bEnableChanged=TRUE; }
    void SetCheck(int value) override { ++checks;check=value; }
    void SetText(const wchar_t*) override { ++texts; }
};
static void storageAndRouting() {
    alignas(CCmdUI) unsigned char storage[72];memset(storage,0xa5,sizeof(storage));
    auto* ui=static_cast<CCmdUI*>(impl___0CCmdUI__QEAA_XZ(storage));
    check(sizeof(CCmdUI)==64 && !ui->m_nID && !ui->m_nIndex && !ui->m_pMenu && !ui->m_pSubMenu && !ui->m_pOther &&
        !ui->m_bEnableChanged && !ui->m_bContinueRouting && !ui->m_nIndexMax && !ui->m_pParentMenu &&
        field<DWORD>(ui,52)==0xa5a5a5a5 && field<UINT64>(storage,64)==0xa5a5a5a5a5a5a5a5ULL,
        "export constructor initializes native 64-byte UI storage while preserving padding and caller guards");
    static_assert(offsetof(CCmdUI,m_pOther)==32 && offsetof(CCmdUI,m_bEnableChanged)==40 && offsetof(CCmdUI,m_bContinueRouting)==44 && offsetof(CCmdUI,m_pParentMenu)==56,"native command UI offsets");
    Probe probe;
    using Enable=void (MS_ABI*)(void*,BOOL);
    using Check=void (MS_ABI*)(void*,int);
    using Text=void (MS_ABI*)(void*,const wchar_t*);
    auto* table=*reinterpret_cast<void***>(&probe);
    reinterpret_cast<Enable>(table[0])(&probe,7);reinterpret_cast<Check>(table[1])(&probe,2);
    reinterpret_cast<Enable>(table[2])(&probe,99);reinterpret_cast<Text>(table[3])(&probe,L"text");
    check(probe.enables==1 && probe.enabled==7 && probe.checks==2 && probe.check==1 && probe.texts==1,
        "the four native UI virtual slots dispatch overrides and SetRadio delegates to SetCheck");
    Target target;Probe update;update.m_nID=17;
    check(!impl__DoUpdate_CCmdUI__QEAAHPEAVCCmdTarget__H_Z(&update,&target,TRUE) && target.updates==1 && target.queries==1 &&
        update.enables==1 && !update.enabled,"unhandled commands are probed and automatically disabled");
    target.command=TRUE;
    check(!update.DoUpdate(&target,TRUE) && update.enabled && target.queries==2,"commands with handlers are automatically enabled without changing the update-handler result");
    target.handled=TRUE;target.explicitEnable=TRUE;target.enabled=FALSE;
    check(update.DoUpdate(&target,TRUE) && !update.enabled && target.queries==2,"explicit enable decisions suppress fallback handler probes");
    target.explicitEnable=FALSE;int probes=target.queries;
    check(update.DoUpdate(&target,FALSE) && target.queries==probes && !update.m_bEnableChanged,"disabled fallback policy leaves UI enable state to the update handler");
    update.m_nID=0;check(update.DoUpdate(nullptr,TRUE),"zero command IDs bypass update routing");
    update.m_nID=0x12ffff;check(update.DoUpdate(nullptr,TRUE),"IDs with native low-word 0xffff bypass update routing");
    update.m_nID=17;bool invalid=false;
    try { update.DoUpdate(nullptr,TRUE); }catch(int code) { invalid=code==19; }
    check(invalid,"valid command IDs require a target and use the framework invalid-argument exception");
    update.ContinueRouting();check(field<BOOL>(&update,44)==TRUE,"ContinueRouting writes the native BOOL field");
    ui->~CCmdUI();

    alignas(CTestCmdUI) unsigned char testStorage[80];memset(testStorage,0xa5,sizeof(testStorage));
    auto* test=static_cast<CTestCmdUI*>(impl___0CTestCmdUI__QEAA_XZ(testStorage));
    check(test->m_bEnabled && field<UINT64>(testStorage,72)==0xa5a5a5a5a5a5a5a5ULL,"CTestCmdUI initializes its enabled flag at native offset 64");
    test->Enable(23);test->SetCheck(2);test->SetRadio(FALSE);test->SetText(nullptr);
    check(test->m_bEnabled==23 && test->m_bEnableChanged && !test->m_pMenu,"CTestCmdUI records enable state and its other callbacks are native no-ops");
    test->~CTestCmdUI();
}
static void menusAndControls() {
    struct MenuModel { void* table;HMENU handle; } menu{nullptr,CreatePopupMenu()};
    AppendMenuW(menu.handle,MF_STRING,101,L"first");AppendMenuW(menu.handle,MF_STRING,102,L"second");
    CCmdUI ui;ui.m_pMenu=reinterpret_cast<CMenu*>(&menu);ui.m_nID=101;ui.m_nIndex=1;ui.m_nIndexMax=2;
    ui.Enable(FALSE);ui.SetCheck(2);ui.SetText(L"updated");wchar_t text[40]{};
    GetMenuStringW(menu.handle,1,text,40,MF_BYPOSITION);
    check((GetMenuState(menu.handle,1,MF_BYPOSITION)&(MF_DISABLED|MF_GRAYED|MF_CHECKED))==(MF_DISABLED|MF_GRAYED|MF_CHECKED) &&
        !(GetMenuState(menu.handle,0,MF_BYPOSITION)&MF_CHECKED) && !wcscmp(text,L"updated") && ui.m_bEnableChanged,
        "menu updates use native item positions, including index zero, independently of the command ID");
    ui.m_nIndex=0;ui.Enable(FALSE);check(GetMenuState(menu.handle,0,MF_BYPOSITION)&MF_GRAYED,"the first menu item is updated by position");
    ui.m_nIndex=1;ui.SetRadio(TRUE);
    MENUITEMINFOW item{};item.cbSize=sizeof(item);item.fMask=MIIM_CHECKMARKS;GetMenuItemInfoW(menu.handle,1,TRUE,&item);
    BITMAP bitmap{};
    check(item.hbmpChecked && !item.hbmpUnchecked && GetObjectW(item.hbmpChecked,sizeof(bitmap),&bitmap) && bitmap.bmPlanes==1 && bitmap.bmBitsPixel==1,
        "radio updates install the native cached monochrome checkmark bitmap");
    HBITMAP previous=item.hbmpChecked;ui.SetRadio(FALSE);GetMenuItemInfoW(menu.handle,1,TRUE,&item);
    check(item.hbmpChecked==previous && !(GetMenuState(menu.handle,1,MF_BYPOSITION)&MF_CHECKED),"unchecking a radio item preserves its cached bitmap");
    ui.m_pSubMenu=reinterpret_cast<CMenu*>(&menu);ui.m_bEnableChanged=FALSE;
    ui.Enable(TRUE);ui.SetCheck(TRUE);ui.SetText(L"ignored");GetMenuStringW(menu.handle,1,text,40,MF_BYPOSITION);
    check(!ui.m_bEnableChanged && !wcscmp(text,L"updated"),"popup command UI entries leave menu state and text unchanged");
    ui.m_pSubMenu=nullptr;ui.m_nIndex=2;bool invalid=false;
    try { ui.Enable(TRUE); }catch(int code) { invalid=code==19; }
    check(invalid,"menu updates reject indexes outside the native index limit");
    DestroyMenu(menu.handle);ui.m_pMenu=nullptr;
    CWnd control;control.m_hWnd=CreateWindowExW(0,L"BUTTON",L"old",WS_POPUP|BS_3STATE,0,0,100,40,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);
    ui.m_pOther=&control;ui.Enable(FALSE);ui.SetCheck(2);ui.SetText(L"control text");
    GetWindowTextW(control.m_hWnd,text,40);
    check(!IsWindowEnabled(control.m_hWnd) && SendMessageW(control.m_hWnd,BM_GETCHECK,0,0)==BST_INDETERMINATE && !wcscmp(text,L"control text"),
        "control updates use CWnd handles for enable, button check and text operations");
    ui.Enable(TRUE);check(IsWindowEnabled(control.m_hWnd),"control enable updates restore the native window state");
    ui.m_pOther=nullptr;invalid=false;
    try { ui.SetText(nullptr); }catch(int code) { invalid=code==19; }
    check(invalid,"null text uses the native invalid-argument path");
    CToolCmdUI tool;tool.m_pOther=&control;tool.m_nIndex=3;g_toolCommandStyle=0x20000;
    tool.Enable(FALSE);tool.SetCheck(2);
    check(g_toolCommandIndex==3 && tool.m_bEnableChanged && g_toolCommandStyle==0x140002,
        "toolbar UI updates translate pressed, disabled, check and indeterminate style bits without recursive virtual calls");
    tool.SetRadio(TRUE);check(g_toolCommandStyle==0x50002,"toolbar radio updates use the native SetCheck override");
}
static void oleState() {
    OLECMD commands[]={{10,OLECMDF_SUPPORTED|OLECMDF_LATCHED},{20,OLECMDF_SUPPORTED|OLECMDF_NINCHED}};
    GUID group{0x1234};alignas(COleCmdUI) unsigned char storage[104];memset(storage,0xa5,sizeof(storage));
    auto* ui=static_cast<COleCmdUI*>(impl___0COleCmdUI__QEAA_PEAU_tagOLECMD__KPEBU_GUID___Z(storage,commands,2,&group));
    check(sizeof(COleCmdUI)==96 && ui->m_nIndexMax==2 && field<OLECMD*>(ui,72)==commands && field<const GUID*>(ui,80)==&group &&
        ui->m_strText.IsEmpty() && field<UINT64>(storage,96)==0xa5a5a5a5a5a5a5a5ULL,"OLE UI construction uses native command, group and CString offsets");
    ui->m_nIndex=1;ui->Enable(TRUE);ui->SetCheck(1);ui->SetText(L"OLE command");
    check(commands[1].cmdf==(OLECMDF_SUPPORTED|OLECMDF_ENABLED|OLECMDF_LATCHED) && commands[0].cmdf==(OLECMDF_SUPPORTED|OLECMDF_LATCHED) &&
        ui->m_strText==L"OLE command","OLE state updates preserve unrelated flags and select the current command index");
    ui->SetCheck(2);ui->Enable(FALSE);
    check(commands[1].cmdf==(OLECMDF_SUPPORTED|OLECMDF_NINCHED),"OLE check-state transitions clear latched and indeterminate bits independently of enabled state");
    ui->SetText(nullptr);check(ui->m_strText.IsEmpty(),"OLE text callbacks own and clear native CString storage");
    Target target;ui->m_nID=17;target.handled=TRUE;target.explicitEnable=TRUE;target.enabled=TRUE;
    check(ui->DoUpdate(&target,TRUE) && target.mappings==1 && target.updates==1 && target.queries==0 &&
        (commands[1].cmdf&(OLECMDF_SUPPORTED|OLECMDF_ENABLED))==(OLECMDF_SUPPORTED|OLECMDF_ENABLED),
        "OLE updates route command-group translation before normal update handlers");
    target.mapped=FALSE;target.command=FALSE;target.explicitEnable=FALSE;
    check(!ui->DoUpdate(&target,TRUE) && target.updates==1 && target.queries==1 && !(commands[1].cmdf&(OLECMDF_SUPPORTED|OLECMDF_ENABLED)),
        "unmapped OLE commands are probed and marked unsupported when no handler exists");
    target.command=TRUE;
    check(ui->DoUpdate(&target,TRUE) && (commands[1].cmdf&(OLECMDF_SUPPORTED|OLECMDF_ENABLED))==(OLECMDF_SUPPORTED|OLECMDF_ENABLED),
        "OLE command fallback marks a discovered command supported and enabled");
    ui->~COleCmdUI();
}
}
static void commandUI() { cmdui_tests::storageAndRouting();cmdui_tests::menusAndControls();cmdui_tests::oleState(); }
