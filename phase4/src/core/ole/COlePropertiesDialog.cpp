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

// Symbol: ?ConvertObject@XOleUIObjInfo@COlePropertiesDialog@@UEAAJKAEBU_GUID@@@Z
extern "C" long MS_ABI impl__ConvertObject_XOleUIObjInfo_COlePropertiesDialog__UEAAJKAEBU_GUID___Z(unsigned long p0, const void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?GetConvertInfo@XOleUIObjInfo@COlePropertiesDialog@@UEAAJKPEAU_GUID@@PEAG0PEAPEAU3@PEAI@Z
extern "C" long MS_ABI impl__GetConvertInfo_XOleUIObjInfo_COlePropertiesDialog__UEAAJKPEAU_GUID__PEAG0PEAPEAU3_PEAI_Z(unsigned long p0, void* /*struct*/* p1, unsigned short* p2, unsigned long p3, void* /*struct*/** p4, unsigned int* p5) {
    return 0;
}

// Symbol: ?GetObjectInfo@XOleUIObjInfo@COlePropertiesDialog@@UEAAJKPEAKPEAPEA_W111@Z
extern "C" long MS_ABI impl__GetObjectInfo_XOleUIObjInfo_COlePropertiesDialog__UEAAJKPEAKPEAPEA_W111_Z(unsigned long p0, unsigned long* p1, wchar_t** p2, unsigned long* p3, unsigned long* p4, unsigned long* p5) {
    return 0;
}

// Symbol: ?GetViewInfo@XOleUIObjInfo@COlePropertiesDialog@@UEAAJKPEAPEAXPEAKPEAH@Z
extern "C" long MS_ABI impl__GetViewInfo_XOleUIObjInfo_COlePropertiesDialog__UEAAJKPEAPEAXPEAKPEAH_Z(unsigned long p0, void** p1, unsigned long* p2, int* p3) {
    return 0;
}

// Symbol: ?SetViewInfo@XOleUIObjInfo@COlePropertiesDialog@@UEAAJKPEAXKHH@Z
extern "C" long MS_ABI impl__SetViewInfo_XOleUIObjInfo_COlePropertiesDialog__UEAAJKPEAXKHH_Z(unsigned long p0, void* p1, unsigned long p2, int p3, int p4) {
    return 0;
}
