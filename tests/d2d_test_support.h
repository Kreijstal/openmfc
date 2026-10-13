#pragma once
#include "openmfc/afxinet.h"
#include "openmfc/afxstat_.h"
#include <cstdlib>

// Unused GDI siblings in the shared support TU still need link definitions.
// Abort if a drawing path unexpectedly reaches one of these test doubles.
CImageList::CImageList() { std::abort(); }
CImageList::~CImageList() { std::abort(); }
CDC::CDC() { std::abort(); }
CDC::~CDC() { std::abort(); }
BOOL CGdiObject::DeleteObject() { std::abort(); }
HINSTANCE g_d2dTestResourceModule = nullptr;
#define g_cmdTestCurrentModule (AfxGetThreadState()->m_pModuleState)
HINSTANCE g_typeCacheInstance=nullptr;
AFX_MODULE_STATE* g_typeCacheInstanceModule=nullptr;
bool g_typeCacheForceNoInstance=false;
HINSTANCE AFXAPI AfxGetInstanceHandle() {
    g_typeCacheInstanceModule=g_cmdTestCurrentModule;
    return g_typeCacheForceNoInstance ? nullptr : g_typeCacheInstance ? g_typeCacheInstance : GetModuleHandleW(nullptr);
}
HINSTANCE AFXAPI AfxGetResourceHandle() { return g_d2dTestResourceModule ? g_d2dTestResourceModule : GetModuleHandleW(nullptr); }
// List serialization is linked for its vtable but is outside these drawing tests.
CArchive& CArchive::operator<<(unsigned short) { std::abort(); }
CArchive& CArchive::operator<<(const CObject*) { std::abort(); }
CArchive& CArchive::operator>>(unsigned short&) { std::abort(); }
CArchive& CArchive::operator>>(CObject*&) { std::abort(); }
// Command routing is outside these rendering tests.
CCmdTarget::~CCmdTarget() = default;
int CCmdTarget::OnCmdMsg(unsigned int,int,void*,AFX_CMDHANDLERINFO*) { std::abort(); }
CRuntimeClass CCmdTarget::classCCmdTarget{};
CRuntimeClass CWinThread::classCWinThread{};
CRuntimeClass CWinApp::classCWinApp{};
CRuntimeClass CWnd::classCWnd{};
CRuntimeClass CFileFind::classCFileFind{};
CRuntimeClass CDocument::classCDocument{};
CRuntimeClass CHtmlView::classCHtmlView{};


// Module switching is peripheral to rendering; these doubles model thread
// state so the command-target tests can verify capture and balanced restore.
extern "C" AFX_MODULE_STATE* MS_ABI impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ() { return g_cmdTestCurrentModule; }

// Property getter allocation/type failures use the framework exception sibling.
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long result) {
    auto* error=new COleException;error->m_sc=result;throw error;
}

// Invalid event arguments exercise the framework exception path.
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ() { throw 19; }

// Pointer-array serialization is unrelated to connection enumeration.
CArchive& CArchive::operator<<(const void*) { std::abort(); }
CArchive& CArchive::operator>>(void*&) { std::abort(); }
CArchive& CArchive::operator<<(unsigned int) { std::abort(); }
CArchive& CArchive::operator>>(unsigned int&) { std::abort(); }

// Connection-array allocation failures use the framework memory exception.
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ() { throw std::bad_alloc(); }

// Exception diagnostics and application lookup are peripheral to automation.
CRuntimeClass CException::classCException{"CException",sizeof(CException),0xffff,nullptr,&CObject::GetThisClass,nullptr,nullptr};
int CException::GetErrorMessage(wchar_t*,UINT,UINT*) const { std::abort(); }
void CException::Dump() const { std::abort(); }
void CException::AssertValid() const { std::abort(); }
CWinThread* AFXAPI AfxGetThread() { return nullptr; }
CRuntimeClass g_automationMemoryExceptionClass{"CMemoryException",sizeof(CException),0xffff,nullptr,nullptr,nullptr,nullptr};
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ() { return &g_automationMemoryExceptionClass; }

