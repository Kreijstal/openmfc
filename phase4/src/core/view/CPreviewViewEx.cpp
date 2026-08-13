// CPreviewViewEx — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp, viewrich.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/ViewrichSupport.h"

// Symbol: ?m_bScaleLargeImages@CPreviewViewEx@@1HA
extern "C" int MS_ABI impl__m_bScaleLargeImages_CPreviewViewEx__1HA = 1;

// Reuse CPreviewView helpers for behavior compatibility where layouts align.
extern "C" int MS_ABI impl__OnCreate_CPreviewView__IEAAHPEAUtagCREATESTRUCTW___Z(
    void* pThis, CREATESTRUCTW* lpCreateStruct);
extern "C" CObject* MS_ABI impl__CreateObject_CPreviewView__SAPEAVCObject__XZ();
extern "C" void MS_ABI impl__OnDisplayPageNumber_CPreviewView__MEAAXII_Z(
    void* pThis, unsigned int nCurrent, unsigned int nLast);
extern "C" int MS_ABI impl__OnEraseBkgnd_CPreviewView__IEAAHPEAVCDC___Z(
    void* pThis, CDC* pDC);
extern "C" void MS_ABI impl__OnSize_CPreviewView__IEAAXIHH_Z(
    void* pThis, unsigned int nType, int cx, int cy);

// Symbol: ??0CPreviewViewEx@@IEAA@XZ
extern "C" void* MS_ABI impl___0CPreviewViewEx__IEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ?OnUpdatePreviewNumPage@CPreviewViewEx@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdatePreviewNumPage_CPreviewViewEx__IEAAXPEAVCCmdUI___Z(void*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ??1CPreviewViewEx@@MEAA@XZ
extern "C" void MS_ABI impl___1CPreviewViewEx__MEAA_XZ(void* pThis) {
    (void)pThis;
}
// Symbol: ?CreateObject@CPreviewViewEx@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CPreviewViewEx__SAPEAVCObject__XZ()
{
    void* p = ::operator new(0x218, std::nothrow);
    if (p) {
        impl___0CPreviewViewEx__IEAA_XZ(p);
    }
    return static_cast<CObject*>(p);
}
// Symbol: ?OnCreate@CPreviewViewEx@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CPreviewViewEx__IEAAHPEAUtagCREATESTRUCTW___Z(
    void* pThis, CREATESTRUCTW* lpCreateStruct)
{
    return impl__OnCreate_CPreviewView__IEAAHPEAUtagCREATESTRUCTW___Z(pThis, lpCreateStruct);
}
// Symbol: ?OnDisplayPageNumber@CPreviewViewEx@@MEAAXII@Z
extern "C" void MS_ABI impl__OnDisplayPageNumber_CPreviewViewEx__MEAAXII_Z(
    void* pThis, unsigned int nCurrent, unsigned int nLast)
{
    impl__OnDisplayPageNumber_CPreviewView__MEAAXII_Z(pThis, nCurrent, nLast);
}
// Symbol: ?OnEraseBkgnd@CPreviewViewEx@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CPreviewViewEx__IEAAHPEAVCDC___Z(
    void* pThis, CDC* pDC)
{
    return impl__OnEraseBkgnd_CPreviewView__IEAAHPEAVCDC___Z(pThis, pDC);
}
// Symbol: ?OnSize@CPreviewViewEx@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CPreviewViewEx__IEAAXIHH_Z(
    void* pThis, unsigned int nType, int cx, int cy)
{
    impl__OnSize_CPreviewView__IEAAXIHH_Z(pThis, nType, cx, cy);
}
// Symbol: ?SetToolbarSize@CPreviewViewEx@@IEAAXXZ
extern "C" void MS_ABI impl__SetToolbarSize_CPreviewViewEx__IEAAXXZ(void* /*pThis*/) {
    // Not modeled.
}
