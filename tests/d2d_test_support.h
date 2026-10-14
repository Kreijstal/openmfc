#pragma once
#include "openmfc/afxinet.h"
#include "openmfc/afxstat_.h"
#include <cstdlib>

// Unused GDI siblings in the shared support TU still need link definitions.
// Abort if a drawing path unexpectedly reaches one of these test doubles.
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
CWinThread* g_dialogHelpThread=nullptr;
CWinThread* AFXAPI AfxGetThread() { return g_dialogHelpThread; }
CRuntimeClass g_automationMemoryExceptionClass{"CMemoryException",sizeof(CException),0xffff,nullptr,nullptr,nullptr,nullptr};
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ() { return &g_automationMemoryExceptionClass; }

// Model the recursive CRIT_TYPELIBCACHE lock used by the production lookup.
std::recursive_mutex g_typeCacheTestMutex;
thread_local int g_typeCacheLockDepth=0;
std::recursive_mutex g_messageCacheTestMutex;
thread_local int g_messageCacheLockDepth=0;
std::recursive_mutex g_databaseTestMutex;
thread_local int g_databaseLockDepth=0;
extern "C" void MS_ABI impl__AfxLockGlobals__YAXH_Z(int id) {
    if(id==7) { g_messageCacheTestMutex.lock();++g_messageCacheLockDepth;return; }
    if(id==15) {g_databaseTestMutex.lock();++g_databaseLockDepth;return;}
    if(id!=13)std::abort();g_typeCacheTestMutex.lock();++g_typeCacheLockDepth;
}
extern "C" void MS_ABI impl__AfxUnlockGlobals__YAXH_Z(int id) {
    if(id==7) { if(g_messageCacheLockDepth<=0)std::abort();--g_messageCacheLockDepth;g_messageCacheTestMutex.unlock();return; }
    if(id==15) {if(g_databaseLockDepth<=0)std::abort();--g_databaseLockDepth;g_databaseTestMutex.unlock();return;}
    if(id!=13 || g_typeCacheLockDepth<=0)std::abort();--g_typeCacheLockDepth;g_typeCacheTestMutex.unlock();
}

// Generic clone allocation uses the same malloc/free pair as MFC exports.
bool g_testGenericAllocationFailure=false;
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(size_t bytes) { return g_testGenericAllocationFailure ? nullptr : std::malloc(bytes); }
int g_testGenericFrees=0;
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void* memory) { if(memory)++g_testGenericFrees;std::free(memory); }
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


extern "C" int MS_ABI impl__CreateControlSite_CWnd__MEAAHPEAVCOleControlContainer__PEAPEAVCOleControlSite__IAEBU_GUID___Z(
    CWnd* pThis,
    COleControlContainer* pContainer,
    COleControlSite** ppSite,
    unsigned int nID,
    const GUID* pClsid) { std::abort(); }



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

