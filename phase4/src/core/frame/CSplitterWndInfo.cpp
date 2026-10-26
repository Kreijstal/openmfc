// Native inline row/column records and reference-output ABI.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
void CSplitterWnd::GetRowInfo(int row,int& current,int& minimum) const {current=m_pRowInfo[row].nCurSize;minimum=m_pRowInfo[row].nMinSize;}
void CSplitterWnd::GetColumnInfo(int col,int& current,int& minimum) const {current=m_pColInfo[col].nCurSize;minimum=m_pColInfo[col].nMinSize;}
void CSplitterWnd::SetRowInfo(int row,int ideal,int minimum) {m_pRowInfo[row].nIdealSize=ideal;m_pRowInfo[row].nMinSize=minimum;}
void CSplitterWnd::SetColumnInfo(int col,int ideal,int minimum) {m_pColInfo[col].nIdealSize=ideal;m_pColInfo[col].nMinSize=minimum;}
// Symbol: ?GetRowInfo@CSplitterWnd@@QEBAXHAEAH0@Z
extern "C" void MS_ABI impl__GetRowInfo_CSplitterWnd__QEBAXHAEAH0_Z(const CSplitterWnd* self,int row,int* current,int* minimum) {self->GetRowInfo(row,*current,*minimum);}
// Symbol: ?GetColumnInfo@CSplitterWnd@@QEBAXHAEAH0@Z
extern "C" void MS_ABI impl__GetColumnInfo_CSplitterWnd__QEBAXHAEAH0_Z(const CSplitterWnd* self,int col,int* current,int* minimum) {self->GetColumnInfo(col,*current,*minimum);}
// Symbol: ?SetRowInfo@CSplitterWnd@@QEAAXHHH@Z
extern "C" void MS_ABI impl__SetRowInfo_CSplitterWnd__QEAAXHHH_Z(CSplitterWnd* self,int row,int ideal,int minimum) {self->SetRowInfo(row,ideal,minimum);}
// Symbol: ?SetColumnInfo@CSplitterWnd@@QEAAXHHH@Z
extern "C" void MS_ABI impl__SetColumnInfo_CSplitterWnd__QEAAXHHH_Z(CSplitterWnd* self,int col,int ideal,int minimum) {self->SetColumnInfo(col,ideal,minimum);}
