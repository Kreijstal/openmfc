// CColorDialog — OpenMFC implementation.
// Sources: ctrl_ownerdraw.cpp, dlgcommon.cpp, global_ctordtor_param.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CtrlOwnerdrawSupport.h"
#include "detail/DlgcommonSupport.h"

// Real parameterized-constructor export thunks for header-defined, publicly
// constructible MFC classes. Each viewed-as MS_ABI thunk placement-constructs
// the object with the real ctor body (defined in filecore/olecore/dlgcommon/
// inetcore or inline in headers) -- NOT a stub. Completes classes that were one
// export short of full coverage.
#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxdb.h"
#include "openmfc/afxinet.h"
#include <new>
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


















// Symbol: ?GetRuntimeClass@CColorDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CColorDialog__UEBAPEAUCRuntimeClass__XZ(const CColorDialog* pThis) {
    return CColorDialog::GetThisClass();
}
// Symbol: ?GetThisClass@CColorDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CColorDialog__SAPEAUCRuntimeClass__XZ() {
    return CColorDialog::GetThisClass();
}
// Symbol: ?OnColorOK@CColorDialog@@MEAAHXZ
extern "C" int MS_ABI impl__OnColorOK_CColorDialog__MEAAHXZ(CColorDialog* pThis) {
    (void)pThis;
    return FALSE;
}
CColorDialog::CColorDialog(unsigned long clrInit, unsigned long dwFlags, CWnd* pParentWnd)
    : CDialog(), m_clrInit(clrInit), m_clrResult(clrInit), m_dwFlags(dwFlags) {
    (void)pParentWnd;
    memset(_colordialog_padding, 0, sizeof(_colordialog_padding));
}
intptr_t CColorDialog::DoModal() {
    CHOOSECOLORW cc;
    memset(&cc, 0, sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = nullptr;
    cc.lpCustColors = s_acrCustClr;
    cc.rgbResult = m_clrInit;
    cc.Flags = m_dwFlags | CC_RGBINIT;

    if (ChooseColorW(&cc)) {
        m_clrResult = cc.rgbResult;
        return IDOK;
    }

    return IDCANCEL;
}
unsigned long CColorDialog::GetColor() const {
    return m_clrResult;
}
void CColorDialog::SetCurrentColor(unsigned long clr) {
    m_clrInit = clr;
}
unsigned long* CColorDialog::GetSavedCustomColors() {
    return s_acrCustClr;
}
// Symbol: ??0CColorDialog@@QEAA@KKPEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CColorDialog__QEAA_KKPEAVCWnd___Z(
        void* p, unsigned long clrInit, unsigned long dwFlags, CWnd* pParentWnd) {
    return new (p) CColorDialog(clrInit, dwFlags, pParentWnd);
}
