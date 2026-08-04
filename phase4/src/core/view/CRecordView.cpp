// CRecordView — OpenMFC implementation.
// Sources: dbcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CRecordsetSupport.h"

// Symbol: ?GetRuntimeClass@CRecordView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CRecordView__UEBAPEAUCRuntimeClass__XZ(const CRecordView* pThis) {
    (void)pThis;
    return CRecordView::GetThisClass();
}
// Symbol: ?GetThisClass@CRecordView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CRecordView__SAPEAUCRuntimeClass__XZ() {
    return CRecordView::GetThisClass();
}
// Symbol: ?GetMessageMap@CRecordView@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CRecordView__MEBAPEBUAFX_MSGMAP__XZ(const CRecordView* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}
// Symbol: ?GetThisMessageMap@CRecordView@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CRecordView__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}
// Symbol: ?OnInitialUpdate@CRecordView@@UEAAXXZ
extern "C" void MS_ABI impl__OnInitialUpdate_CRecordView__UEAAXXZ(CRecordView* pThis) {
    if (!pThis) return;
    pThis->m_pSet = pThis->OnGetRecordset();
    pThis->m_bOnFirstRecord = TRUE;
    pThis->m_bOnLastRecord = FALSE;
}
// Symbol: ?OnUpdateRecordFirst@CRecordView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecordFirst_CRecordView__IEAAXPEAVCCmdUI___Z(CRecordView* pThis, CCmdUI* pCmdUI) {
    if (!pThis || !pThis->GetRecordset() || !pCmdUI) return;
    pCmdUI->Enable(!pThis->GetRecordset()->IsBOF());
}
// Symbol: ?OnUpdateRecordLast@CRecordView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecordLast_CRecordView__IEAAXPEAVCCmdUI___Z(CRecordView* pThis, CCmdUI* pCmdUI) {
    if (!pThis || !pThis->GetRecordset() || !pCmdUI) return;
    pCmdUI->Enable(!pThis->GetRecordset()->IsEOF());
}
// Symbol: ?OnUpdateRecordNext@CRecordView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecordNext_CRecordView__IEAAXPEAVCCmdUI___Z(CRecordView* pThis, CCmdUI* pCmdUI) {
    if (!pThis || !pThis->GetRecordset() || !pCmdUI) return;
    pCmdUI->Enable(!pThis->GetRecordset()->IsEOF());
}
// Symbol: ?OnUpdateRecordPrev@CRecordView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateRecordPrev_CRecordView__IEAAXPEAVCCmdUI___Z(CRecordView* pThis, CCmdUI* pCmdUI) {
    if (!pThis || !pThis->GetRecordset() || !pCmdUI) return;
    pCmdUI->Enable(!pThis->GetRecordset()->IsBOF());
}
CRecordView::CRecordView(UINT nIDTemplate)
    : CFormView(nIDTemplate), m_pSet(nullptr),
      m_bOnFirstRecord(FALSE), m_bOnLastRecord(FALSE) {
    memset(_crecordview_padding, 0, sizeof(_crecordview_padding));
}
CRecordView::CRecordView(const wchar_t* lpszTemplateName)
    : CFormView(lpszTemplateName), m_pSet(nullptr),
      m_bOnFirstRecord(FALSE), m_bOnLastRecord(FALSE) {
    memset(_crecordview_padding, 0, sizeof(_crecordview_padding));
}
CRecordView::~CRecordView() {
}
BOOL CRecordView::OnMove(UINT nIDMoveCommand) {
    if (!m_pSet || !m_pSet->IsOpen()) return FALSE;

    switch (nIDMoveCommand) {
    case 0xE900: // ID_RECORD_FIRST
        m_pSet->MoveFirst();
        break;
    case 0xE901: // ID_RECORD_LAST
        m_pSet->MoveLast();
        break;
    case 0xE902: // ID_RECORD_NEXT
        m_pSet->MoveNext();
        break;
    case 0xE903: // ID_RECORD_PREV
        m_pSet->MovePrev();
        break;
    default:
        return FALSE;
    }

    m_bOnFirstRecord = m_pSet->IsBOF() || m_pSet->GetAbsolutePosition() <= 1;
    m_bOnLastRecord = m_pSet->IsEOF() || (m_pSet->GetRecordCount() > 0 && m_pSet->GetAbsolutePosition() >= m_pSet->GetRecordCount());
    return TRUE;
}
CRecordset* CRecordView::OnGetRecordset() {
    return m_pSet;
}
BOOL CRecordView::IsOnFirstRecord() {
    return m_bOnFirstRecord;
}
BOOL CRecordView::IsOnLastRecord() {
    return m_bOnLastRecord;
}
