// Native x64 CRecordView commands (mfc140u 0x204500..0x2048d0).
#define OPENMFC_APPCORE_IMPL
#include "detail/CRecordsetSupport.h"
#include "detail/WindowMessageSupport.h"
extern "C" void MS_ABI impl__OnInitialUpdate_CFormView__UEAAXXZ(CFormView*);
namespace {
// CWaitCursor calls the application's virtual DoWaitCursor (SDK slot 48),
// including when opening throws. Fetch the current app again on destruction.
void WaitCursor(int code) {
    void* app=*reinterpret_cast<void**>(reinterpret_cast<char*>(impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ())+8);
    using Fn=void (MS_ABI*)(void*,int);
    openmfc::detail::WindowSlot<Fn>(app,48)(app,code);
}
struct Cursor { Cursor(){WaitCursor(1);} ~Cursor(){WaitCursor(-1);} };
using Enable=void (MS_ABI*)(CCmdUI*,BOOL);
Enable EnableHandler(CCmdUI* ui) { return openmfc::detail::WindowSlot<Enable>(ui,0); }
}
BOOL CRecordView::IsOnFirstRecord() { return OnGetRecordset()->m_lCurrentRecord==0; }
BOOL CRecordView::IsOnLastRecord() {
    CRecordset* set=OnGetRecordset();
    return set->m_bEOFSeen && static_cast<long>(static_cast<DWORD>(set->m_lCurrentRecord)+1)==set->m_lRecordCount;
}
BOOL CRecordView::OnMove(UINT command) {
    CRecordset* set=OnGetRecordset();
    if(set->CanUpdate() && !set->IsDeleted()) {
        set->Edit();
        if(!UpdateData(TRUE))return TRUE;
        set->Update();
    }
    switch(command) {
    case 0xe900:set->Move(1,SQL_FETCH_FIRST);break;
    case 0xe901:set->Move(-1,SQL_FETCH_LAST);break;
    case 0xe902:
        set->Move(1,SQL_FETCH_NEXT);
        if(set->IsEOF()) {
            if(set->CanScroll())set->Move(-1,SQL_FETCH_LAST);
            else set->SetFieldNull(nullptr,TRUE);
        }
        break;
    case 0xe903:
        set->Move(-1,SQL_FETCH_PRIOR);
        if(set->IsBOF())set->Move(1,SQL_FETCH_FIRST);
        break;
    }
    UpdateData(FALSE);
    return TRUE;
}
void CRecordView::OnInitialUpdate() {
    CRecordset* set=OnGetRecordset();
    if(!set->IsOpen()) { Cursor cursor;set->Open(AFX_DB_USE_DEFAULT_TYPE,nullptr,0); }
    impl__OnInitialUpdate_CFormView__UEAAXXZ(this);
}
void CRecordView::OnUpdateRecordFirst(CCmdUI* ui) {
    CRecordset* set=OnGetRecordset();Enable enable=EnableHandler(ui);
    enable(ui,set->IsOpen() && set->CanScroll() && !(set->IsEOF() && set->IsBOF()) && !IsOnFirstRecord());
}
void CRecordView::OnUpdateRecordPrev(CCmdUI* ui) { CRecordView::OnUpdateRecordFirst(ui); }
void CRecordView::OnUpdateRecordNext(CCmdUI* ui) {
    CRecordset* set=OnGetRecordset();Enable enable=EnableHandler(ui);
    enable(ui,set->IsOpen() && !(set->IsEOF() && set->IsBOF()) && !IsOnLastRecord());
}
void CRecordView::OnUpdateRecordLast(CCmdUI* ui) {
    CRecordset* set=OnGetRecordset();Enable enable=EnableHandler(ui);
    enable(ui,set->IsOpen() && set->CanScroll() && !(set->IsEOF() && set->IsBOF()) && !IsOnLastRecord());
}
// The eight command entries are separate, matching native handler discovery.
BEGIN_MESSAGE_MAP(CRecordView,CFormView)
    ON_COMMAND_EX(0xe900,&CRecordView::OnMove)
    ON_UPDATE_COMMAND_UI(0xe900,&CRecordView::OnUpdateRecordFirst)
    ON_COMMAND_EX(0xe903,&CRecordView::OnMove)
    ON_UPDATE_COMMAND_UI(0xe903,&CRecordView::OnUpdateRecordPrev)
    ON_COMMAND_EX(0xe902,&CRecordView::OnMove)
    ON_UPDATE_COMMAND_UI(0xe902,&CRecordView::OnUpdateRecordNext)
    ON_COMMAND_EX(0xe901,&CRecordView::OnMove)
    ON_UPDATE_COMMAND_UI(0xe901,&CRecordView::OnUpdateRecordLast)
END_MESSAGE_MAP()
// Symbol: ?GetMessageMap@CRecordView@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CRecordView__MEBAPEBUAFX_MSGMAP__XZ(const CRecordView* self) { return self->CRecordView::GetMessageMap(); }
// Symbol: ?GetThisMessageMap@CRecordView@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CRecordView__KAPEBUAFX_MSGMAP__XZ() { return CRecordView::GetThisMessageMap(); }
// Symbol: ?IsOnFirstRecord@CRecordView@@QEAAHXZ
extern "C" BOOL MS_ABI impl__IsOnFirstRecord_CRecordView__QEAAHXZ(CRecordView* self) { return self->CRecordView::IsOnFirstRecord(); }
// Symbol: ?IsOnLastRecord@CRecordView@@QEAAHXZ
extern "C" BOOL MS_ABI impl__IsOnLastRecord_CRecordView__QEAAHXZ(CRecordView* self) { return self->CRecordView::IsOnLastRecord(); }
// Symbol: ?OnMove@CRecordView@@UEAAHI@Z
extern "C" BOOL MS_ABI impl__OnMove_CRecordView__UEAAHI_Z(CRecordView* self,UINT command) { return self->CRecordView::OnMove(command); }
// Symbol: ?OnInitialUpdate@CRecordView@@UEAAXXZ
extern "C" void MS_ABI impl__OnInitialUpdate_CRecordView__UEAAXXZ(CRecordView* self) { self->CRecordView::OnInitialUpdate(); }
// Symbol: ?OnUpdateRecordFirst@CRecordView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecordFirst_CRecordView__IEAAXPEAVCCmdUI___Z(CRecordView* self,CCmdUI* ui) { self->OnUpdateRecordFirst(ui); }
// Symbol: ?OnUpdateRecordPrev@CRecordView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecordPrev_CRecordView__IEAAXPEAVCCmdUI___Z(CRecordView* self,CCmdUI* ui) { self->OnUpdateRecordPrev(ui); }
// Symbol: ?OnUpdateRecordNext@CRecordView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecordNext_CRecordView__IEAAXPEAVCCmdUI___Z(CRecordView* self,CCmdUI* ui) { self->OnUpdateRecordNext(ui); }
// Symbol: ?OnUpdateRecordLast@CRecordView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecordLast_CRecordView__IEAAXPEAVCCmdUI___Z(CRecordView* self,CCmdUI* ui) { self->OnUpdateRecordLast(ui); }
