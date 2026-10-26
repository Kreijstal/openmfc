// Release MFC initializes dialog exchange before the ordinary view update.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#ifndef MS_ABI
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
#endif
// Symbol: ?OnInitialUpdate@CFormView@@UEAAXXZ
extern "C" void MS_ABI impl__OnInitialUpdate_CFormView__UEAAXXZ(CFormView* self) {
    self->m_bInitDlgCompleted=TRUE;
    self->UpdateData(FALSE);
    self->OnUpdate(nullptr,0,nullptr);
}
void CFormView::OnInitialUpdate() { impl__OnInitialUpdate_CFormView__UEAAXXZ(this); }
