// C++ virtual entry points in native CWnd slot order; ABI bodies retain ownership.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" void MS_ABI impl__PreSubclassWindow_CWnd__UEAAXXZ(CWnd* pThis);
extern "C" int MS_ABI impl__PreCreateWindow_CWnd__UEAAHAEAUtagCREATESTRUCTW___Z(
    CWnd* pThis, CREATESTRUCTW& cs);
extern "C" void MS_ABI impl__CalcWindowRect_CWnd__UEAAXPEAUtagRECT__I_Z(
    CWnd* pThis, RECT* lpClientRect, unsigned int nAdjustType);
extern "C" CMenu* MS_ABI impl__GetMenu_CWnd__UEBAPEAVCMenu__XZ(const CWnd* pThis);
extern "C" int MS_ABI impl__SetMenu_CWnd__UEAAHPEAVCMenu___Z(CWnd* pThis, CMenu* pMenu);
extern "C" LONGLONG MS_ABI impl__OnToolHitTest_CWnd__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
    const CWnd* pThis, CPoint point, TOOLINFOW* pTI);
extern "C" CScrollBar* MS_ABI impl__GetScrollBarCtrl_CWnd__UEBAPEAVCScrollBar__H_Z(const CWnd* pThis, int nBar);
extern "C" void MS_ABI impl__WinHelpW_CWnd__UEAAX_KI_Z(CWnd* pThis, unsigned long long dwData, unsigned int nCmd);
extern "C" void MS_ABI impl__HtmlHelpW_CWnd__UEAAX_KI_Z(
    CWnd* pThis, unsigned long long data, unsigned int command);
extern "C" void MS_ABI impl__WinHelpInternal_CWnd__UEAAX_KI_Z(CWnd* pThis, unsigned long long dwData, unsigned int nCmd);
extern "C" int MS_ABI impl__ContinueModal_CWnd__UEAAHXZ(CWnd* pThis);
extern "C" void MS_ABI impl__EndModalLoop_CWnd__UEAAXH_Z(
    CWnd* pThis, int nResult);
extern "C" void MS_ABI impl__OnDrawIconicThumbnailOrLivePreview_CWnd__UEAAXAEAVCDC__VCRect__VCSize__HAEAH_Z(
    CWnd* pThis, CDC* pDC, CRect rc, CSize sz, int bDwmEnabled, int* bUpdateDwm);
extern "C" int MS_ABI impl__OnAmbientProperty_CWnd__UEAAHPEAVCOleControlSite__JPEAUtagVARIANT___Z(
    CWnd* pThis, COleControlSite* pSite, long dispid, VARIANT* pVar);
extern "C" int MS_ABI impl__CheckAutoCenter_CWnd__UEAAHXZ(CWnd* pThis);
extern "C" int MS_ABI impl__IsFrameWnd_CWnd__UEBAHXZ(const CWnd* pThis);
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CWnd__MEBAPEBUAFX_MSGMAP__XZ(const void*);
extern "C" int MS_ABI impl__CreateControlSite_CWnd__MEAAHPEAVCOleControlContainer__PEAPEAVCOleControlSite__IAEBU_GUID___Z(
    CWnd* pThis,
    COleControlContainer* pContainer,
    COleControlSite** ppSite,
    unsigned int nID,
    const GUID* pClsid);
extern "C" int MS_ABI impl__OnTouchInputs_CWnd__MEAAHIPEAUtagTOUCHINPUT___Z(
    CWnd* pThis, unsigned int nInputs, TOUCHINPUT* pInputs);
extern "C" int MS_ABI impl__OnTouchInput_CWnd__MEAAHVCPoint__HHPEAUtagTOUCHINPUT___Z(
    CWnd* pThis, CPoint pt, int nInputNumber, int nTouchInputs, TOUCHINPUT* pInput);
extern "C" int MS_ABI impl__OnGestureZoom_CWnd__MEAAHVCPoint__J_Z(
    CWnd* pThis, CPoint pt, long dwFactor);
extern "C" int MS_ABI impl__OnGesturePan_CWnd__MEAAHVCPoint__0_Z(
    CWnd* pThis, CPoint pt, CPoint pt2);
extern "C" int MS_ABI impl__OnGestureRotate_CWnd__MEAAHVCPoint__N_Z(
    CWnd* pThis, CPoint pt, double dblRotate);
extern "C" int MS_ABI impl__OnGestureTwoFingerTap_CWnd__MEAAHVCPoint___Z(
    CWnd* pThis, CPoint pt);
