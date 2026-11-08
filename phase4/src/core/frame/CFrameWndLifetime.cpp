// Release x64 frame construction, module registration and owned-member cleanup.
#define OPENMFC_APPCORE_IMPL
#include "detail/FrameLifetimeSupport.h"
#include "detail/FrameAccessSupport.h"
#include "detail/WindowMessageSupport.h"
#include "openmfc/afxpriv.h"
#include <cstdlib>
#include <new>
extern "C" int MS_ABI impl__Remove_CSimpleList__QEAAHPEAX_Z(CSimpleList*,void*);
extern "C" int MS_ABI impl__AfxMessageBox__YAHPEB_WII_Z(const wchar_t*,UINT,UINT);
IMPLEMENT_DYNCREATE(CFrameWnd,CWnd)
namespace {
using namespace openmfc::detail::frame;
struct Construction {
    CFrameWnd* self;
    bool title=false,complete=false;
    ~Construction() {
        if(!complete) {
            if(title)Title(self).~CString();
            ControlBars(self).OpenMFC_CPtrList::~OpenMFC_CPtrList();
        }
    }
};
void ReportFailure(CException* error) {
    {
        CString report;
        wchar_t description[512];
        using Describe=BOOL (MS_ABI*)(CException*,wchar_t*,UINT,UINT*);
        const BOOL described=openmfc::detail::WindowSlot<Describe>(error,5)(error,description,512,nullptr);
        // This diagnostic identifies the native compatibility entry point.
        report=L"Exception thrown in destructor (D:\\a\\_work\\1\\s\\src\\vctools\\VC7Libs\\Ship\\ATLMFC\\Src\\MFC\\winfrm.cpp:159)";
        if(described) {report+=L"\n";report+=description;}
        impl__AfxMessageBox__YAHPEB_WII_Z(report.GetString(),0,0);
    }
    delete error;
}
}
CFrameWnd::CFrameWnd() {
    using namespace openmfc::detail::frame;
    m_retailMiddle.m_rectBorder={0,0,0,0};
    ::new(&ControlBars(this)) OpenMFC_CPtrList(10);
    Construction members{this};
    auto* data=OpenMFC_GetStringMgr()->GetNilString();
    ::new(m_retailTail.m_titleStorage) CString(data,CString::OwnedData{});
    members.title=true;
    m_retailPrefix.m_nWindow=-1;m_retailPrefix.m_bAutoMenuEnable=TRUE;
    m_retailTail.m_lpfnCloseProc=nullptr;
    m_retailPrefix.m_hMenuDefault=nullptr;m_hAccelTable=nullptr;
    m_nIDHelp=0;m_nIDTracking=0;m_nIDLastMessage=0;m_pViewActive=nullptr;
    m_retailTail.m_cModalStack=0;m_retailTail.m_phWndDisable=nullptr;
    std::memset(m_retailMiddle.m_pNotifyHook,0,sizeof(void*));
    m_retailTail.m_hMenuAlt=nullptr;m_retailTail.m_dwBorderFlags=0;
    ::SetRectEmpty(&m_retailMiddle.m_rectBorder);
    m_retailMiddle.m_dwPromptContext=0;m_retailMiddle.m_bHelpMode=FALSE;
    m_retailMiddle.m_pNextFrameWnd=nullptr;
    m_retailTail.m_bInRecalcLayout=FALSE;m_retailTail.m_pFloatingFrameClass=nullptr;
    m_retailMiddle.m_nShowDelay=-1;
    m_retailTail.m_dwMenuBarVisibility=1;m_retailTail.m_dwMenuBarState=1;
    m_retailTail.m_hMenu=nullptr;m_retailTail.m_bTempShowMenu=FALSE;
    m_retailTail.m_bMouseHitMenu=FALSE;
    m_retailTail.m_nProgressBarRangeMin=0;m_retailTail.m_nProgressBarRangeMax=100;
    AddFrameWnd();
    members.complete=true;
}
CFrameWnd::~CFrameWnd() {
    try {
        RemoveFrameWnd();
        auto* state=AfxGetThreadState();
        while(state->m_pRoutingFrame==this)state->m_pPushRoutingFrame->Pop();
        std::free(m_retailTail.m_phWndDisable);
        void* hook=*reinterpret_cast<void**>(m_retailMiddle.m_pNotifyHook);
        if(hook) {
            auto& frame=*reinterpret_cast<CFrameWnd**>(static_cast<BYTE*>(hook)+0x40);
            if(frame==this)frame=nullptr;
        }
    } catch(CException* error) {ReportFailure(error);}
    openmfc::detail::frame::Title(this).~CString();
    openmfc::detail::frame::ControlBars(this).OpenMFC_CPtrList::~OpenMFC_CPtrList();
}
void CFrameWnd::AddFrameWnd() {
    auto& list=openmfc::detail::modulethreadstate::ForModule(m_pModuleState)->m_frameList;
    list.GetNextPtr(this)=list.m_pHead;list.m_pHead=this;
}
void CFrameWnd::RemoveFrameWnd() {
    auto& list=openmfc::detail::modulethreadstate::ForModule(m_pModuleState)->m_frameList;
    (void)impl__Remove_CSimpleList__QEAAHPEAX_Z(&list,this);
}
BOOL CFrameWnd::IsFrameWnd() const {return TRUE;}
// Symbol: ??0CFrameWnd@@QEAA@XZ
extern "C" CFrameWnd* MS_ABI impl___0CFrameWnd__QEAA_XZ(CFrameWnd* self) {return ::new(self) CFrameWnd;}
// Symbol: ??1CFrameWnd@@UEAA@XZ
extern "C" void MS_ABI impl___1CFrameWnd__UEAA_XZ(CFrameWnd* self) {self->CFrameWnd::~CFrameWnd();}
// Symbol: ?AddFrameWnd@CFrameWnd@@IEAAXXZ
extern "C" void MS_ABI impl__AddFrameWnd_CFrameWnd__IEAAXXZ(CFrameWnd* self) {OpenMfcCFrameWndAccess::Add(self);}
// Symbol: ?RemoveFrameWnd@CFrameWnd@@IEAAXXZ
extern "C" void MS_ABI impl__RemoveFrameWnd_CFrameWnd__IEAAXXZ(CFrameWnd* self) {OpenMfcCFrameWndAccess::Remove(self);}
// Symbol: ?IsFrameWnd@CFrameWnd@@UEBAHXZ
extern "C" BOOL MS_ABI impl__IsFrameWnd_CFrameWnd__UEBAHXZ(const CFrameWnd*) {return TRUE;}
static_assert(sizeof(CFrameWnd)==0x1d8 && sizeof(CFrameWnd::RetailTailStorage)==0x60 &&
    offsetof(CFrameWnd,m_retailTail)+offsetof(CFrameWnd::RetailTailStorage,m_titleStorage)==0x198,
    "native frame storage and title string");
