// CPalette — OpenMFC implementation.
// Sources: gdicore.cpp, manual_small_stub_implementations.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/GdicoreSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

CPalette::CPalette() : CGdiObject() {
}
CPalette::~CPalette() {
    if (m_hObject) {
        ::DeleteObject(m_hObject);
        m_hObject = nullptr;
    }
}
int CPalette::CreatePalette(const LOGPALETTE* lpLogPalette) {
    if (!lpLogPalette) return FALSE;
    if (m_hObject) {
        ::DeleteObject(m_hObject);
    }
    m_hObject = ::CreatePalette(lpLogPalette);
    return m_hObject != nullptr;
}
int CPalette::CreateHalftonePalette(CDC* pDC) {
    if (!pDC || !pDC->m_hDC) return FALSE;
    if (m_hObject) {
        ::DeleteObject(m_hObject);
    }
    m_hObject = ::CreateHalftonePalette(pDC->m_hDC);
    return m_hObject != nullptr;
}
int CPalette::GetEntryCount() const {
    if (!m_hObject) return 0;
    WORD nEntries = 0;
    ::GetObject(m_hObject, sizeof(WORD), &nEntries);
    return nEntries;
}
unsigned int CPalette::GetPaletteEntries(unsigned int nStartIndex, unsigned int nNumEntries,
                                          PALETTEENTRY* lpPaletteColors) const {
    if (!m_hObject) return 0;
    return ::GetPaletteEntries((HPALETTE)m_hObject, nStartIndex, nNumEntries, lpPaletteColors);
}
unsigned int CPalette::SetPaletteEntries(unsigned int nStartIndex, unsigned int nNumEntries,
                                          PALETTEENTRY* lpPaletteColors) {
    if (!m_hObject) return 0;
    return ::SetPaletteEntries((HPALETTE)m_hObject, nStartIndex, nNumEntries, lpPaletteColors);
}
void CPalette::AnimatePalette(unsigned int nStartIndex, unsigned int nNumEntries,
                              PALETTEENTRY* lpPaletteColors) {
    if (m_hObject) {
        ::AnimatePalette((HPALETTE)m_hObject, nStartIndex, nNumEntries, lpPaletteColors);
    }
}
int CPalette::ResizePalette(unsigned int nNumEntries) {
    if (!m_hObject) return FALSE;
    return ::ResizePalette((HPALETTE)m_hObject, nNumEntries);
}
unsigned int CPalette::GetNearestPaletteIndex(unsigned long crColor) const {
    if (!m_hObject) return CLR_INVALID;
    return ::GetNearestPaletteIndex((HPALETTE)m_hObject, crColor);
}
// Symbol: ?GetRuntimeClass@CPalette@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CPalette__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return &CPalette::classCPalette;
}
// Symbol: ?GetThisClass@CPalette@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CPalette__SAPEAUCRuntimeClass__XZ() {
    return &CPalette::classCPalette;
}