extern "C" int MS_ABI impl__OnGesturePressAndTap_CWnd__MEAAHVCPoint__J_Z(
    CWnd* pThis, CPoint pt, long dwExtraInfo);
extern "C" long MS_ABI impl__EnsureStdObj_CWnd__UEAAJXZ(CWnd* pThis);
extern "C" long MS_ABI impl__get_accParent_CWnd__UEAAJPEAPEAUIDispatch___Z(void* pThis, void* p0);
extern "C" long MS_ABI impl__get_accChildCount_CWnd__UEAAJPEAJ_Z(void* pThis, void* p0);
extern "C" long MS_ABI impl__get_accChild_CWnd__UEAAJUtagVARIANT__PEAPEAUIDispatch___Z(void* pThis, void* p0, void* p1);
extern "C" long MS_ABI impl__get_accHelpTopic_CWnd__UEAAJPEAPEA_WUtagVARIANT__PEAJ_Z(void* pThis, void* p0, void* p1, void* p2);
extern "C" long MS_ABI impl__accNavigate_CWnd__UEAAJJUtagVARIANT__PEAU2__Z(void* pThis, long p0, void* p1, void* p2);
extern "C" long MS_ABI impl__accSelect_CWnd__UEAAJJUtagVARIANT___Z(void* pThis, long p0, void* p1);
extern "C" long MS_ABI impl__accLocation_CWnd__UEAAJPEAJ000UtagVARIANT___Z(void* pThis, void* p0, void* p1, void* p2, void* p3, void* p4);
extern "C" long MS_ABI impl__accHitTest_CWnd__UEAAJJJPEAUtagVARIANT___Z(void* pThis, long p0, long p1, void* p2);
extern "C" long MS_ABI impl__accDoDefaultAction_CWnd__UEAAJUtagVARIANT___Z(void* pThis, void* p0);
extern "C" long MS_ABI impl__get_accName_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, void* p0, void* p1);
extern "C" long MS_ABI impl__get_accValue_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, void* p0, void* p1);
extern "C" long MS_ABI impl__get_accDescription_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, void* p0, void* p1);
extern "C" long MS_ABI impl__get_accHelp_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, void* p0, void* p1);
extern "C" long MS_ABI impl__get_accKeyboardShortcut_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, void* p0, void* p1);
extern "C" long MS_ABI impl__get_accDefaultAction_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, void* p0, void* p1);
extern "C" long MS_ABI impl__get_accRole_CWnd__UEAAJUtagVARIANT__PEAU2__Z(void* pThis, void* p0, void* p1);
extern "C" long MS_ABI impl__get_accState_CWnd__UEAAJUtagVARIANT__PEAU2__Z(void* pThis, void* p0, void* p1);
extern "C" long MS_ABI impl__get_accFocus_CWnd__UEAAJPEAUtagVARIANT___Z(void* pThis, void* p0);
extern "C" long MS_ABI impl__get_accSelection_CWnd__UEAAJPEAUtagVARIANT___Z(void* pThis, void* p0);
extern "C" long MS_ABI impl__put_accName_CWnd__UEAAJUtagVARIANT__PEA_W_Z(void* pThis, void* p0, void* p1);
extern "C" long MS_ABI impl__put_accValue_CWnd__UEAAJUtagVARIANT__PEA_W_Z(void* pThis, void* p0, void* p1);
extern "C" long MS_ABI impl__SetProxy_CWnd__UEAAJPEAUIAccessibleProxy___Z(CWnd* window, void* proxy);
extern "C" long MS_ABI impl__CreateAccessibleProxy_CWnd__UEAAJ_K_JPEA_J_Z(
    CWnd* window, unsigned __int64 wParam, __int64 objectID, __int64* result);
