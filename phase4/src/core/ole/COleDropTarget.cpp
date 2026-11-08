// Native release-x64 registration, cached COM data and drag-scrolling protocol.
#define OPENMFC_APPCORE_IMPL
#include "detail/OleDropTargetSupport.h"
#include "detail/InterfaceMapsSupport.h"
#include <new>
extern "C" {
void AFXAPI impl__AfxLockGlobals__YAXH_Z(int);
void AFXAPI impl__AfxUnlockGlobals__YAXH_Z(int);
ULONG AFXAPI impl__ExternalAddRef_CCmdTarget__QEAAKXZ(CCmdTarget*);
ULONG AFXAPI impl__ExternalRelease_CCmdTarget__QEAAKXZ(CCmdTarget*);
ULONG AFXAPI impl__ExternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(CCmdTarget*,const void*,void**);
void* AFXAPI impl___0AFX_MAINTAIN_STATE2__QEAA_PEAVAFX_MODULE_STATE___Z(void*,AFX_MODULE_STATE*);
void AFXAPI impl___1AFX_MAINTAIN_STATE2__QEAA_XZ(void*);
int AFXAPI impl__AfxMessageBox__YAHPEB_WII_Z(const wchar_t*,UINT,UINT);
}
namespace {
bool metricsInitialized=false;
const AFX_INTERFACEMAP_ENTRY entries[]={{&IID_IDropTarget,0x60},{nullptr,SIZE_MAX}};
COleDropTarget* Owner(COleDropTarget::XDropTarget* self){return reinterpret_cast<COleDropTarget*>(reinterpret_cast<BYTE*>(self)-0x60);}
class ModuleScope {
    void* fields[2];
public:
    explicit ModuleScope(AFX_MODULE_STATE* state){impl___0AFX_MAINTAIN_STATE2__QEAA_PEAVAFX_MODULE_STATE___Z(fields,state);}
    ~ModuleScope(){impl___1AFX_MAINTAIN_STATE2__QEAA_XZ(fields);}
    ModuleScope(const ModuleScope&)=delete;
    ModuleScope& operator=(const ModuleScope&)=delete;
};
void ReleaseData(IDataObject*& data){if(auto* value=data){value->Release();data=nullptr;}}
DWORD Filter(DWORD effect,DWORD allowed){
    if(effect&allowed)return effect;
    if(effect==DROPEFFECT_COPY){if(allowed&DROPEFFECT_MOVE)return DROPEFFECT_MOVE;}
    else if(effect==DROPEFFECT_MOVE){if(allowed&DROPEFFECT_COPY)return DROPEFFECT_COPY;}
    else return DROPEFFECT_NONE;
    return allowed&DROPEFFECT_LINK ? DROPEFFECT_LINK:DROPEFFECT_NONE;
}
CView* View(CWnd* window){return window->IsKindOf(RUNTIME_CLASS(CView)) ? static_cast<CView*>(window):nullptr;}
void Report(CException* error){
    {
        CString text;wchar_t message[512];BOOL described=error->GetErrorMessage(message,512,nullptr);
        text=L"Exception thrown in destructor (D:\\a\\_work\\1\\s\\src\\vctools\\VC7Libs\\Ship\\ATLMFC\\Src\\MFC\\oledrop2.cpp:67)";
        if(described){text+=L"\n";text+=message;}
        impl__AfxMessageBox__YAHPEB_WII_Z(text.GetString(),0,0);
    }
    delete error;
}
}
namespace openmfc { namespace detail { namespace interfacemaps {
const AFX_INTERFACEMAP* AFXAPI gib_COleDropTarget(){return impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ();}
const AFX_INTERFACEMAP g_imap_COleDropTarget={gib_COleDropTarget,entries};
}}}
// Symbol: ?nScrollDelay@COleDropTarget@@1IA
UINT COleDropTarget::nScrollDelay=0;
// Symbol: ?nScrollInset@COleDropTarget@@1HA
int COleDropTarget::nScrollInset=0;
// Symbol: ?nScrollInterval@COleDropTarget@@1IA
UINT COleDropTarget::nScrollInterval=0;
COleDropTarget::COleDropTarget():m_hWnd(nullptr),m_lpDataObject(nullptr),m_nTimerID(0xffff){
    impl__AfxLockGlobals__YAXH_Z(4);
    if(!metricsInitialized){
        nScrollInset=static_cast<int>(::GetProfileIntW(L"windows",L"DragScrollInset",11));
        nScrollDelay=::GetProfileIntW(L"windows",L"DragScrollDelay",50);
        nScrollInterval=::GetProfileIntW(L"windows",L"DragScrollInterval",50);
        metricsInitialized=true;
    }
    impl__AfxUnlockGlobals__YAXH_Z(4);
}
COleDropTarget::~COleDropTarget(){try{if(m_hWnd)COleDropTarget::Revoke();}catch(CException* error){Report(error);}}
BOOL COleDropTarget::Register(CWnd* window){
    auto* unknown=GetInterface(&IID_IUnknown);
    if(::CoLockObjectExternal(unknown,TRUE,FALSE)!=S_OK)return FALSE;
    auto* target=reinterpret_cast<IDropTarget*>(GetInterface(&IID_IDropTarget));
    if(::RegisterDragDrop(window->m_hWnd,target)!=S_OK){::CoLockObjectExternal(unknown,FALSE,FALSE);return FALSE;}
    m_hWnd=window->m_hWnd;window->m_pDropTarget=this;return TRUE;
}
void COleDropTarget::Revoke(){
    if(m_hWnd){
        ::RevokeDragDrop(m_hWnd);
        auto* unknown=GetInterface(&IID_IUnknown);::CoLockObjectExternal(unknown,FALSE,TRUE);
        CWnd::FromHandle(m_hWnd)->m_pDropTarget=nullptr;m_hWnd=nullptr;
    }
}
DROPEFFECT COleDropTarget::OnDragEnter(CWnd* w,COleDataObject* d,DWORD keys,CPoint pt){auto* v=View(w);return v ? v->OnDragEnter(d,keys,pt):0;}
DROPEFFECT COleDropTarget::OnDragOver(CWnd* w,COleDataObject* d,DWORD keys,CPoint pt){auto* v=View(w);return v ? v->OnDragOver(d,keys,pt):0;}
BOOL COleDropTarget::OnDrop(CWnd* w,COleDataObject* d,DROPEFFECT effect,CPoint pt){auto* v=View(w);return v ? v->OnDrop(d,effect,pt):FALSE;}
DROPEFFECT COleDropTarget::OnDropEx(CWnd* w,COleDataObject* d,DROPEFFECT effect,DROPEFFECT allowed,CPoint pt){auto* v=View(w);return v ? v->OnDropEx(d,effect,allowed,pt):DWORD(-1);}
void COleDropTarget::OnDragLeave(CWnd* w){if(auto* v=View(w))v->OnDragLeave();}
DROPEFFECT COleDropTarget::OnDragScroll(CWnd* window,DWORD keys,CPoint point){
    auto* view=View(window);if(!view)return 0;
    DWORD effect=view->OnDragScroll(keys,point);if(effect!=DROPEFFECT_SCROLL)return effect;
    RECT outside{},inside;::GetClientRect(view->m_hWnd,&outside);inside=outside;
    int shrink=static_cast<int>(0u-static_cast<UINT>(nScrollInset));
    ::InflateRect(&inside,shrink,shrink);
    UINT timer=0xffff;CSplitterWnd* splitter=nullptr;
    if(::PtInRect(&outside,POINT{point.x,point.y}) && !::PtInRect(&inside,POINT{point.x,point.y})){
        if(point.x<inside.left)timer=0xff00;else if(point.x>=inside.right)timer=0xff01;
        if(point.y<inside.top)timer=LOBYTE(timer);else if(point.y>=inside.bottom)timer=LOBYTE(timer)|0x100;
        splitter=CView::GetParentSplitter(view,FALSE);
        BOOL possible=splitter ? splitter->DoScroll(view,timer,FALSE):view->OnScroll(timer,0,FALSE);
        if(!possible)timer=0xffff;
    }
    if(timer==0xffff){
        if(m_nTimerID!=0xffff){COleDataObject data;data.Attach(m_lpDataObject,FALSE);OnDragEnter(window,&data,keys,point);m_nTimerID=0xffff;}
        return 0;
    }
    ULONGLONG tick=::GetTickCount64();
    if(timer!=m_nTimerID){m_dwLastTick=static_cast<DWORD>(tick);m_nScrollDelay=nScrollDelay;}
    if(static_cast<DWORD>(tick)-m_dwLastTick>m_nScrollDelay){
        if(splitter)splitter->DoScroll(view,timer,TRUE);else view->OnScroll(timer,0,TRUE);
        m_dwLastTick=static_cast<DWORD>(tick);m_nScrollDelay=nScrollInterval;
    }
    if(m_nTimerID==0xffff)OnDragLeave(window);
    m_nTimerID=timer;
    if((keys&(MK_SHIFT|MK_CONTROL))==(MK_SHIFT|MK_CONTROL))return DROPEFFECT_SCROLL|DROPEFFECT_LINK;
    return DROPEFFECT_SCROLL | (keys&MK_CONTROL ? DROPEFFECT_COPY:DROPEFFECT_MOVE);
}
const AFX_INTERFACEMAP* AFXAPI COleDropTarget::GetThisInterfaceMap(){return &openmfc::detail::interfacemaps::g_imap_COleDropTarget;}
const AFX_INTERFACEMAP* COleDropTarget::GetInterfaceMap() const{return GetThisInterfaceMap();}
STDMETHODIMP COleDropTarget::XDropTarget::QueryInterface(REFIID iid,void** result){return static_cast<HRESULT>(impl__ExternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(Owner(this),&iid,result));}
STDMETHODIMP_(ULONG) COleDropTarget::XDropTarget::AddRef(){return impl__ExternalAddRef_CCmdTarget__QEAAKXZ(Owner(this));}
STDMETHODIMP_(ULONG) COleDropTarget::XDropTarget::Release(){return impl__ExternalRelease_CCmdTarget__QEAAKXZ(Owner(this));}
STDMETHODIMP COleDropTarget::XDropTarget::DragEnter(IDataObject* input,DWORD keys,POINTL point,DWORD* effect){
    auto* owner=Owner(this);ModuleScope module(owner->m_pModuleState);
    if(!input || !effect)return E_INVALIDARG;
    HRESULT result=E_UNEXPECTED;
    try{
        input->AddRef();ReleaseData(owner->m_lpDataObject);owner->m_lpDataObject=input;
        auto* window=CWnd::FromHandle(owner->m_hWnd);POINT local{point.x,point.y};::ScreenToClient(window->m_hWnd,&local);
        DWORD value=owner->OnDragScroll(window,keys,CPoint(local));
        if(!(value&DROPEFFECT_SCROLL)){COleDataObject data;data.Attach(input,FALSE);value=owner->OnDragEnter(window,&data,keys,CPoint(local));}
        *effect=Filter(value,*effect);result=S_OK;
    }catch(CException* error){error->Delete();}
    return result;
}
STDMETHODIMP COleDropTarget::XDropTarget::DragOver(DWORD keys,POINTL point,DWORD* effect){
    auto* owner=Owner(this);ModuleScope module(owner->m_pModuleState);if(!effect)return E_INVALIDARG;
    HRESULT result=E_UNEXPECTED;
    try{
        auto* window=CWnd::FromHandle(owner->m_hWnd);POINT local{point.x,point.y};::ScreenToClient(window->m_hWnd,&local);
        DWORD value=owner->OnDragScroll(window,keys,CPoint(local));
        if(!(value&DROPEFFECT_SCROLL)){COleDataObject data;data.Attach(owner->m_lpDataObject,FALSE);value=owner->OnDragOver(window,&data,keys,CPoint(local));}
        *effect=Filter(value,*effect);result=S_OK;
    }catch(CException* error){error->Delete();}
    return result;
}
STDMETHODIMP COleDropTarget::XDropTarget::DragLeave(){
    auto* owner=Owner(this);ModuleScope module(owner->m_pModuleState);auto* window=CWnd::FromHandle(owner->m_hWnd);owner->m_nTimerID=0xffff;
    COleDataObject data;data.Attach(owner->m_lpDataObject,FALSE);owner->OnDragLeave(window);ReleaseData(owner->m_lpDataObject);return S_OK;
}
STDMETHODIMP COleDropTarget::XDropTarget::Drop(IDataObject* input,DWORD keys,POINTL point,DWORD* effect){
    auto* owner=Owner(this);ModuleScope module(owner->m_pModuleState);if(!input || !effect)return E_INVALIDARG;
    HRESULT result=E_UNEXPECTED;
    try{
        owner->m_nTimerID=0xffff;auto* window=CWnd::FromHandle(owner->m_hWnd);
        COleDataObject data;data.Attach(input,FALSE);POINT local{point.x,point.y};::ScreenToClient(window->m_hWnd,&local);
        DWORD allowed=*effect;DWORD value=Filter(owner->OnDragOver(window,&data,keys,CPoint(local)),allowed);
        DWORD dropped=owner->OnDropEx(window,&data,value,*effect,CPoint(local));
        if(dropped==DWORD(-1)){
            if(value)dropped=owner->OnDrop(window,&data,value,CPoint(local)) ? value:0;
            else{owner->OnDragLeave(window);dropped=0;}
        }
        ReleaseData(owner->m_lpDataObject);*effect=dropped;result=S_OK;
    }catch(CException* error){error->Delete();}
    return result;
}

