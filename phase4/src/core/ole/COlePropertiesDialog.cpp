// COlePropertiesDialog — OpenMFC implementation.
// Sources: ole_olectors_exports.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

























// Symbol: ??0COlePropertiesDialog@@QEAA@PEAVCOleClientItem@@IIPEAVCWnd@@@Z
// COlePropertiesDialog::COlePropertiesDialog(COleClientItem* pItem, UINT nScaleMin, UINT nScaleMax, CWnd* pParentWnd)
extern "C" void* MS_ABI impl___0COlePropertiesDialog__QEAA_PEAVCOleClientItem__IIPEAVCWnd___Z(
    COleClientItem* pItem, unsigned int nScaleMin, unsigned int nScaleMax, CWnd* pParentWnd) {
    return new COlePropertiesDialog(pItem, nScaleMin, nScaleMax, pParentWnd);
}
// Symbol: ?OnApplyScale@COlePropertiesDialog@@UEAAHPEAVCOleClientItem@@HH@Z
// COlePropertiesDialog::OnApplyScale
extern "C" int MS_ABI impl__OnApplyScale_COlePropertiesDialog__UEAAHPEAVCOleClientItem__HH_Z(
    COlePropertiesDialog* pThis, COleClientItem* pItem, unsigned int nScaleMin, unsigned int nScaleMax) {
    if (pThis) {
        if (pItem) {
            pThis->m_pItem = pItem;
        }
        return (nScaleMin <= nScaleMax) ? TRUE : FALSE;
    }
    return FALSE;
}
// Symbol: ?OnInitDialog@COlePropertiesDialog@@UEAAHXZ
// COlePropertiesDialog::OnInitDialog
extern "C" int MS_ABI impl__OnInitDialog_COlePropertiesDialog__UEAAHXZ(
    COlePropertiesDialog* pThis) {
    return pThis ? static_cast<CDialog*>(static_cast<COleDialog*>(pThis))->OnInitDialog() : FALSE;
}
COlePropertiesDialog::COlePropertiesDialog(COleClientItem* pItem, UINT nScaleMin, UINT nScaleMax, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd), m_pItem(pItem) {
    (void)nScaleMin; (void)nScaleMax;
    memset(_olepropertiesdialog_padding, 0, sizeof(_olepropertiesdialog_padding));
}
COlePropertiesDialog::~COlePropertiesDialog() {
}
intptr_t COlePropertiesDialog::DoModal() {
    return IDOK;  // OLEUIOBJECTPROPSW is not fully defined in MinGW
}
