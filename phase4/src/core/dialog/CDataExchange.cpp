// CDataExchange — OpenMFC implementation.
// Sources: global_cdataexchange.cpp

#include "detail/CDataExchangeSupport.h"

// Symbol: ??0CDataExchange@@QEAA@PEAVCWnd@@H@Z
// CDataExchange(CWnd* pDlgWnd, BOOL bSaveAndValidate)
extern "C" void* MS_ABI
impl___0CDataExchange__QEAA_PEAVCWnd__H_Z(void* pThis, void* pDlgWnd, BOOL bSaveAndValidate)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pThis);
    self->m_bSaveAndValidate = bSaveAndValidate;
    self->m_pDlgWnd = pDlgWnd;
    self->m_idLastControl = 0;
    self->m_bEditLastControl = FALSE;
    return pThis;
}
// Symbol: ?PrepareCtrl@CDataExchange@@QEAAPEAUHWND__@@H@Z
// HWND PrepareCtrl(int nIDC)
// Resolve the control window from the dialog, record it as the last-touched
// (non-edit) control, and hand back its HWND for the DDX routine.
extern "C" HWND MS_ABI
impl__PrepareCtrl_CDataExchange__QEAAPEAUHWND____H_Z(void* pThis, int nIDC)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pThis);
    HWND hWndCtrl = ::GetDlgItem(DlgHwnd(self), nIDC);
    self->m_idLastControl = static_cast<UINT>(nIDC);
    self->m_bEditLastControl = FALSE;  // not an edit item unless PrepareEditCtrl
    return hWndCtrl;
}
// Symbol: ?PrepareEditCtrl@CDataExchange@@QEAAPEAUHWND__@@H@Z
// HWND PrepareEditCtrl(int nIDC)
// Same as PrepareCtrl but flags the control as an edit item so that a later
// Fail() knows to select all its text.
extern "C" HWND MS_ABI
impl__PrepareEditCtrl_CDataExchange__QEAAPEAUHWND____H_Z(void* pThis, int nIDC)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pThis);
    HWND hWndCtrl = ::GetDlgItem(DlgHwnd(self), nIDC);
    self->m_idLastControl = static_cast<UINT>(nIDC);
    self->m_bEditLastControl = TRUE;
    return hWndCtrl;
}
// Symbol: ?PrepareOleCtrl@CDataExchange@@QEAAPEAVCOleControlSite@@H@Z
// COleControlSite* PrepareOleCtrl(int nIDC)
// Record the control id (non-edit) exactly like PrepareCtrl. The real routine
// then looks the id up in the dialog's OLE control container site map; when the
// dialog hosts no OLE control container (the common case) MFC returns NULL, so
// that is the faithful result here. The bookkeeping side-effects are real.
extern "C" void* MS_ABI
impl__PrepareOleCtrl_CDataExchange__QEAAPEAVCOleControlSite__H_Z(void* pThis, int nIDC)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pThis);
    self->m_idLastControl = static_cast<UINT>(nIDC);
    self->m_bEditLastControl = FALSE;
    return nullptr;
}
// Symbol: ?Fail@CDataExchange@@QEAAXXZ
// void Fail()
// Restore focus to the last control involved in the exchange (selecting all of
// its text if it was an edit control) and then abort the DDX pass by throwing.
extern "C" void MS_ABI
impl__Fail_CDataExchange__QEAAXXZ(void* pThis)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pThis);
    if (self->m_idLastControl != 0) {
        HWND hWndCtrl = ::GetDlgItem(DlgHwnd(self), self->m_idLastControl);
        if (hWndCtrl != nullptr) {
            ::SetFocus(hWndCtrl);
            if (self->m_bEditLastControl) {
                // Select the entire contents of the offending edit control.
                ::SendMessageW(hWndCtrl, EM_SETSEL, 0, static_cast<LPARAM>(-1));
            }
        }
    }
    // AfxThrowUserException(): never returns normally.
    impl__AfxThrowUserException__YAXXZ();
}

// Symbol: ?AfxFailMaxChars@@YAXPEAVCDataExchange@@H@Z
extern "C" void MS_ABI impl__AfxFailMaxChars__YAXPEAVCDataExchange__H_Z(void* /*class*/* p0, int p1) {}

// Symbol: ?AfxFailRadio@@YAXPEAVCDataExchange@@@Z
extern "C" void MS_ABI impl__AfxFailRadio__YAXPEAVCDataExchange___Z(void* /*class*/* p0) {}

// Symbol: ?AfxFieldText@@YAHPEAVCDataExchange@@HPEAXPEAVCRecordset@@@Z
extern "C" int MS_ABI impl__AfxFieldText__YAHPEAVCDataExchange__HPEAXPEAVCRecordset___Z(void* /*class*/* p0, int p1, void* p2, void* /*class*/* p3) {
    return 0;
}

// Symbol: ?AfxTextFloatFormat@@YAXPEAVCDataExchange@@HPEAXNH@Z
extern "C" void MS_ABI impl__AfxTextFloatFormat__YAXPEAVCDataExchange__HPEAXNH_Z(void* /*class*/* p0, int p1, void* p2, double p3, int p4) {}