// Runtime descriptors and handle lookup are peripheral to dynamic layout.
// The layout storage, resource parser and all positioning remain production.
CRuntimeClass& g_layoutDialog=CDialog::classCDialog;
CRuntimeClass g_layoutSheet{"CPropertySheet",sizeof(CWnd),0xffff,nullptr,&CWnd::GetThisClass,nullptr,nullptr};
CRuntimeClass g_layoutPage{"CPropertyPage",sizeof(CWnd),0xffff,nullptr,&CWnd::GetThisClass,nullptr,nullptr};
CRuntimeClass g_layoutMfcPage{"CMFCPropertyPage",sizeof(CWnd),0xffff,nullptr,nullptr,nullptr,nullptr};
CRuntimeClass g_layoutMfcSheet{"CMFCPropertySheet",0x71e0,0xffff,nullptr,nullptr,nullptr,nullptr};
CRuntimeClass g_layoutForm{"CFormView",sizeof(CWnd),0xffff,nullptr,&CWnd::GetThisClass,nullptr,nullptr};
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDialog__SAPEAUCRuntimeClass__XZ() { return CDialog::GetThisClass(); }
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPropertySheet__SAPEAUCRuntimeClass__XZ() { return &g_layoutSheet; }
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPropertyPage__SAPEAUCRuntimeClass__XZ() { return &g_layoutPage; }
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCPropertyPage__SAPEAUCRuntimeClass__XZ() { return &g_layoutMfcPage; }
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCPropertySheet__SAPEAUCRuntimeClass__XZ() { return &g_layoutMfcSheet; }
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFormView__SAPEAUCRuntimeClass__XZ() { return &g_layoutForm; }
CWnd* g_layoutParent=nullptr;
CWnd* (*g_messageWindowLookup)(HWND)=nullptr;
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND handle) {
    if(g_messageWindowLookup)return g_messageWindowLookup(handle);
    if(!handle)return nullptr;
    if(g_layoutParent && g_layoutParent->m_hWnd==handle)return g_layoutParent;
    std::abort();
}
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND handle) {
    return g_messageWindowLookup ? g_messageWindowLookup(handle) : nullptr;
}
// Menu/GDI map ownership is peripheral to window argument decoding.
CMenu* g_messageMenu=nullptr;
CRuntimeClass CMenu::classCMenu{};
extern "C" CMenu* MS_ABI impl__FromHandle_CMenu__SAPEAV1_PEAUHMENU_____Z(HMENU handle) {
    if(!handle)return nullptr;
    if(g_messageMenu && g_messageMenu->m_hMenu==handle)return g_messageMenu;
    std::abort();
}
extern "C" CGdiObject* MS_ABI impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z(HGDIOBJ handle) {
    return GetTempGdiObject(handle);
}

// Toolbar get/set siblings are peripheral to CToolCmdUI's state translation.
UINT g_toolCommandStyle=0;int g_toolCommandIndex=-1;
extern "C" UINT MS_ABI impl__GetButtonStyle_CToolBar__QEBAIH_Z(const void*,int index) { g_toolCommandIndex=index;return g_toolCommandStyle; }
extern "C" void MS_ABI impl__SetButtonStyle_CToolBar__QEAAXHI_Z(void*,int index,UINT style) { g_toolCommandIndex=index;g_toolCommandStyle=style; }

// Dialog tests exercise exchange/exception behavior; the user-facing message box
// is captured so framework failures cannot open a blocking modal UI.
int g_exchangeMessageBoxes=0;
UINT g_exchangeMessageID=0,g_exchangeMessageFlags=0;
extern "C" int MS_ABI impl__AfxMessageBox__YAHIII_Z(UINT id,UINT flags,UINT) { ++g_exchangeMessageBoxes;g_exchangeMessageID=id;g_exchangeMessageFlags=flags;return IDOK; }
extern "C" void MS_ABI impl__AfxThrowNotSupportedException__YAXXZ() { throw new CException; }
extern "C" int MS_ABI impl__ReportError_CException__UEAAHII_Z(CException*,UINT,UINT) { std::abort(); }

// Array/map serialization is peripheral to the DLGINIT storage tests.
UINT CArchive::Read(void*,UINT) {std::abort();}
void CArchive::Write(const void*,UINT) {std::abort();}
CArchive& CArchive::operator<<(unsigned long) {std::abort();}
CArchive& CArchive::operator>>(unsigned long&) {std::abort();}
// Control subclassing is modeled at its attachment boundary; container ownership,
// map storage and Win32 DLGINIT message delivery are all production code.
BOOL g_containerSubclassResult=TRUE;
extern "C" int MS_ABI impl__SubclassWindow_CWnd__QEAAHPEAUHWND_____Z(CWnd* control,HWND window) {if(g_containerSubclassResult)control->m_hWnd=window;return g_containerSubclassResult;}
extern "C" HWND MS_ABI impl__UnsubclassWindow_CWnd__QEAAPEAUHWND____XZ(CWnd* control) {HWND window=control->m_hWnd;control->m_hWnd=nullptr;return window;}
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* object,const CRuntimeClass* type) {return object->IsKindOf(type);}
CRuntimeClass g_containerShellClass{"CMFCShellListCtrl",sizeof(CWnd),0xffff,nullptr,&CWnd::GetThisClass,nullptr,nullptr};
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCShellListCtrl__SAPEAUCRuntimeClass__XZ() {return &g_containerShellClass;}

