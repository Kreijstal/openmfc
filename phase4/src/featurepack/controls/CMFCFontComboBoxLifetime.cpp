#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxfontcombobox.h"
#include "detail/WindowMessageSupport.h"
#include <new>
#pragma comment(linker,"/alternatename:?m_bDrawUsingFont@CMFCFontComboBox@@2HA=impl__m_bDrawUsingFont_CMFCFontComboBox__2HA")
extern "C" {
BOOL MS_ABI impl__Setup_CMFCFontComboBox__QEAAHHEE_Z(void*,int,BYTE,BYTE);
BOOL MS_ABI impl__SelectFont_CMFCFontComboBox__QEAAHPEAVCMFCFontInfo___Z(void*,void*);
BOOL MS_ABI impl__SelectFont_CMFCFontComboBox__QEAAHPEB_WE_Z(void*,const wchar_t*,BYTE);
void* MS_ABI impl__GetSelFont_CMFCFontComboBox__QEBAPEAVCMFCFontInfo__XZ(const void*);
int MS_ABI impl__DeleteString_CMFCFontComboBox__QEAAHI_Z(void*,UINT);
int MS_ABI impl__CompareItem_CMFCFontComboBox__UEAAHPEAUtagCOMPAREITEMSTRUCT___Z(void*,COMPAREITEMSTRUCT*);
void MS_ABI impl__DrawItem_CMFCFontComboBox__UEAAXPEAUtagDRAWITEMSTRUCT___Z(void*,DRAWITEMSTRUCT*);
void MS_ABI impl__MeasureItem_CMFCFontComboBox__UEAAXPEAUtagMEASUREITEMSTRUCT___Z(void*,MEASUREITEMSTRUCT*);
BOOL MS_ABI impl__PreTranslateMessage_CMFCFontComboBox__UEAAHPEAUtagMSG___Z(void*,MSG*);
void MS_ABI impl__PreSubclassWindow_CMFCFontComboBox__MEAAXXZ(void*);
void MS_ABI impl__Init_CMFCFontComboBox__IEAAXXZ(void*);
void MS_ABI impl__CleanUp_CMFCFontComboBox__IEAAXXZ(void*);
int MS_ABI impl__OnCreate_CMFCFontComboBox__IEAAHPEAUtagCREATESTRUCTW___Z(void*,CREATESTRUCTW*);
void MS_ABI impl__OnDestroy_CMFCFontComboBox__IEAAXXZ(void*);
LRESULT MS_ABI impl__OnInitControl_CMFCFontComboBox__IEAA_J_K_J_Z(void*,WPARAM,LPARAM);
}
CMFCFontComboBox::CMFCFontComboBox():m_bToolBarMode(FALSE){}
CMFCFontComboBox::~CMFCFontComboBox()=default;
BOOL CMFCFontComboBox::Setup(int type,BYTE charset,BYTE pitch){return impl__Setup_CMFCFontComboBox__QEAAHHEE_Z(this,type,charset,pitch);}
BOOL CMFCFontComboBox::SelectFont(CMFCFontInfo* font){return impl__SelectFont_CMFCFontComboBox__QEAAHPEAVCMFCFontInfo___Z(this,font);}
BOOL CMFCFontComboBox::SelectFont(const wchar_t* name,BYTE charset){return impl__SelectFont_CMFCFontComboBox__QEAAHPEB_WE_Z(this,name,charset);}
CMFCFontInfo* CMFCFontComboBox::GetSelFont()const{return static_cast<CMFCFontInfo*>(impl__GetSelFont_CMFCFontComboBox__QEBAPEAVCMFCFontInfo__XZ(this));}
int CMFCFontComboBox::DeleteString(UINT index){return impl__DeleteString_CMFCFontComboBox__QEAAHI_Z(this,index);}
int CMFCFontComboBox::CompareItem(COMPAREITEMSTRUCT* item){return impl__CompareItem_CMFCFontComboBox__UEAAHPEAUtagCOMPAREITEMSTRUCT___Z(this,item);}
void CMFCFontComboBox::DrawItem(DRAWITEMSTRUCT* item){impl__DrawItem_CMFCFontComboBox__UEAAXPEAUtagDRAWITEMSTRUCT___Z(this,item);}
void CMFCFontComboBox::MeasureItem(MEASUREITEMSTRUCT* item){impl__MeasureItem_CMFCFontComboBox__UEAAXPEAUtagMEASUREITEMSTRUCT___Z(this,item);}
BOOL CMFCFontComboBox::PreTranslateMessage(MSG* message){return impl__PreTranslateMessage_CMFCFontComboBox__UEAAHPEAUtagMSG___Z(this,message);}
void CMFCFontComboBox::PreSubclassWindow(){impl__PreSubclassWindow_CMFCFontComboBox__MEAAXXZ(this);}
void CMFCFontComboBox::Init(){impl__Init_CMFCFontComboBox__IEAAXXZ(this);}
void CMFCFontComboBox::CleanUp(){impl__CleanUp_CMFCFontComboBox__IEAAXXZ(this);}
int CMFCFontComboBox::OnCreate(CREATESTRUCTW* create){return impl__OnCreate_CMFCFontComboBox__IEAAHPEAUtagCREATESTRUCTW___Z(this,create);}
void CMFCFontComboBox::OnDestroy(){impl__OnDestroy_CMFCFontComboBox__IEAAXXZ(this);}
LRESULT CMFCFontComboBox::OnInitControl(WPARAM size,LPARAM data){return impl__OnInitControl_CMFCFontComboBox__IEAA_J_K_J_Z(this,size,data);}
BEGIN_MESSAGE_MAP(CMFCFontComboBox,CComboBox)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_MESSAGE(0x37c,&CMFCFontComboBox::OnInitControl)
END_MESSAGE_MAP()
// Symbol: ??0CMFCFontComboBox@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCFontComboBox__QEAA_XZ(void* self){return new(self) CMFCFontComboBox;}
// Symbol: ??1CMFCFontComboBox@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCFontComboBox__UEAA_XZ(CMFCFontComboBox* self){self->~CMFCFontComboBox();}
// Symbol: ?GetMessageMap@CMFCFontComboBox@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CMFCFontComboBox__MEBAPEBUAFX_MSGMAP__XZ(const CMFCFontComboBox*){return CMFCFontComboBox::GetThisMessageMap();}
// Symbol: ?GetThisMessageMap@CMFCFontComboBox@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCFontComboBox__KAPEBUAFX_MSGMAP__XZ(){return CMFCFontComboBox::GetThisMessageMap();}