// Symbol: ?DDV_MinMaxDateTime@@YAXPEAVCDataExchange@@AEAVCOleDateTime@ATL@@PEBV23@2@Z
extern "C" void MS_ABI impl__DDV_MinMaxDateTime__YAXPEAVCDataExchange__AEAVCOleDateTime_ATL__PEBV23_2_Z(void* /*class*/* p0, void* /*class*/* p1, const void* /*class*/* p2, void* p3, const void* /*class*/* p4) {}

// Symbol: ?DDV_MinMaxDateTime@@YAXPEAVCDataExchange@@AEAVCTime@ATL@@PEBV23@2@Z
extern "C" void MS_ABI impl__DDV_MinMaxDateTime__YAXPEAVCDataExchange__AEAVCTime_ATL__PEBV23_2_Z(void* /*class*/* p0, void* /*class*/* p1, const void* /*class*/* p2, void* p3, const void* /*class*/* p4) {}

// Symbol: ?DDV_MinMaxMonth@@YAXPEAVCDataExchange@@AEAVCOleDateTime@ATL@@PEBV23@2@Z
extern "C" void MS_ABI impl__DDV_MinMaxMonth__YAXPEAVCDataExchange__AEAVCOleDateTime_ATL__PEBV23_2_Z(void* /*class*/* p0, void* /*class*/* p1, const void* /*class*/* p2, void* p3, const void* /*class*/* p4) {}

// Symbol: ?DDV_MinMaxMonth@@YAXPEAVCDataExchange@@AEAVCTime@ATL@@PEBV23@2@Z
extern "C" void MS_ABI impl__DDV_MinMaxMonth__YAXPEAVCDataExchange__AEAVCTime_ATL__PEBV23_2_Z(void* /*class*/* p0, void* /*class*/* p1, const void* /*class*/* p2, void* p3, const void* /*class*/* p4) {}

// Symbol: ?DDX_DateTimeCtrl@@YAXPEAVCDataExchange@@HAEAUtagDBDATE@@@Z
extern "C" void MS_ABI impl__DDX_DateTimeCtrl__YAXPEAVCDataExchange__HAEAUtagDBDATE___Z(void* /*class*/* p0, int p1, void* /*struct*/* p2) {}

// Symbol: ?DDX_DateTimeCtrl@@YAXPEAVCDataExchange@@HAEAUtagDBTIME@@@Z
extern "C" void MS_ABI impl__DDX_DateTimeCtrl__YAXPEAVCDataExchange__HAEAUtagDBTIME___Z(void* /*class*/* p0, int p1, void* /*struct*/* p2) {}

// Symbol: ?DDX_DateTimeCtrl@@YAXPEAVCDataExchange@@HAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__DDX_DateTimeCtrl__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* /*class*/* p0, int p1, void* /*class*/* p2, void** p3, void* p4) {}

// Symbol: ?DDX_MonthCalCtrl@@YAXPEAVCDataExchange@@HAEAUtagDBDATE@@@Z
extern "C" void MS_ABI impl__DDX_MonthCalCtrl__YAXPEAVCDataExchange__HAEAUtagDBDATE___Z(void* /*class*/* p0, int p1, void* /*struct*/* p2) {}

// Symbol: ?DDX_OCBool@@YAXPEAVCDataExchange@@HJAEAH@Z
extern "C" void MS_ABI impl__DDX_OCBool__YAXPEAVCDataExchange__HJAEAH_Z(void* /*class*/* p0, int p1, long p2, int* p3) {}

// Symbol: ?DDX_OCBoolRO@@YAXPEAVCDataExchange@@HJAEAH@Z
extern "C" void MS_ABI impl__DDX_OCBoolRO__YAXPEAVCDataExchange__HJAEAH_Z(void* /*class*/* p0, int p1, long p2, int* p3) {}

// Symbol: ?DDX_OCColor@@YAXPEAVCDataExchange@@HJAEAK@Z
extern "C" void MS_ABI impl__DDX_OCColor__YAXPEAVCDataExchange__HJAEAK_Z(void* /*class*/* p0, int p1, long p2, unsigned long* p3) {}

// Symbol: ?DDX_OCColorRO@@YAXPEAVCDataExchange@@HJAEAK@Z
extern "C" void MS_ABI impl__DDX_OCColorRO__YAXPEAVCDataExchange__HJAEAK_Z(void* /*class*/* p0, int p1, long p2, unsigned long* p3) {}

// Symbol: ?DDX_OCFloat@@YAXPEAVCDataExchange@@HJAEAM@Z
extern "C" void MS_ABI impl__DDX_OCFloat__YAXPEAVCDataExchange__HJAEAM_Z(void* /*class*/* p0, int p1, long p2, float* p3) {}

// Symbol: ?DDX_OCFloat@@YAXPEAVCDataExchange@@HJAEAN@Z
extern "C" void MS_ABI impl__DDX_OCFloat__YAXPEAVCDataExchange__HJAEAN_Z(void* /*class*/* p0, int p1, long p2, double* p3) {}

