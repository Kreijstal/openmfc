#pragma once
#include "openmfc/afxlayout.h"
#include "detail/CMFCDynamicLayoutDataSupport.h"

extern "C" void* MS_ABI impl___0CMFCDynamicLayout__QEAA_XZ(void*);
extern "C" void* MS_ABI impl___1CMFCDynamicLayout__UEAA_XZ(CMFCDynamicLayout*);
extern "C" void MS_ABI impl__EnableDynamicLayout_CWnd__QEAAXH_Z(CWnd*,int);
extern "C" int MS_ABI impl__LoadDynamicLayoutResource_CWnd__IEAAHPEB_W_Z(CWnd*,const wchar_t*);
namespace dynamic_layout_tests {
extern "C" HDWP (WINAPI* __imp_BeginDeferWindowPos)(int);
static auto nativeBegin=__imp_BeginDeferWindowPos;
static int batches=0;
static HDWP WINAPI Begin(int count) { ++batches;return nativeBegin(count); }
class Window : public CWnd {
public:
    CRuntimeClass* runtime=&CWnd::classCWnd;
    int resizes=0;
    CRuntimeClass* GetRuntimeClass() const override { return runtime; }
    CMenu* GetMenu() const override { return nullptr; }
    void ResizeDynamicLayout() override { ++resizes;CWnd::ResizeDynamicLayout(); }
};
struct Node { Node* next;Node* prev;void* item; };
template<class T> static T& field(void* object,size_t offset) { return *reinterpret_cast<T*>(static_cast<char*>(object)+offset); }
static HWND child(Window& host,int id,int x=10,int y=20,int width=60,int height=30,const wchar_t* cls=L"STATIC",DWORD style=0) {
    return CreateWindowExW(0,cls,L"",WS_CHILD|style,x,y,width,height,host.m_hWnd,reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),GetModuleHandleW(nullptr),nullptr);
}
static void host(Window& window,int width=400,int height=300,DWORD style=WS_POPUP,HWND parent=nullptr) {
    window.m_hWnd=CreateWindowExW(0,L"STATIC",L"dynamic layout",style,50,60,width,height,parent,nullptr,GetModuleHandleW(nullptr),nullptr);
    check(window.m_hWnd!=nullptr,"dynamic layout uses real host windows");
}
static RECT bounds(HWND window,HWND parent) {
    RECT rect{};GetWindowRect(window,&rect);MapWindowPoints(nullptr,parent,reinterpret_cast<POINT*>(&rect),2);return rect;
}
static void resize(Window& window,int width,int height) {
    SetWindowPos(window.m_hWnd,nullptr,0,0,width,height,SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
    impl__AfxCallWndProc__YA_JPEAVCWnd__PEAUHWND____I_K_J_Z(&window,window.m_hWnd,WM_SIZE,SIZE_RESTORED,MAKELPARAM(width,height));
}
static void lifecycleAndPositioning() {
    alignas(CMFCDynamicLayout) unsigned char storage[sizeof(CMFCDynamicLayout)+8];
    memset(storage,0xa5,sizeof(storage));
    auto* raw=static_cast<CMFCDynamicLayout*>(impl___0CMFCDynamicLayout__QEAA_XZ(storage));
    check(raw->GetRuntimeClass()==CMFCDynamicLayout::GetThisClass() && raw->GetRuntimeClass()->m_nObjectSize==80 &&
        raw->IsEmpty() && !raw->GetHostWnd() && raw->GetMinSize().cx==0 && raw->GetMinSize().cy==0 &&
        field<INT_PTR>(raw,0x48)==10 && field<unsigned long long>(storage,80)==0xa5a5a5a5a5a5a5a5ULL,
        "export constructor builds native vtables and initializes exactly 80 bytes");
    check(!raw->Create(nullptr),"layout rejects a missing host");
    Window window;host(window);
    HWND moving=child(window,201),sizing=child(window,202,120,30,70,40);
    check(raw->Create(&window) && raw->AddItem(moving,CMFCDynamicLayout::MoveHorizontalAndVertical(50,100),CMFCDynamicLayout::SizeNone()) &&
        raw->AddItem(UINT(202),CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeHorizontalAndVertical(100,50)),
        "native layout adds controls by handle and ID");
    auto* head=field<Node*>(raw,0x20);auto* tail=field<Node*>(raw,0x28);
    check(field<INT_PTR>(raw,0x30)==2 && head->next==tail && tail->prev==head && !head->prev && !tail->next &&
        field<void*>(raw,0x40)!=nullptr && raw->HasItem(moving) && !raw->AddItem(moving,CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeNone()),
        "native list head, tail, count, blocks and duplicate lookup agree");
    Window other;host(other);HWND unrelated=child(other,1);
    check(!raw->AddItem(unrelated,CMFCDynamicLayout::MoveHorizontal(100),CMFCDynamicLayout::SizeNone()) &&
        !raw->AddItem(UINT(999),CMFCDynamicLayout::MoveHorizontal(100),CMFCDynamicLayout::SizeNone()),
        "layout rejects unrelated and nonexistent controls");
    for(int i=0;i<11;++i)raw->AddItem(child(window,300+i),CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeNone());
    check(field<INT_PTR>(raw,0x30)==13 && *static_cast<void**>(field<void*>(raw,0x40))!=nullptr,
        "layout allocates a second native CPlex block after ten items");
    SetWindowPos(window.m_hWnd,nullptr,0,0,480,340,SWP_NOMOVE|SWP_NOZORDER);raw->Adjust();
    RECT move=bounds(moving,window.m_hWnd),size=bounds(sizing,window.m_hWnd);
    check(move.left==50 && move.top==60 && move.right-move.left==60 && size.left==120 && size.top==30 &&
        size.right-size.left==150 && size.bottom-size.top==60,"layout applies independent movement and sizing percentages");
    raw->SetMinSize(CSize(400,300));SetWindowPos(window.m_hWnd,nullptr,0,0,200,150,SWP_NOMOVE|SWP_NOZORDER);raw->Adjust();
    move=bounds(moving,window.m_hWnd);size=bounds(sizing,window.m_hWnd);
    check(move.left==10 && move.top==20 && size.right-size.left==70,"minimum host size clamps shrinking layout calculations");
    DestroyWindow(unrelated);DestroyWindow(sizing);raw->Adjust();
    impl___1CMFCDynamicLayout__UEAA_XZ(raw);
    check(!field<void*>(raw,0x20) && !field<INT_PTR>(raw,0x30) && !field<void*>(raw,0x40) &&
        field<unsigned long long>(storage,80)==0xa5a5a5a5a5a5a5a5ULL,"layout destructor clears native list storage and respects caller-owned storage");

    impl__EnableDynamicLayout_CWnd__QEAAXH_Z(&window,TRUE);
    auto* owned=window.GetDynamicLayout();
    check(window.IsDynamicLayoutEnabled() && field<CMFCDynamicLayout*>(&window,0x98)==owned && !owned->GetHostWnd(),
        "window owns a real layout at native offset 0x98");
    owned->Create(&window);owned->AddItem(moving,CMFCDynamicLayout::MoveHorizontal(100),CMFCDynamicLayout::SizeNone());
    resize(window,240,170);move=bounds(moving,window.m_hWnd);
    check(window.resizes==1 && move.left==50,"WM_SIZE reaches the native resize virtual and moves child controls");
    {
        window_input_tests::Import batch(__imp_BeginDeferWindowPos,Begin);
        ShowWindow(window.m_hWnd,SW_MINIMIZE);int previous=batches;window.ResizeDynamicLayout();
        check(IsIconic(window.m_hWnd) && batches==previous,"minimized windows skip dynamic layout adjustment");
        ShowWindow(window.m_hWnd,SW_RESTORE);
    }
    window.EnableDynamicLayout(TRUE);
    check(window.GetDynamicLayout()->IsEmpty() && !window.GetDynamicLayout()->GetHostWnd(),"re-enabling replaces the previous layout with a fresh object");
    window.EnableDynamicLayout(FALSE);check(!window.IsDynamicLayoutEnabled(),"disabling deletes the layout and clears the native field");
    g_testGenericAllocationFailure=true;window.EnableDynamicLayout(TRUE);g_testGenericAllocationFailure=false;
    check(!window.IsDynamicLayoutEnabled(),"failed native allocation leaves dynamic layout disabled");
    auto* heap=static_cast<CMFCDynamicLayout*>(impl___0CMFCDynamicLayout__QEAA_XZ(::operator new(80)));
    heap->Create(&window);heap->AddItem(moving,CMFCDynamicLayout::MoveHorizontal(100),CMFCDynamicLayout::SizeNone());
    using Delete=void* (MS_ABI*)(void*,unsigned int);
    int releases=g_testGenericFrees;
    auto deletion=reinterpret_cast<Delete>((*reinterpret_cast<void***>(heap))[1]);void* deleted=deletion(heap,1);
    check(deleted==heap && g_testGenericFrees==releases+1,"native deleting-destructor slot releases a heap layout and its item list");

    CMFCDynamicLayout comboLayout;comboLayout.Create(&window);
    HWND drop=child(window,401,0,0,120,100,L"COMBOBOX",CBS_DROPDOWN);
    HWND simple=child(window,402,0,0,120,100,L"COMBOBOX",CBS_SIMPLE);
    HWND list=child(window,403,0,0,120,100,L"COMBOBOX",CBS_DROPDOWNLIST);
    check(drop && simple && list && comboLayout.AddItem(drop,CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeVertical(100)) &&
        comboLayout.AddItem(simple,CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeVertical(100)) &&
        comboLayout.AddItem(list,CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeVertical(100)),"combo boxes register through the real layout path");
    auto* node=field<Node*>(&comboLayout,0x20);
    check(node && field<int>(node->item,52)==0 && field<int>(node->next->item,52)==100 && field<int>(node->next->next->item,52)==100,
        "combo-box vertical correction follows the native style-bit rule");
}
static void resources() {
    CMFCDynamicLayoutData data;
    short blob[]={0,-5,150,80,-40, 0,0,0,0};
    check(data.ReadResource(blob,sizeof(blob)),"resource parser reads version-zero layout records");
    auto* state=reinterpret_cast<openmfc::detail::cmfcdynamiclayoutdata::S_Cmfcdynamiclayoutdata*>(&data);
    check(state->m_nCount==2 && state->m_nBlockSize==10 && state->m_pNodeHead->data.m_moveSettings.m_nXRatio==0 &&
        state->m_pNodeHead->data.m_moveSettings.m_nYRatio==100 && state->m_pNodeHead->data.m_sizeSettings.m_nXRatio==80 &&
        state->m_pNodeHead->data.m_sizeSettings.m_nYRatio==0,"resource ratios clamp to the native 0-100 range");
    check(!data.ReadResource(nullptr,10) && state->m_nCount==2,"null resource failure preserves existing parsed records");
    WORD version=1;check(!data.ReadResource(&version,2) && state->m_nCount==0,"unsupported resource version clears previous records");
    version=0;check(!data.ReadResource(&version,2),"version-only resources have no usable layout records");
    unsigned char unaligned[12]{};memcpy(unaligned+1,blob,10);
    check(data.ReadResource(unaligned+1,11) && state->m_nCount==1,"parser accepts unaligned resource data and ignores a partial trailing record");
    Window empty;check(!data.ApplyLayoutDataTo(nullptr,FALSE) && !data.ApplyLayoutDataTo(&empty,FALSE),"resource application rejects missing host windows");
    Window window;host(window);child(window,1);child(window,2);child(window,3);
    HWND first=GetWindow(window.m_hWnd,GW_CHILD),second=GetWindow(first,GW_HWNDNEXT),third=GetWindow(second,GW_HWNDNEXT);
    short records[]={0,0,0,0,0, 100,0,0,0, 0,0,100,100};
    check(CMFCDynamicLayout::LoadResource(&window,records,sizeof(records)) && window.GetDynamicLayout()->GetHostWnd()==&window &&
        !window.GetDynamicLayout()->HasItem(first) && window.GetDynamicLayout()->HasItem(second) && window.GetDynamicLayout()->HasItem(third),
        "resource records follow native child order and skip controls with no movement or sizing");
    RECT before=bounds(second,window.m_hWnd);resize(window,460,320);RECT after=bounds(second,window.m_hWnd),size=bounds(third,window.m_hWnd);
    check(after.left==before.left+60 && size.right-size.left==120 && size.bottom-size.top==50,"loaded layout moves and resizes the corresponding real controls");
    auto* previous=window.GetDynamicLayout();
    check(!window.LoadDynamicLayoutResource(MAKEINTRESOURCEW(999)) && window.GetDynamicLayout()==previous,"missing named resource preserves the existing layout");
    check(impl__LoadDynamicLayoutResource_CWnd__IEAAHPEB_W_Z(&window,MAKEINTRESOURCEW(200)) &&
        window.GetDynamicLayout()->GetMinSize().cx==460 && window.GetDynamicLayout()->GetMinSize().cy==320,
        "integer resource IDs load AFX_DIALOG_LAYOUT and initialize the minimum client size");
    check(window.LoadDynamicLayoutResource(L"NamedLayout"),"named layout resources use the same native resource path");
    {
        window_input_tests::Import batch(__imp_BeginDeferWindowPos,Begin);
        batches=0;data.ApplyLayoutDataTo(&window,FALSE);
        check(batches==0,"resource application defers adjustment when bUpdate is false");
        data.ApplyLayoutDataTo(&window,TRUE);
        check(batches==1,"resource application adjusts the real layout when bUpdate is true");
    }
    data.CleanUp();check(!state->m_nCount && !state->m_pBlocks && !state->m_pNodeHead,"resource cleanup frees native list blocks and resets storage");
}
static void hostGeometry() {
    Window dialog;host(dialog,400,300);dialog.runtime=&g_layoutDialog;dialog.EnableDynamicLayout();
    RECT before{};GetClientRect(dialog.m_hWnd,&before);dialog.InitDynamicLayout();RECT after{};GetClientRect(dialog.m_hWnd,&after);
    check((GetWindowLongW(dialog.m_hWnd,GWL_STYLE)&(WS_THICKFRAME|WS_MAXIMIZEBOX))==(WS_THICKFRAME|WS_MAXIMIZEBOX) &&
        before.right==after.right && before.bottom==after.bottom && dialog.GetDynamicLayout()->GetMinSize().cx==after.right,
        "dialog initialization enables resizing while preserving its client dimensions");
    Window page;host(page,200,150,WS_CHILD,dialog.m_hWnd);page.runtime=&g_layoutPage;page.EnableDynamicLayout();page.InitDynamicLayout();
    check(page.GetDynamicLayout()->GetMinSize().cx==0 && !(GetWindowLongW(page.m_hWnd,GWL_STYLE)&WS_THICKFRAME),
        "property pages keep their host style and have no forced minimum size");
    Window form;host(form,400,300,WS_POPUP|WS_HSCROLL|WS_VSCROLL);form.runtime=&g_layoutForm;
    SetScrollRange(form.m_hWnd,SB_HORZ,0,200,FALSE);SetScrollRange(form.m_hWnd,SB_VERT,0,200,FALSE);
    SetScrollPos(form.m_hWnd,SB_HORZ,30,FALSE);SetScrollPos(form.m_hWnd,SB_VERT,40,FALSE);
    CMFCDynamicLayout layout;layout.Create(&form);CRect rect;layout.GetHostWndRect(rect);RECT client{};GetClientRect(form.m_hWnd,&client);
    check(rect.left==-30 && rect.top==-40 && rect.right==client.right && rect.bottom==client.bottom,
        "form-view layout includes the native scroll offsets");
    struct Sheet : Window { unsigned char nativeTail[0x71e0]; } sheet;
    host(sheet,600,500);sheet.runtime=&g_layoutMfcSheet;
    field<int>(&sheet,0x190)=1;field<int>(&sheet,0x71d0)=80;field<int>(&sheet,0x71d8)=30;
    Window mfcPage;host(mfcPage,400,300,WS_CHILD,sheet.m_hWnd);mfcPage.runtime=&g_layoutMfcPage;
    g_layoutParent=&sheet;CMFCDynamicLayout pageLayout;pageLayout.Create(&mfcPage);pageLayout.GetHostWndRect(rect);
    check(rect.left==0 && rect.top==0 && rect.right==320 && rect.bottom==240,
        "MFC property-page layout reads native sheet navigation and header fields");
    field<int>(&sheet,0x190)=0;pageLayout.GetHostWndRect(rect);
    check(rect.right==400 && rect.bottom==270,"standard property-sheet layout reserves only the header area");
    g_layoutParent=nullptr;
}
}
static void dynamicLayout() {
    dynamic_layout_tests::lifecycleAndPositioning();dynamic_layout_tests::resources();dynamic_layout_tests::hostGeometry();
}