// Symbol: ?OnDragEnter@COleDropTarget@@UEAAKPEAVCWnd@@PEAVCOleDataObject@@KVCPoint@@@Z
// COleDropTarget::OnDragEnter
extern "C" unsigned long MS_ABI impl__OnDragEnter_COleDropTarget__UEAAKPEAVCWnd__PEAVCOleDataObject__KVCPoint___Z(COleDropTarget* pThis, CWnd* p0, COleDataObject* p1, unsigned long p2, CPoint p3) {
    return (unsigned long)pThis->COleDropTarget::OnDragEnter(p0, p1, p2, p3);
}

// Symbol: ?OnDragLeave@COleDropTarget@@UEAAXPEAVCWnd@@@Z
// COleDropTarget::OnDragLeave
extern "C" void MS_ABI impl__OnDragLeave_COleDropTarget__UEAAXPEAVCWnd___Z(COleDropTarget* pThis, CWnd* p0) {
    pThis->COleDropTarget::OnDragLeave(p0);
}

// Symbol: ?OnDragOver@COleDropTarget@@UEAAKPEAVCWnd@@PEAVCOleDataObject@@KVCPoint@@@Z
// COleDropTarget::OnDragOver
extern "C" unsigned long MS_ABI impl__OnDragOver_COleDropTarget__UEAAKPEAVCWnd__PEAVCOleDataObject__KVCPoint___Z(COleDropTarget* pThis, CWnd* p0, COleDataObject* p1, unsigned long p2, CPoint p3) {
    return (unsigned long)pThis->COleDropTarget::OnDragOver(p0, p1, p2, p3);
}

