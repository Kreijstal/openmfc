// Distinct view-map identities and native base links. The base-view message
// entries remain pending; record commands live in CRecordViewCommands.cpp.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#ifndef MS_ABI
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
#endif
namespace { const AFX_MSGMAP_ENTRY endEntry[]={{0,0,0,0,AfxSig_end,nullptr}}; }
// Symbol: ?GetMessageMap@CView@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CView__MEBAPEBUAFX_MSGMAP__XZ(const CView*) { return CView::GetThisMessageMap(); }
// Symbol: ?GetThisMessageMap@CView@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CView__KAPEBUAFX_MSGMAP__XZ() { return CView::GetThisMessageMap(); }
const AFX_MSGMAP* CView::GetThisMessageMap() {
    static const AFX_MSGMAP map={&CWnd::GetThisMessageMap,endEntry};
    return &map;
}
const AFX_MSGMAP* CView::GetMessageMap() const { return CView::GetThisMessageMap(); }
// Symbol: ?GetMessageMap@CScrollView@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CScrollView__MEBAPEBUAFX_MSGMAP__XZ(const CScrollView*) { return CScrollView::GetThisMessageMap(); }
// Symbol: ?GetThisMessageMap@CScrollView@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CScrollView__KAPEBUAFX_MSGMAP__XZ() { return CScrollView::GetThisMessageMap(); }
const AFX_MSGMAP* CScrollView::GetThisMessageMap() {
    static const AFX_MSGMAP map={&CView::GetThisMessageMap,endEntry};
    return &map;
}
const AFX_MSGMAP* CScrollView::GetMessageMap() const { return CScrollView::GetThisMessageMap(); }
// Symbol: ?GetMessageMap@CFormView@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CFormView__MEBAPEBUAFX_MSGMAP__XZ(const CFormView*) { return CFormView::GetThisMessageMap(); }
// Symbol: ?GetThisMessageMap@CFormView@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CFormView__KAPEBUAFX_MSGMAP__XZ() { return CFormView::GetThisMessageMap(); }
const AFX_MSGMAP* CFormView::GetThisMessageMap() {
    static const AFX_MSGMAP map={&CScrollView::GetThisMessageMap,endEntry};
    return &map;
}
const AFX_MSGMAP* CFormView::GetMessageMap() const { return CFormView::GetThisMessageMap(); }
