// CFont — OpenMFC implementation.
// Sources: gdicore.cpp, manual_small_stub_implementations.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/GdicoreSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// CFont::CreatePointFont
// Symbol: ?CreatePointFont@CFont@@QEAAHHPEB_WPEAVCDC@@@Z
extern "C" int MS_ABI impl__CreatePointFont_CFont__QEAAHHPEB_WPEAVCDC___Z(
    CFont* pThis, int nPointSize, const wchar_t* lpszFaceName, CDC* pDC) {
    if (!pThis) return FALSE;

    LOGFONTW lf = {};
    lf.lfHeight = -MulDiv(nPointSize,
        pDC ? ::GetDeviceCaps(pDC->m_hDC, LOGPIXELSY) : 96,
        720);
    lf.lfWeight = FW_NORMAL;
    if (lpszFaceName) {
        wcsncpy(lf.lfFaceName, lpszFaceName, LF_FACESIZE - 1);
        // Ensure null termination - wcsncpy doesn't guarantee it
        lf.lfFaceName[LF_FACESIZE - 1] = L'\0';
    }

    if (pThis->m_hObject) {
        ::DeleteObject(pThis->m_hObject);
    }
    pThis->m_hObject = ::CreateFontIndirectW(&lf);
    return pThis->m_hObject != nullptr;
}
// CFont::CreatePointFontIndirect
// Symbol: ?CreatePointFontIndirect@CFont@@QEAAHPEBUtagLOGFONTW@@PEAVCDC@@@Z
extern "C" int MS_ABI impl__CreatePointFontIndirect_CFont__QEAAHPEBUtagLOGFONTW__PEAVCDC___Z(
    CFont* pThis, const LOGFONTW* lpLogFont, CDC* pDC) {
    if (!pThis || !lpLogFont) return FALSE;

    LOGFONTW lf = *lpLogFont;
    int dpiY = (pDC && pDC->m_hDC) ? ::GetDeviceCaps(pDC->m_hDC, LOGPIXELSY) : 96;
    lf.lfHeight = -MulDiv(lf.lfHeight, dpiY, 720);

    if (pThis->m_hObject) {
        ::DeleteObject(pThis->m_hObject);
    }
    pThis->m_hObject = ::CreateFontIndirectW(&lf);
    return pThis->m_hObject != nullptr;
}
// Symbol: ?GetRuntimeClass@CFont@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CFont__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return &CFont::classCFont;
}
// Symbol: ?GetThisClass@CFont@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CFont__SAPEAUCRuntimeClass__XZ() {
    return &CFont::classCFont;
}
