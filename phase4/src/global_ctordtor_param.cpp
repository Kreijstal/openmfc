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

// Symbol: ??0CException@@QEAA@H@Z
extern "C" void* MS_ABI impl___0CException__QEAA_H_Z(void* p, int bAutoDelete) {
    return new (p) CException(bAutoDelete);
}

// Symbol: ??0CArchive@@QEAA@PEAVCFile@@IHPEAX@Z
extern "C" void* MS_ABI impl___0CArchive__QEAA_PEAVCFile__IHPEAX_Z(
        void* p, CFile* pFile, unsigned int nMode, int nBufSize, void* lpBuf) {
    return new (p) CArchive(pFile, nMode, nBufSize, lpBuf);
}

// Symbol: ??0CDBException@@QEAA@F@Z
extern "C" void* MS_ABI impl___0CDBException__QEAA_F_Z(void* p, short nRetCode) {
    return new (p) CDBException(nRetCode);
}

// Symbol: ??0CColorDialog@@QEAA@KKPEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CColorDialog__QEAA_KKPEAVCWnd___Z(
        void* p, unsigned long clrInit, unsigned long dwFlags, CWnd* pParentWnd) {
    return new (p) CColorDialog(clrInit, dwFlags, pParentWnd);
}

// Symbol: ??0CFileDialog@@QEAA@HPEB_W0K0PEAVCWnd@@KH@Z
extern "C" void* MS_ABI impl___0CFileDialog__QEAA_HPEB_W0K0PEAVCWnd__KH_Z(
        void* p, int bOpenFileDialog, const wchar_t* lpszDefExt,
        const wchar_t* lpszFileName, unsigned long dwFlags,
        const wchar_t* lpszFilter, CWnd* pParentWnd, unsigned long dwSize,
        int bVistaStyle) {
    return new (p) CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags,
                               lpszFilter, pParentWnd, dwSize, bVistaStyle);
}

// Symbol: ??0CPageSetupDialog@@QEAA@KPEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CPageSetupDialog__QEAA_KPEAVCWnd___Z(
        void* p, unsigned long dwFlags, CWnd* pParentWnd) {
    return new (p) CPageSetupDialog(dwFlags, pParentWnd);
}

// Symbol: ??0CFtpFileFind@@QEAA@PEAVCFtpConnection@@_K@Z
extern "C" void* MS_ABI impl___0CFtpFileFind__QEAA_PEAVCFtpConnection___K_Z(
        void* p, CFtpConnection* pConn, unsigned long long dwContext) {
    return new (p) CFtpFileFind(pConn, dwContext);
}

// Symbol: ??0COleDialog@@QEAA@PEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0COleDialog__QEAA_PEAVCWnd___Z(void* p, CWnd* pParentWnd) {
    return new (p) COleDialog(pParentWnd);
}

// Symbol: ??0COleBusyDialog@@QEAA@PEAUHTASK__@@HKPEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0COleBusyDialog__QEAA_PEAUHTASK____HKPEAVCWnd___Z(
        void* p, HTASK hTaskBusy, int bNotResponding, unsigned long dwFlags,
        CWnd* pParentWnd) {
    return new (p) COleBusyDialog(hTaskBusy, bNotResponding, dwFlags, pParentWnd);
}

// Symbol: ??0COleChangeIconDialog@@QEAA@PEAVCOleClientItem@@KPEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0COleChangeIconDialog__QEAA_PEAVCOleClientItem__KPEAVCWnd___Z(
        void* p, COleClientItem* pItem, unsigned long dwFlags, CWnd* pParentWnd) {
    return new (p) COleChangeIconDialog(pItem, dwFlags, pParentWnd);
}

// Symbol: ??0COleChangeSourceDialog@@QEAA@PEAVCOleClientItem@@PEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0COleChangeSourceDialog__QEAA_PEAVCOleClientItem__PEAVCWnd___Z(
        void* p, COleClientItem* pItem, CWnd* pParentWnd) {
    return new (p) COleChangeSourceDialog(pItem, 0, pParentWnd);
}

// Symbol: ??0COleConvertDialog@@QEAA@PEAVCOleClientItem@@KPEAU_GUID@@PEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0COleConvertDialog__QEAA_PEAVCOleClientItem__KPEAU_GUID__PEAVCWnd___Z(
        void* p, COleClientItem* pItem, unsigned long dwFlags, GUID* pClassID,
        CWnd* pParentWnd) {
    return new (p) COleConvertDialog(pItem, dwFlags, pClassID, pParentWnd);
}

// Symbol: ??0COleInsertDialog@@QEAA@KPEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0COleInsertDialog__QEAA_KPEAVCWnd___Z(
        void* p, unsigned long dwFlags, CWnd* pParentWnd) {
    return new (p) COleInsertDialog(dwFlags, pParentWnd);
}

// Symbol: ??0COleLinksDialog@@QEAA@PEAVCOleDocument@@PEAVCView@@KPEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0COleLinksDialog__QEAA_PEAVCOleDocument__PEAVCView__KPEAVCWnd___Z(
        void* p, COleDocument* pDoc, CView* pView, unsigned long dwFlags,
        CWnd* pParentWnd) {
    return new (p) COleLinksDialog(pDoc, pView, dwFlags, pParentWnd);
}

// Symbol: ??0COlePasteSpecialDialog@@QEAA@KPEAVCOleDataObject@@PEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0COlePasteSpecialDialog__QEAA_KPEAVCOleDataObject__PEAVCWnd___Z(
        void* p, unsigned long dwFlags, COleDataObject* pDataObject, CWnd* pParentWnd) {
    return new (p) COlePasteSpecialDialog(dwFlags, pDataObject, pParentWnd);
}

// Symbol: ??0COleUpdateDialog@@QEAA@PEAVCOleDocument@@HHPEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0COleUpdateDialog__QEAA_PEAVCOleDocument__HHPEAVCWnd___Z(
        void* p, COleDocument* pDoc, int bUpdateLinks, int bUpdateEmbeddings,
        CWnd* pParentWnd) {
    return new (p) COleUpdateDialog(pDoc, bUpdateLinks, bUpdateEmbeddings, pParentWnd);
}

// Symbol: ??0COleCmdUI@@QEAA@PEAU_tagOLECMD@@KPEBU_GUID@@@Z
extern "C" void* MS_ABI impl___0COleCmdUI__QEAA_PEAU_tagOLECMD__KPEBU_GUID___Z(
        void* p, OLECMD* rgCmds, unsigned long cCmds, const GUID* pGuid) {
    return new (p) COleCmdUI(rgCmds, cCmds, pGuid);
}
