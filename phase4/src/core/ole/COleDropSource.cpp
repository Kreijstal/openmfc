// Release-x64 drag source: native fields, interface identity and drag-start loop.
#define OPENMFC_APPCORE_IMPL
#include "detail/OleDropSourceSupport.h"
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
}
namespace {
bool metricsInitialized=false;
COleDropSource* Owner(COleDropSource::XDropSource* self) {
    return reinterpret_cast<COleDropSource*>(reinterpret_cast<BYTE*>(self)-offsetof(COleDropSource,m_xDropSource));
}
class ModuleScope {
    void* state[2];
public:
    explicit ModuleScope(AFX_MODULE_STATE* module) {impl___0AFX_MAINTAIN_STATE2__QEAA_PEAVAFX_MODULE_STATE___Z(state,module);}
    ~ModuleScope() {impl___1AFX_MAINTAIN_STATE2__QEAA_XZ(state);}
    ModuleScope(const ModuleScope&)=delete;
    ModuleScope& operator=(const ModuleScope&)=delete;
};
const AFX_INTERFACEMAP_ENTRY sourceEntries[]={{&IID_IDropSource,0x40},{nullptr,SIZE_MAX}};
}
namespace openmfc { namespace detail { namespace interfacemaps {
const AFX_INTERFACEMAP* AFXAPI gib_COleDropSource() {return impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ();}
const AFX_INTERFACEMAP g_imap_COleDropSource={gib_COleDropSource,sourceEntries};
}}}
// Symbol: ?nDragDelay@COleDropSource@@1IA
UINT COleDropSource::nDragDelay=0;
// Symbol: ?nDragMinDist@COleDropSource@@1IA
UINT COleDropSource::nDragMinDist=0;
COleDropSource::COleDropSource() : m_bDragStarted(FALSE),m_dwButtonCancel(0),m_dwButtonDrop(0) {
    impl__AfxLockGlobals__YAXH_Z(3);
    if(!metricsInitialized) {
        nDragMinDist=::GetProfileIntW(L"windows",L"DragMinDist",2);
        nDragDelay=::GetProfileIntW(L"windows",L"DragDelay",200);
        metricsInitialized=true;
    }
    impl__AfxUnlockGlobals__YAXH_Z(3);
}
SCODE COleDropSource::QueryContinueDrag(BOOL escape,DWORD keys) {
    if(escape || (keys&m_dwButtonCancel)) {m_bDragStarted=FALSE;return DRAGDROP_S_CANCEL;}
    if(keys&m_dwButtonDrop)return S_OK;
    return m_bDragStarted ? DRAGDROP_S_DROP:DRAGDROP_S_CANCEL;
}
SCODE COleDropSource::GiveFeedback(DROPEFFECT) {
    return m_bDragStarted ? DRAGDROP_S_USEDEFAULTCURSORS:S_OK;
}
BOOL COleDropSource::OnBeginDrag(CWnd* window) {
    m_bDragStarted=FALSE;m_dwButtonCancel=0;m_dwButtonDrop=0;
    if(::GetKeyState(VK_LBUTTON)<0) {m_dwButtonDrop|=MK_LBUTTON;m_dwButtonCancel|=MK_RBUTTON;}
    else if(::GetKeyState(VK_RBUTTON)<0) {m_dwButtonDrop|=MK_RBUTTON;m_dwButtonCancel|=MK_LBUTTON;}
    ULONGLONG started=::GetTickCount64();
    (void)CWnd::FromHandle(::SetCapture(window->m_hWnd));
    while(!m_bDragStarted) {
        if(CWnd::FromHandle(::GetCapture())!=window)break;
        MSG message;
        if(::PeekMessageW(&message,nullptr,WM_MOUSEFIRST,WM_MBUTTONDBLCLK,PM_REMOVE) ||
           ::PeekMessageW(&message,nullptr,WM_KEYFIRST,WM_KEYLAST,PM_REMOVE)) {
            if(message.message>WM_MOUSEMOVE && message.message<WM_RBUTTONDBLCLK && message.message!=WM_LBUTTONDBLCLK)break;
            if(message.message==WM_KEYDOWN && message.wParam==VK_ESCAPE)break;
            m_bDragStarted=!::PtInRect(reinterpret_cast<const RECT*>(&m_rectStartDrag),message.pt);
        }
        if(::GetTickCount64()-started>nDragDelay)m_bDragStarted=TRUE;
    }
    ::ReleaseCapture();
    return m_bDragStarted;
}
const AFX_INTERFACEMAP* AFXAPI COleDropSource::GetThisInterfaceMap() {return &openmfc::detail::interfacemaps::g_imap_COleDropSource;}
const AFX_INTERFACEMAP* COleDropSource::GetInterfaceMap() const {return GetThisInterfaceMap();}
STDMETHODIMP COleDropSource::XDropSource::QueryInterface(REFIID iid,void** result) {
    return static_cast<HRESULT>(impl__ExternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(Owner(this),&iid,result));
}
STDMETHODIMP_(ULONG) COleDropSource::XDropSource::AddRef() {return impl__ExternalAddRef_CCmdTarget__QEAAKXZ(Owner(this));}
STDMETHODIMP_(ULONG) COleDropSource::XDropSource::Release() {return impl__ExternalRelease_CCmdTarget__QEAAKXZ(Owner(this));}
STDMETHODIMP COleDropSource::XDropSource::QueryContinueDrag(BOOL escape,DWORD keys) {return Owner(this)->QueryContinueDrag(escape,keys);}
STDMETHODIMP COleDropSource::XDropSource::GiveFeedback(DWORD effect) {
    auto* owner=Owner(this);ModuleScope module(owner->m_pModuleState);return owner->GiveFeedback(effect);
}
// Symbol: ??0COleDropSource@@QEAA@XZ
extern "C" void* AFXAPI impl___0COleDropSource__QEAA_XZ(void* self) {return ::new(self) COleDropSource;}
// Symbol: ?QueryContinueDrag@COleDropSource@@UEAAJHK@Z
extern "C" SCODE AFXAPI impl__QueryContinueDrag_COleDropSource__UEAAJHK_Z(COleDropSource* self,BOOL escape,DWORD keys) {return self->COleDropSource::QueryContinueDrag(escape,keys);}
// Symbol: ?GiveFeedback@COleDropSource@@UEAAJK@Z
extern "C" SCODE AFXAPI impl__GiveFeedback_COleDropSource__UEAAJK_Z(COleDropSource* self,DROPEFFECT effect) {return self->COleDropSource::GiveFeedback(effect);}
// Symbol: ?OnBeginDrag@COleDropSource@@UEAAHPEAVCWnd@@@Z
extern "C" BOOL AFXAPI impl__OnBeginDrag_COleDropSource__UEAAHPEAVCWnd___Z(COleDropSource* self,CWnd* window) {return self->COleDropSource::OnBeginDrag(window);}
// Symbol: ?QueryContinueDrag@XDropSource@COleDropSource@@UEAAJHK@Z
extern "C" HRESULT AFXAPI impl__QueryContinueDrag_XDropSource_COleDropSource__UEAAJHK_Z(COleDropSource::XDropSource* self,BOOL escape,DWORD keys) {return self->COleDropSource::XDropSource::QueryContinueDrag(escape,keys);}
// Symbol: ?GiveFeedback@XDropSource@COleDropSource@@UEAAJK@Z
extern "C" HRESULT AFXAPI impl__GiveFeedback_XDropSource_COleDropSource__UEAAJK_Z(COleDropSource::XDropSource* self,DWORD effect) {return self->COleDropSource::XDropSource::GiveFeedback(effect);}
// Symbol: ?GetThisInterfaceMap@COleDropSource@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* AFXAPI impl__GetThisInterfaceMap_COleDropSource__KAPEBUAFX_INTERFACEMAP__XZ() {return &openmfc::detail::interfacemaps::g_imap_COleDropSource;}
// Symbol: ?GetInterfaceMap@COleDropSource@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* AFXAPI impl__GetInterfaceMap_COleDropSource__MEBAPEBUAFX_INTERFACEMAP__XZ(const void*) {return &openmfc::detail::interfacemaps::g_imap_COleDropSource;}
