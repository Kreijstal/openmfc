// CBrush — OpenMFC implementation.
// Sources: gdicore.cpp, manual_small_stub_implementations.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/GdicoreSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/CRuntimeClassSupport.h"

// CBrush constructor with color
// Symbol: ??0CBrush@@QEAA@K@Z
extern "C" CBrush* MS_ABI impl___0CBrush__QEAA_K_Z(CBrush* pThis, unsigned long crColor) {
    if (!pThis) return nullptr;
    pThis->m_hObject = ::CreateSolidBrush(crColor);
    return pThis;
}
// Symbol: ??0CBrush@@QEAA@HK@Z
extern "C" CBrush* MS_ABI impl___0CBrush__QEAA_HK_Z(CBrush* pThis, int nIndex, unsigned long crColor) {
    if (!pThis) return nullptr;
    pThis = new(pThis) CBrush();
    pThis->m_hObject = ::CreateHatchBrush(nIndex, crColor);
    return pThis;
}
// CBrush constructor with bitmap
// Symbol: ??0CBrush@@QEAA@PEAVCBitmap@@@Z
extern "C" CBrush* MS_ABI impl___0CBrush__QEAA_PEAVCBitmap___Z(CBrush* pThis, const CBitmap* pBitmap) {
    if (!pThis) return nullptr;
    pThis = new(pThis) CBrush();
    pThis->m_hObject = ::CreatePatternBrush(
        pBitmap ? static_cast<HBITMAP>(pBitmap->m_hObject) : nullptr
    );
    return pThis;
}
// CBrush::CreateDIBPatternBrush
// Symbol: ?CreateDIBPatternBrush@CBrush@@QEAAHPEAXI@Z
extern "C" int MS_ABI impl__CreateDIBPatternBrush_CBrush__QEAAHPEAXI_Z(
    CBrush* pThis, void* lpPackedDIB, unsigned int nUsage) {
    if (!pThis || !lpPackedDIB) return FALSE;
    if (pThis->m_hObject) {
        ::DeleteObject(pThis->m_hObject);
    }
    pThis->m_hObject = ::CreateDIBPatternBrushPt(lpPackedDIB, nUsage);
    return pThis->m_hObject != nullptr;
}
CBrush::CBrush() : CGdiObject() {}
// Symbol: ?GetRuntimeClass@CBrush@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CBrush__UEBAPEAUCRuntimeClass__XZ(void* pThis) {
    (void)pThis;
    InitializeClasses();
    return &CBrush::classCBrush;
}
// Symbol: ?GetThisClass@CBrush@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CBrush__SAPEAUCRuntimeClass__XZ() {
    InitializeClasses();
    return &CBrush::classCBrush;
}
