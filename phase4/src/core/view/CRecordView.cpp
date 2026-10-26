// Record-view lifetime follows CFormView. The native abstract constructors do
// not install a CRecordView vtable or initialize the two legacy record flags.
#define OPENMFC_APPCORE_IMPL
#include "detail/CRecordsetSupport.h"
extern "C" void MS_ABI impl___0CFormView__IEAA_I_Z(CFormView*,UINT);
extern "C" void MS_ABI impl___0CFormView__IEAA_PEB_W_Z(CFormView*,const wchar_t*);
extern "C" void MS_ABI impl___1CFormView__UEAA_XZ(CFormView*);
CRecordView::CRecordView(UINT id):CFormView(id) {}
CRecordView::CRecordView(const wchar_t* name):CFormView(name) {}
CRecordView::~CRecordView() {}
// Symbol: ??0CRecordView@@IEAA@I@Z
extern "C" CRecordView* MS_ABI impl___0CRecordView__IEAA_I_Z(CRecordView* self,UINT id) {
    impl___0CFormView__IEAA_I_Z(self,id);return self;
}
// Symbol: ??0CRecordView@@IEAA@PEB_W@Z
extern "C" CRecordView* MS_ABI impl___0CRecordView__IEAA_PEB_W_Z(CRecordView* self,const wchar_t* name) {
    impl___0CFormView__IEAA_PEB_W_Z(self,name);return self;
}
// Symbol: ??1CRecordView@@UEAA@XZ
extern "C" void MS_ABI impl___1CRecordView__UEAA_XZ(CRecordView* self) { impl___1CFormView__UEAA_XZ(self); }
// Symbol: ?GetRuntimeClass@CRecordView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CRecordView__UEBAPEAUCRuntimeClass__XZ(const CRecordView*) { return CRecordView::GetThisClass(); }
// Symbol: ?GetThisClass@CRecordView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CRecordView__SAPEAUCRuntimeClass__XZ() { return CRecordView::GetThisClass(); }