// Model the recursive CRIT_TYPELIBCACHE lock used by the production lookup.
std::recursive_mutex g_typeCacheTestMutex;
thread_local int g_typeCacheLockDepth=0;
extern "C" void MS_ABI impl__AfxLockGlobals__YAXH_Z(int id) {
    if(id!=13)std::abort();g_typeCacheTestMutex.lock();++g_typeCacheLockDepth;
}
extern "C" void MS_ABI impl__AfxUnlockGlobals__YAXH_Z(int id) {
    if(id!=13 || g_typeCacheLockDepth<=0)std::abort();--g_typeCacheLockDepth;g_typeCacheTestMutex.unlock();
}

// Generic clone allocation uses the same malloc/free pair as MFC exports.
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(size_t bytes) { return std::malloc(bytes); }
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void* memory) { std::free(memory); }
// Generic clone virtuals retain peripheral runtime-class and command siblings.
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CCmdTarget__UEBAPEAUCRuntimeClass__XZ(const CCmdTarget*) { return &CCmdTarget::classCCmdTarget; }
// Command-map lookup is peripheral to the frame/view routing tests. Their
// callback records the real target, arguments and thread routing context.
int (*g_cmdRoutingMapHook)(CCmdTarget*,UINT,int,void*,void*)=nullptr;
extern "C" int MS_ABI impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CCmdTarget* target,UINT id,int code,void* extra,void* info) {
    if(g_cmdRoutingMapHook)return g_cmdRoutingMapHook(target,id,code,extra,info);
    std::abort();
}

// Native CWnd virtuals outside drawing/message-context coverage abort on use.
extern "C" void MS_ABI impl__PreSubclassWindow_CWnd__UEAAXXZ(CWnd* pThis) { std::abort(); }

extern "C" int MS_ABI impl__PreCreateWindow_CWnd__UEAAHAEAUtagCREATESTRUCTW___Z(
    CWnd* pThis, CREATESTRUCTW& cs) { std::abort(); }

extern "C" void MS_ABI impl__CalcWindowRect_CWnd__UEAAXPEAUtagRECT__I_Z(
    CWnd* pThis, RECT* lpClientRect, unsigned int nAdjustType) { std::abort(); }

extern "C" CMenu* MS_ABI impl__GetMenu_CWnd__UEBAPEAVCMenu__XZ(const CWnd* pThis) { std::abort(); }

extern "C" int MS_ABI impl__SetMenu_CWnd__UEAAHPEAVCMenu___Z(CWnd* pThis, CMenu* pMenu) { std::abort(); }

extern "C" LONGLONG MS_ABI impl__OnToolHitTest_CWnd__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
    const CWnd* pThis, CPoint point, TOOLINFOW* pTI) { std::abort(); }

extern "C" CScrollBar* MS_ABI impl__GetScrollBarCtrl_CWnd__UEBAPEAVCScrollBar__H_Z(const CWnd* pThis, int nBar) { std::abort(); }

extern "C" void MS_ABI impl__WinHelpW_CWnd__UEAAX_KI_Z(CWnd* pThis, unsigned long long dwData, unsigned int nCmd) { std::abort(); }

extern "C" void MS_ABI impl__HtmlHelpW_CWnd__UEAAX_KI_Z(
    CWnd* pThis, unsigned long long data, unsigned int command) { std::abort(); }

extern "C" void MS_ABI impl__WinHelpInternal_CWnd__UEAAX_KI_Z(CWnd* pThis, unsigned long long dwData, unsigned int nCmd) { std::abort(); }

extern "C" int MS_ABI impl__ContinueModal_CWnd__UEAAHXZ(CWnd* pThis) { std::abort(); }

extern "C" void MS_ABI impl__EndModalLoop_CWnd__UEAAXH_Z(
    CWnd* pThis, int nResult) { std::abort(); }

extern "C" void MS_ABI impl__OnDrawIconicThumbnailOrLivePreview_CWnd__UEAAXAEAVCDC__VCRect__VCSize__HAEAH_Z(
    CWnd* pThis, CDC* pDC, CRect rc, CSize sz, int bDwmEnabled, int* bUpdateDwm) { std::abort(); }

extern "C" int MS_ABI impl__OnAmbientProperty_CWnd__UEAAHPEAVCOleControlSite__JPEAUtagVARIANT___Z(
    CWnd* pThis, COleControlSite* pSite, long dispid, VARIANT* pVar) { std::abort(); }

extern "C" int MS_ABI impl__CheckAutoCenter_CWnd__UEAAHXZ(CWnd* pThis) { std::abort(); }

extern "C" int MS_ABI impl__IsFrameWnd_CWnd__UEBAHXZ(const CWnd* pThis) { std::abort(); }

extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CWnd__MEBAPEBUAFX_MSGMAP__XZ(const void*) { std::abort(); }

extern "C" int MS_ABI impl__CreateControlSite_CWnd__MEAAHPEAVCOleControlContainer__PEAPEAVCOleControlSite__IAEBU_GUID___Z(
    CWnd* pThis,
    COleControlContainer* pContainer,
    COleControlSite** ppSite,
    unsigned int nID,
    const GUID* pClsid) { std::abort(); }

extern "C" int MS_ABI impl__OnTouchInputs_CWnd__MEAAHIPEAUtagTOUCHINPUT___Z(
    CWnd* pThis, unsigned int nInputs, TOUCHINPUT* pInputs) { std::abort(); }

extern "C" int MS_ABI impl__OnTouchInput_CWnd__MEAAHVCPoint__HHPEAUtagTOUCHINPUT___Z(
    CWnd* pThis, CPoint pt, int nInputNumber, int nTouchInputs, TOUCHINPUT* pInput) { std::abort(); }

extern "C" int MS_ABI impl__OnGestureZoom_CWnd__MEAAHVCPoint__J_Z(
    CWnd* pThis, CPoint pt, long dwFactor) { std::abort(); }

extern "C" int MS_ABI impl__OnGesturePan_CWnd__MEAAHVCPoint__0_Z(
    CWnd* pThis, CPoint pt, CPoint pt2) { std::abort(); }

extern "C" int MS_ABI impl__OnGestureRotate_CWnd__MEAAHVCPoint__N_Z(
    CWnd* pThis, CPoint pt, double dblRotate) { std::abort(); }

extern "C" int MS_ABI impl__OnGestureTwoFingerTap_CWnd__MEAAHVCPoint___Z(
    CWnd* pThis, CPoint pt) { std::abort(); }

extern "C" int MS_ABI impl__OnGesturePressAndTap_CWnd__MEAAHVCPoint__J_Z(
    CWnd* pThis, CPoint pt, long dwExtraInfo) { std::abort(); }

extern "C" int MS_ABI impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(CWnd* pThis, MSG* pMsg) { std::abort(); }

DWORD CWnd::GetGestureStatus(CPoint) { std::abort(); }
void CWnd::ResizeDynamicLayout() { std::abort(); }
BOOL CWnd::CreateControlContainer(COleControlContainer**) { std::abort(); }
BOOL CWnd::SetOccDialogInfo(_AFX_OCC_DIALOG_INFO*) { std::abort(); }
_AFX_OCC_DIALOG_INFO* CWnd::GetOccDialogInfo() { std::abort(); }
BOOL CWnd::OnChildNotify(UINT,WPARAM,LPARAM,LRESULT*) { std::abort(); }
void CWnd::PostNcDestroy() { std::abort(); }
WNDPROC* CWnd::GetSuperWndProcAddr() { static WNDPROC procedure=nullptr;return &procedure; }
extern "C" LRESULT MS_ABI impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(CWnd*,UINT,WPARAM,LPARAM);
LRESULT CWnd::DefWindowProcW(UINT message,WPARAM wparam,LPARAM lparam) {
    return impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(this,message,wparam,lparam);
}
// Sent-message dispatch uses these peripheral framework siblings.
LRESULT (*g_wndExceptionHandler)(CException*,const MSG*)=nullptr;
extern "C" LRESULT MS_ABI impl__AfxProcessWndProcException__YA_JPEAVCException__PEBUtagMSG___Z(CException* error,const MSG* message) {
    if(g_wndExceptionHandler)return g_wndExceptionHandler(error,message);std::abort();
}
extern "C" void MS_ABI impl__CenterWindow_CWnd__QEAAXPEAV1__Z(CWnd*,CWnd*) { std::abort(); }
extern "C" int MS_ABI impl__Attach_CWnd__QEAAHPEAUHWND_____Z(CWnd* window,HWND hwnd) { window->m_hWnd=hwnd;return TRUE; }

extern "C" int MS_ABI impl__OnCommand_CWnd__MEAAH_K_J_Z(CWnd*, WPARAM, LPARAM) { std::abort(); }
extern "C" int MS_ABI impl__OnNotify_CWnd__MEAAH_K_JPEA_J_Z(CWnd*, WPARAM, LPARAM,LRESULT*) { std::abort(); }