// Symbol: ?OnDragScroll@COleDropTarget@@UEAAKPEAVCWnd@@KVCPoint@@@Z
// COleDropTarget::OnDragScroll
extern "C" unsigned long MS_ABI impl__OnDragScroll_COleDropTarget__UEAAKPEAVCWnd__KVCPoint___Z(COleDropTarget* pThis, CWnd* p0, unsigned long p1, CPoint p2) {
    return (unsigned long)pThis->COleDropTarget::OnDragScroll(p0, p1, p2);
}

// Symbol: ?OnDrop@COleDropTarget@@UEAAHPEAVCWnd@@PEAVCOleDataObject@@KVCPoint@@@Z
// COleDropTarget::OnDrop
extern "C" int MS_ABI impl__OnDrop_COleDropTarget__UEAAHPEAVCWnd__PEAVCOleDataObject__KVCPoint___Z(COleDropTarget* pThis, CWnd* p0, COleDataObject* p1, unsigned long p2, CPoint p3) {
    return (int)pThis->COleDropTarget::OnDrop(p0, p1, p2, p3);
}

// Symbol: ?OnDropEx@COleDropTarget@@UEAAKPEAVCWnd@@PEAVCOleDataObject@@KKVCPoint@@@Z
// COleDropTarget::OnDropEx
extern "C" unsigned long MS_ABI impl__OnDropEx_COleDropTarget__UEAAKPEAVCWnd__PEAVCOleDataObject__KKVCPoint___Z(COleDropTarget* pThis, CWnd* p0, COleDataObject* p1, unsigned long p2, unsigned long p3, CPoint p4) {
    return (unsigned long)pThis->COleDropTarget::OnDropEx(p0, p1, p2, p3, p4);
}