// Symbol: ?DDX_OCFloatRO@@YAXPEAVCDataExchange@@HJAEAM@Z
extern "C" void MS_ABI impl__DDX_OCFloatRO__YAXPEAVCDataExchange__HJAEAM_Z(void* /*class*/* p0, int p1, long p2, float* p3) {}

// Symbol: ?DDX_OCFloatRO@@YAXPEAVCDataExchange@@HJAEAN@Z
extern "C" void MS_ABI impl__DDX_OCFloatRO__YAXPEAVCDataExchange__HJAEAN_Z(void* /*class*/* p0, int p1, long p2, double* p3) {}

// Symbol: ?DDX_OCInt@@YAXPEAVCDataExchange@@HJAEAH@Z
extern "C" void MS_ABI impl__DDX_OCInt__YAXPEAVCDataExchange__HJAEAH_Z(void* /*class*/* p0, int p1, long p2, int* p3) {}

// Symbol: ?DDX_OCInt@@YAXPEAVCDataExchange@@HJAEAJ@Z
extern "C" void MS_ABI impl__DDX_OCInt__YAXPEAVCDataExchange__HJAEAJ_Z(void* /*class*/* p0, int p1, long p2, long* p3) {}

// Symbol: ?DDX_OCIntRO@@YAXPEAVCDataExchange@@HJAEAH@Z
extern "C" void MS_ABI impl__DDX_OCIntRO__YAXPEAVCDataExchange__HJAEAH_Z(void* /*class*/* p0, int p1, long p2, int* p3) {}

// Symbol: ?DDX_OCIntRO@@YAXPEAVCDataExchange@@HJAEAJ@Z
extern "C" void MS_ABI impl__DDX_OCIntRO__YAXPEAVCDataExchange__HJAEAJ_Z(void* /*class*/* p0, int p1, long p2, long* p3) {}

// Symbol: ?DDX_OCShort@@YAXPEAVCDataExchange@@HJAEAF@Z
extern "C" void MS_ABI impl__DDX_OCShort__YAXPEAVCDataExchange__HJAEAF_Z(void* /*class*/* p0, int p1, long p2, short* p3) {}

// Symbol: ?DDX_OCShortRO@@YAXPEAVCDataExchange@@HJAEAF@Z
extern "C" void MS_ABI impl__DDX_OCShortRO__YAXPEAVCDataExchange__HJAEAF_Z(void* /*class*/* p0, int p1, long p2, short* p3) {}

// Symbol: ?DDX_OCText@@YAXPEAVCDataExchange@@HJAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__DDX_OCText__YAXPEAVCDataExchange__HJAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* /*class*/* p0, int p1, long p2, void* /*class*/* p3, void** p4, void* p5) {}

// Symbol: ?DDX_OCTextRO@@YAXPEAVCDataExchange@@HJAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__DDX_OCTextRO__YAXPEAVCDataExchange__HJAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* /*class*/* p0, int p1, long p2, void* /*class*/* p3, void** p4, void* p5) {}

// Symbol: ?DDX_Text@@YAXPEAVCDataExchange@@HAEAU_FILETIME@@@Z
extern "C" void MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAU_FILETIME___Z(void* /*class*/* p0, int p1, void* /*struct*/* p2) {}

// Symbol: ?DDX_Text@@YAXPEAVCDataExchange@@HAEAU_GUID@@@Z
extern "C" void MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAU_GUID___Z(void* /*class*/* p0, int p1, void* /*struct*/* p2) {}

// Symbol: ?DDX_Text@@YAXPEAVCDataExchange@@HAEAUtagDBDATE@@@Z
extern "C" void MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAUtagDBDATE___Z(void* /*class*/* p0, int p1, void* /*struct*/* p2) {}

// Symbol: ?DDX_Text@@YAXPEAVCDataExchange@@HAEAUtagDBTIME@@@Z
extern "C" void MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAUtagDBTIME___Z(void* /*class*/* p0, int p1, void* /*struct*/* p2) {}

// Symbol: ?DDX_Text@@YAXPEAVCDataExchange@@HAEAUtagDB_NUMERIC@@@Z
extern "C" void MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAUtagDB_NUMERIC___Z(void* /*class*/* p0, int p1, void* /*struct*/* p2) {}

// Symbol: ?DDX_Text@@YAXPEAVCDataExchange@@HAEAUtagDEC@@@Z
extern "C" void MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAUtagDEC___Z(void* /*class*/* p0, int p1, void* /*struct*/* p2) {}

// Symbol: ?DDX_Text@@YAXPEAVCDataExchange@@HAEAVCOleCurrency@@@Z
extern "C" void MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAVCOleCurrency___Z(void* /*class*/* p0, int p1, void* /*class*/* p2) {}

// Symbol: ?DDX_Text@@YAXPEAVCDataExchange@@HAEAVCOleDateTime@ATL@@@Z
extern "C" void MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAVCOleDateTime_ATL___Z(void* /*class*/* p0, int p1, void* /*class*/* p2) {}