extern "C" int MS_ABI impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(CWnd* pThis, MSG* pMsg);
void CWnd::PreSubclassWindow() { impl__PreSubclassWindow_CWnd__UEAAXXZ(this); }
BOOL CWnd::PreCreateWindow(CREATESTRUCTW& cs) { return impl__PreCreateWindow_CWnd__UEAAHAEAUtagCREATESTRUCTW___Z(this,cs); }
void CWnd::CalcWindowRect(RECT* rect,UINT adjust) { impl__CalcWindowRect_CWnd__UEAAXPEAUtagRECT__I_Z(this,rect,adjust); }
CMenu* CWnd::GetMenu() const { return impl__GetMenu_CWnd__UEBAPEAVCMenu__XZ(this); }
BOOL CWnd::SetMenu(CMenu* menu) { return impl__SetMenu_CWnd__UEAAHPEAVCMenu___Z(this,menu); }
INT_PTR CWnd::OnToolHitTest(CPoint point,TOOLINFOW* info) const { return impl__OnToolHitTest_CWnd__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(this,point,info); }
CScrollBar* CWnd::GetScrollBarCtrl(int bar) const { return impl__GetScrollBarCtrl_CWnd__UEBAPEAVCScrollBar__H_Z(this,bar); }
void CWnd::WinHelpW(ULONG_PTR data,UINT command) { impl__WinHelpW_CWnd__UEAAX_KI_Z(this,data,command); }
void CWnd::HtmlHelpW(ULONG_PTR data,UINT command) { impl__HtmlHelpW_CWnd__UEAAX_KI_Z(this,data,command); }
void CWnd::WinHelpInternal(ULONG_PTR data,UINT command) { impl__WinHelpInternal_CWnd__UEAAX_KI_Z(this,data,command); }
BOOL CWnd::ContinueModal() { return impl__ContinueModal_CWnd__UEAAHXZ(this); }
void CWnd::EndModalLoop(int result) { impl__EndModalLoop_CWnd__UEAAXH_Z(this,result); }
void CWnd::OnDrawIconicThumbnailOrLivePreview(CDC& dc,CRect rect,CSize size,BOOL enabled,BOOL& update) { impl__OnDrawIconicThumbnailOrLivePreview_CWnd__UEAAXAEAVCDC__VCRect__VCSize__HAEAH_Z(this,&dc,rect,size,enabled,&update); }
BOOL CWnd::OnAmbientProperty(COleControlSite* site,DISPID id,VARIANT* value) { return impl__OnAmbientProperty_CWnd__UEAAHPEAVCOleControlSite__JPEAUtagVARIANT___Z(this,site,id,value); }
BOOL CWnd::CheckAutoCenter() { return impl__CheckAutoCenter_CWnd__UEAAHXZ(this); }
BOOL CWnd::IsFrameWnd() const { return impl__IsFrameWnd_CWnd__UEBAHXZ(this); }
const AFX_MSGMAP* CWnd::GetMessageMap() const { return impl__GetMessageMap_CWnd__MEBAPEBUAFX_MSGMAP__XZ(this); }
BOOL CWnd::CreateControlSite(COleControlContainer* container,COleControlSite** site,UINT id,const GUID& clsid) { return impl__CreateControlSite_CWnd__MEAAHPEAVCOleControlContainer__PEAPEAVCOleControlSite__IAEBU_GUID___Z(this,container,site,id,&clsid); }
BOOL CWnd::OnTouchInputs(UINT count,TOUCHINPUT* inputs) { return impl__OnTouchInputs_CWnd__MEAAHIPEAUtagTOUCHINPUT___Z(this,count,inputs); }
BOOL CWnd::OnTouchInput(CPoint point,int index,int count,TOUCHINPUT* input) { return impl__OnTouchInput_CWnd__MEAAHVCPoint__HHPEAUtagTOUCHINPUT___Z(this,point,index,count,input); }
BOOL CWnd::OnGestureZoom(CPoint point,LONG value) { return impl__OnGestureZoom_CWnd__MEAAHVCPoint__J_Z(this,point,value); }
BOOL CWnd::OnGesturePan(CPoint point,CPoint end) { return impl__OnGesturePan_CWnd__MEAAHVCPoint__0_Z(this,point,end); }
BOOL CWnd::OnGestureRotate(CPoint point,double value) { return impl__OnGestureRotate_CWnd__MEAAHVCPoint__N_Z(this,point,value); }
BOOL CWnd::OnGestureTwoFingerTap(CPoint point) { return impl__OnGestureTwoFingerTap_CWnd__MEAAHVCPoint___Z(this,point); }
BOOL CWnd::OnGesturePressAndTap(CPoint point,LONG value) { return impl__OnGesturePressAndTap_CWnd__MEAAHVCPoint__J_Z(this,point,value); }
HRESULT CWnd::EnsureStdObj() { return impl__EnsureStdObj_CWnd__UEAAJXZ(this); }
HRESULT CWnd::get_accParent(IDispatch** out) { return impl__get_accParent_CWnd__UEAAJPEAPEAUIDispatch___Z(this,out); }
HRESULT CWnd::get_accChildCount(LONG* out) { return impl__get_accChildCount_CWnd__UEAAJPEAJ_Z(this,out); }
HRESULT CWnd::get_accChild(VARIANT child,IDispatch** out) { return impl__get_accChild_CWnd__UEAAJUtagVARIANT__PEAPEAUIDispatch___Z(this,&child,out); }
HRESULT CWnd::get_accHelpTopic(BSTR* out,VARIANT child,LONG* topic) { return impl__get_accHelpTopic_CWnd__UEAAJPEAPEA_WUtagVARIANT__PEAJ_Z(this,out,&child,topic); }
HRESULT CWnd::accNavigate(LONG direction,VARIANT child,VARIANT* out) { return impl__accNavigate_CWnd__UEAAJJUtagVARIANT__PEAU2__Z(this,direction,&child,out); }
HRESULT CWnd::accSelect(LONG flags,VARIANT child) { return impl__accSelect_CWnd__UEAAJJUtagVARIANT___Z(this,flags,&child); }
HRESULT CWnd::accLocation(LONG* left,LONG* top,LONG* width,LONG* height,VARIANT child) { return impl__accLocation_CWnd__UEAAJPEAJ000UtagVARIANT___Z(this,left,top,width,height,&child); }
HRESULT CWnd::accHitTest(LONG x,LONG y,VARIANT* out) { return impl__accHitTest_CWnd__UEAAJJJPEAUtagVARIANT___Z(this,x,y,out); }
HRESULT CWnd::accDoDefaultAction(VARIANT child) { return impl__accDoDefaultAction_CWnd__UEAAJUtagVARIANT___Z(this,&child); }
HRESULT CWnd::get_accName(VARIANT child,BSTR* out) { return impl__get_accName_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(this,&child,out); }
HRESULT CWnd::get_accValue(VARIANT child,BSTR* out) { return impl__get_accValue_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(this,&child,out); }
HRESULT CWnd::get_accDescription(VARIANT child,BSTR* out) { return impl__get_accDescription_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(this,&child,out); }
HRESULT CWnd::get_accHelp(VARIANT child,BSTR* out) { return impl__get_accHelp_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(this,&child,out); }
HRESULT CWnd::get_accKeyboardShortcut(VARIANT child,BSTR* out) { return impl__get_accKeyboardShortcut_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(this,&child,out); }
HRESULT CWnd::get_accDefaultAction(VARIANT child,BSTR* out) { return impl__get_accDefaultAction_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(this,&child,out); }
HRESULT CWnd::get_accRole(VARIANT child,VARIANT* out) { return impl__get_accRole_CWnd__UEAAJUtagVARIANT__PEAU2__Z(this,&child,out); }
HRESULT CWnd::get_accState(VARIANT child,VARIANT* out) { return impl__get_accState_CWnd__UEAAJUtagVARIANT__PEAU2__Z(this,&child,out); }
HRESULT CWnd::get_accFocus(VARIANT* out) { return impl__get_accFocus_CWnd__UEAAJPEAUtagVARIANT___Z(this,out); }
HRESULT CWnd::get_accSelection(VARIANT* out) { return impl__get_accSelection_CWnd__UEAAJPEAUtagVARIANT___Z(this,out); }
HRESULT CWnd::put_accName(VARIANT child,BSTR value) { return impl__put_accName_CWnd__UEAAJUtagVARIANT__PEA_W_Z(this,&child,value); }
HRESULT CWnd::put_accValue(VARIANT child,BSTR value) { return impl__put_accValue_CWnd__UEAAJUtagVARIANT__PEA_W_Z(this,&child,value); }
HRESULT CWnd::SetProxy(IAccessibleProxy* proxy) { return impl__SetProxy_CWnd__UEAAJPEAUIAccessibleProxy___Z(this,proxy); }
HRESULT CWnd::CreateAccessibleProxy(WPARAM wparam,LPARAM lparam,LRESULT* out) { return impl__CreateAccessibleProxy_CWnd__UEAAJ_K_JPEA_J_Z(this,wparam,lparam,out); }
BOOL CWnd::PreTranslateMessage(MSG* message) { return impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(this,message); }
void CWnd::DoDataExchange(CDataExchange*) {} // Retail folded no-op.
void CWnd::BeginModalState() { ::EnableWindow(m_hWnd,FALSE); }
void CWnd::EndModalState() { ::EnableWindow(m_hWnd,TRUE); }