// Symbol: ?Register@COleDropTarget@@QEAAHPEAVCWnd@@@Z
// COleDropTarget::Register
extern "C" int MS_ABI impl__Register_COleDropTarget__QEAAHPEAVCWnd___Z(COleDropTarget* pThis, CWnd* p0) {
    return (int)pThis->Register(p0);
}

// Symbol: ?Revoke@COleDropTarget@@UEAAXXZ
// COleDropTarget::Revoke
extern "C" void MS_ABI impl__Revoke_COleDropTarget__UEAAXXZ(COleDropTarget* pThis) {
    pThis->COleDropTarget::Revoke();
}

// Symbol: ??0COleDropTarget@@QEAA@XZ
// Constructor: COleDropTarget::COleDropTarget
extern "C" void* MS_ABI impl___0COleDropTarget__QEAA_XZ(void* pThis) {
    return new(pThis) COleDropTarget();
}

// Symbol: ??1COleDropTarget@@UEAA@XZ
// Destructor: COleDropTarget::~COleDropTarget
extern "C" void MS_ABI impl___1COleDropTarget__UEAA_XZ(void* pThis) {
    static_cast<COleDropTarget*>(pThis)->COleDropTarget::~COleDropTarget();
}

// Symbol: ?GetThisInterfaceMap@COleDropTarget@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* AFXAPI impl__GetThisInterfaceMap_COleDropTarget__KAPEBUAFX_INTERFACEMAP__XZ(){return &openmfc::detail::interfacemaps::g_imap_COleDropTarget;}
// Symbol: ?GetInterfaceMap@COleDropTarget@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* AFXAPI impl__GetInterfaceMap_COleDropTarget__MEBAPEBUAFX_INTERFACEMAP__XZ(const void*){return &openmfc::detail::interfacemaps::g_imap_COleDropTarget;}
// Symbol: ?DragEnter@XDropTarget@COleDropTarget@@UEAAJPEAUIDataObject@@KU_POINTL@@PEAK@Z
extern "C" HRESULT AFXAPI impl__DragEnter_XDropTarget_COleDropTarget__UEAAJPEAUIDataObject__KU_POINTL__PEAK_Z(COleDropTarget::XDropTarget* self,IDataObject* data,DWORD keys,POINTL point,DWORD* effect){return self->COleDropTarget::XDropTarget::DragEnter(data,keys,point,effect);}
// Symbol: ?DragOver@XDropTarget@COleDropTarget@@UEAAJKU_POINTL@@PEAK@Z
extern "C" HRESULT AFXAPI impl__DragOver_XDropTarget_COleDropTarget__UEAAJKU_POINTL__PEAK_Z(COleDropTarget::XDropTarget* self,DWORD keys,POINTL point,DWORD* effect){return self->COleDropTarget::XDropTarget::DragOver(keys,point,effect);}
// Symbol: ?DragLeave@XDropTarget@COleDropTarget@@UEAAJXZ
extern "C" HRESULT AFXAPI impl__DragLeave_XDropTarget_COleDropTarget__UEAAJXZ(COleDropTarget::XDropTarget* self){return self->COleDropTarget::XDropTarget::DragLeave();}
// Symbol: ?Drop@XDropTarget@COleDropTarget@@UEAAJPEAUIDataObject@@KU_POINTL@@PEAK@Z
extern "C" HRESULT AFXAPI impl__Drop_XDropTarget_COleDropTarget__UEAAJPEAUIDataObject__KU_POINTL__PEAK_Z(COleDropTarget::XDropTarget* self,IDataObject* data,DWORD keys,POINTL point,DWORD* effect){return self->COleDropTarget::XDropTarget::Drop(data,keys,point,effect);}