// Dialog creation/routing helpers are outside the focused initialization tests.
extern "C" INT_PTR MS_ABI impl__DoModal_CDialog__UEAA_JXZ(CDialog*) {std::abort();}
extern "C" BOOL MS_ABI impl__Create_CDialog__UEAAHPEB_WPEAVCWnd___Z(CDialog*,const wchar_t*,CWnd*) {std::abort();}
extern "C" BOOL MS_ABI impl__Create_CDialog__UEAAHI_PEAVCWnd___Z(CDialog*,UINT,CWnd*) {std::abort();}
extern "C" BOOL MS_ABI impl__CreateIndirect_CDialog__UEAAHPEAXPEAVCWnd___Z(CDialog*,HGLOBAL,CWnd*) {std::abort();}
extern "C" BOOL MS_ABI impl__CreateIndirect_CDialog__UEAAHPEBUDLGTEMPLATE__PEAVCWnd__PEAX_Z(CDialog*,const DLGTEMPLATE*,CWnd*,void*) {std::abort();}
extern "C" BOOL MS_ABI impl__PreTranslateMessage_CDialog__UEAAHPEAUtagMSG___Z(CDialog*,MSG*) {std::abort();}
extern "C" BOOL MS_ABI impl__OnCmdMsg_CDialog__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(CDialog*,UINT,int,void*,AFX_CMDHANDLERINFO*) {std::abort();}
extern "C" BOOL MS_ABI impl__CheckAutoCenter_CDialog__UEAAHXZ(CDialog*) {std::abort();}

extern "C" void MS_ABI impl__Delete_CWinThread__UEAAXXZ(CWinThread*) {std::abort();}
extern "C" BOOL MS_ABI impl__ProcessMessageFilter_CWinThread__UEAAHHPEAUtagMSG___Z(CWinThread*,int,MSG*) {std::abort();}
extern "C" LRESULT MS_ABI impl__ProcessWndProcException_CWinThread__UEAA_JPEAVCException__PEBUtagMSG___Z(CWinThread*,CException*,const MSG*) {std::abort();}

// Font picker tests use real font enumeration and item storage. These unrelated
// frame, application printer and global appearance boundaries remain explicit.
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ(){return nullptr;}
extern "C" BOOL MS_ABI impl__GetPrinterDeviceDefaults_CWinApp__QEAAHPEAUtagPDW___Z(CWinApp*,PRINTDLGW*){std::abort();}
extern "C" HDC MS_ABI impl__AfxCreateDC__YAPEAUHDC____PEAX0_Z(void*,void*){std::abort();}
extern "C" CFrameWnd* impl__g_pTopLevelFrame__3PEAVCFrameWnd__EA=nullptr;
extern "C" CFrameWnd* MS_ABI impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd*){std::abort();}
extern "C" CWnd* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* window){SetFocus(window->m_hWnd);return nullptr;}
extern "C" void MS_ABI impl__OnDestroy_CWnd__IEAAXXZ(CWnd*){}
extern "C" BOOL MS_ABI impl__OnChildNotify_CWnd__MEAAHI_K_JPEA_J_Z(CWnd*,UINT,WPARAM,LPARAM,LRESULT*){std::abort();}
CRuntimeClass g_fontTestToolbarClass{"CMFCToolBar",sizeof(CWnd),0xffff,nullptr,&CWnd::GetThisClass,nullptr,nullptr};
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ(){return &g_fontTestToolbarClass;}
extern "C" BYTE impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720]{};
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void*){std::abort();}

CArchive& CArchive::operator>>(int&){std::abort();}
CArchive& CArchive::operator<<(int){std::abort();}
CArchive& CArchive::operator>>(CString&){std::abort();}
CArchive& CArchive::operator<<(const CString&){std::abort();}
extern "C" UINT MS_ABI impl__Read_CArchive__QEAAIPEAXI_Z(CArchive*,void*,UINT){std::abort();}
extern "C" void MS_ABI impl__Write_CArchive__QEAAXPEBXI_Z(CArchive*,const void*,UINT){std::abort();}
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(const CObject*){return CObject::GetThisClass();}
extern "C" BOOL MS_ABI impl__Create_CComboBox__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(CComboBox*,DWORD,const RECT*,CWnd*,UINT){std::abort();}
extern "C" void MS_ABI impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const CWnd*,CString*){std::abort();}
extern "C" void MS_ABI impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(CWnd*,const wchar_t*){std::abort();}
extern "C" BOOL MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd*){std::abort();}
extern "C" void MS_ABI impl___1CWnd__UEAA_XZ(CWnd*){std::abort();}
extern "C" int MS_ABI impl__GetRowHeight_CMFCToolBar__UEBAHXZ(const void*){std::abort();}
extern "C" BOOL MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(CWnd*,const CWnd*,int,int,int,int,UINT){std::abort();}
extern "C" void MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd*,RECT*){std::abort();}
extern "C" BOOL MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd*,int){std::abort();}
extern "C" DWORD MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd*){std::abort();}
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void*){std::abort();}
extern "C" BOOL MS_ABI impl__EnableWindow_CWnd__QEAAHH_Z(CWnd*,BOOL){std::abort();}
extern "C" DWORD MS_ABI impl__GetHotTextColor_CMFCToolBar__SAKXZ(){std::abort();}
extern "C" void* impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA=nullptr;
extern "C" void MS_ABI impl__OnDrawComboBorder_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(void*,CDC*,CRect,BOOL,BOOL,BOOL,void*){std::abort();}
extern "C" void MS_ABI impl__OnDrawComboDropButton_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(void*,CDC*,CRect,BOOL,BOOL,BOOL,void*){std::abort();}
extern "C" BOOL MS_ABI impl__GetCommandButtons_CMFCToolBar__SAHIAEAVCObList___Z(UINT,CObList*){std::abort();}
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBarComboBoxButton__SAPEAUCRuntimeClass__XZ(){return CMFCToolBarComboBoxButton::GetThisClass();}
extern "C" BOOL MS_ABI impl__AddTool_CToolTipCtrl__QEAAHPEAVCWnd__PEB_WPEBUtagRECT___K_Z(void*,CWnd*,const wchar_t*,const RECT*,UINT_PTR){std::abort();}
// Tag-manager toolbar-image/style parsing is outside the font property path.
CMFCControlRendererInfo::CMFCControlRendererInfo(){std::abort();}
CMFCControlRendererInfo::~CMFCControlRendererInfo(){std::abort();}
BOOL CMFCToolBarImages::Load(UINT,HINSTANCE,BOOL){std::abort();}
BOOL CMFCToolBarImages::Load(const wchar_t*,DWORD){std::abort();}
BOOL CMFCToolBarImages::LoadStr(const wchar_t*,HINSTANCE,BOOL){std::abort();}

extern "C" void MS_ABI impl__Serialize_CMFCToolBarButton__UEAAXAEAVCArchive___Z(void*,CArchive*){std::abort();}
extern "C" CSize MS_ABI impl__OnCalculateSize_CMFCToolBarButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(void*,CDC*,const CSize*,BOOL){std::abort();}
extern "C" void MS_ABI impl__OnChangeParentWnd_CMFCToolBarButton__UEAAXPEAVCWnd___Z(void*,CWnd*){std::abort();}
extern "C" void MS_ABI impl__OnDraw_CMFCToolBarButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(void*,CDC*,const CRect*,void*,BOOL,BOOL,BOOL,BOOL,BOOL){std::abort();}
extern "C" BOOL MS_ABI impl__OnDrawOnCustomizeList_CMFCToolBarButton__UEAAHPEAVCDC__AEBVCRect__H_Z(void*,CDC*,const CRect*,BOOL){std::abort();}
extern "C" BOOL MS_ABI impl__SetACCData_CMFCToolBarButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(void*,CWnd*,void*){std::abort();}
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(void*,const void*){std::abort();}
