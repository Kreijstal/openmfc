// CMetaFileDC — OpenMFC implementation.
// Sources: gdicore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/GdicoreSupport.h"

CMetaFileDC::CMetaFileDC() : CDC() {
}
CMetaFileDC::~CMetaFileDC() {
    // If DC is still open, close it
    if (m_hDC) {
        // Check if it's an enhanced metafile DC
        HENHMETAFILE hMF = CloseEnhMetaFile(m_hDC);
        if (hMF) {
            DeleteEnhMetaFile(hMF);
        }
        m_hDC = nullptr;
        m_hAttribDC = nullptr;
    }
}
int CMetaFileDC::Create(const wchar_t* lpszFilename) {
    // Windows metafiles (WMF) are legacy format
    // CreateMetaFileW returns a DC for recording metafile commands
    m_hDC = ::CreateMetaFileW(lpszFilename);
    m_hAttribDC = m_hDC;
    return m_hDC != nullptr;
}
int CMetaFileDC::CreateEnhanced(void* pDC, const wchar_t* lpszFilename,
                                 const RECT* lpBounds, const wchar_t* lpszDescription)
{
    HDC hRefDC = pDC ? static_cast<CDC*>(pDC)->m_hDC : nullptr;
    m_hDC = ::CreateEnhMetaFileW(hRefDC, lpszFilename, lpBounds, lpszDescription);
    m_hAttribDC = m_hDC;
    return m_hDC != nullptr;
}
void* CMetaFileDC::Close() {
    if (!m_hDC) return nullptr;

    // CloseMetaFile returns an HMETAFILE
    HMETAFILE hMF = ::CloseMetaFile(m_hDC);
    m_hDC = nullptr;
    m_hAttribDC = nullptr;
    return hMF;
}
void* CMetaFileDC::CloseEnhanced() {
    if (!m_hDC) return nullptr;

    // CloseEnhMetaFile returns an HENHMETAFILE
    HENHMETAFILE hMF = ::CloseEnhMetaFile(m_hDC);
    m_hDC = nullptr;
    m_hAttribDC = nullptr;
    return hMF;
}
// Symbol: ?AdjustCP@CMetaFileDC@@IEAAXH@Z
extern "C" void MS_ABI impl__AdjustCP_CMetaFileDC__IEAAXH_Z(int p0) {}

// Symbol: ?DrawTextExW@CMetaFileDC@@UEAAHPEA_WHPEAUtagRECT@@IPEAUtagDRAWTEXTPARAMS@@@Z
extern "C" int MS_ABI impl__DrawTextExW_CMetaFileDC__UEAAHPEA_WHPEAUtagRECT__IPEAUtagDRAWTEXTPARAMS___Z(wchar_t* p0, int p1, void* /*struct*/* p2, unsigned int p3, void* /*struct*/* p4) {
    return 0;
}

// Symbol: ?DrawTextW@CMetaFileDC@@UEAAHPEB_WHPEAUtagRECT@@I@Z
extern "C" int MS_ABI impl__DrawTextW_CMetaFileDC__UEAAHPEB_WHPEAUtagRECT__I_Z(const wchar_t* p0, int p1, void* /*struct*/* p2, unsigned int p3) {
    return 0;
}

// Symbol: ?Escape@CMetaFileDC@@UEAAHHHPEBDPEAX@Z
extern "C" int MS_ABI impl__Escape_CMetaFileDC__UEAAHHHPEBDPEAX_Z(int p0, int p1, const char* p2, void* p3) {
    return 0;
}

// Symbol: ?ExtTextOutW@CMetaFileDC@@UEAAHHHIPEBUtagRECT@@PEB_WIPEAH@Z
extern "C" int MS_ABI impl__ExtTextOutW_CMetaFileDC__UEAAHHHIPEBUtagRECT__PEB_WIPEAH_Z(int p0, int p1, unsigned int p2, const void* /*struct*/* p3, const wchar_t* p4, unsigned int p5, int* p6) {
    return 0;
}

// Symbol: ?GetClipBox@CMetaFileDC@@UEBAHPEAUtagRECT@@@Z
extern "C" int MS_ABI impl__GetClipBox_CMetaFileDC__UEBAHPEAUtagRECT___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?OffsetViewportOrg@CMetaFileDC@@UEAA?AVCPoint@@HH@Z
extern "C" void* MS_ABI impl__OffsetViewportOrg_CMetaFileDC__UEAA_AVCPoint__HH_Z(void* /*class*/* p0, int p1, int p2) {
    return nullptr;
}

// Symbol: ?PtVisible@CMetaFileDC@@UEBAHHH@Z
extern "C" int MS_ABI impl__PtVisible_CMetaFileDC__UEBAHHH_Z(int p0, int p1) {
    return 0;
}

// Symbol: ?RectVisible@CMetaFileDC@@UEBAHPEBUtagRECT@@@Z
extern "C" int MS_ABI impl__RectVisible_CMetaFileDC__UEBAHPEBUtagRECT___Z(const void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?ReleaseOutputDC@CMetaFileDC@@MEAAXXZ
extern "C" void MS_ABI impl__ReleaseOutputDC_CMetaFileDC__MEAAXXZ() {}

// Symbol: ?ScaleViewportExt@CMetaFileDC@@UEAA?AVCSize@@HHHH@Z
extern "C" void* MS_ABI impl__ScaleViewportExt_CMetaFileDC__UEAA_AVCSize__HHHH_Z(void* /*class*/* p0, int p1, int p2, int p3, int p4) {
    return nullptr;
}

// Symbol: ?SetAttribDC@CMetaFileDC@@UEAAXPEAUHDC__@@@Z
extern "C" void MS_ABI impl__SetAttribDC_CMetaFileDC__UEAAXPEAUHDC_____Z(void* /*struct*/* p0) {}

// Symbol: ?SetOutputDC@CMetaFileDC@@MEAAXPEAUHDC__@@@Z
extern "C" void MS_ABI impl__SetOutputDC_CMetaFileDC__MEAAXPEAUHDC_____Z(void* /*struct*/* p0) {}

// Symbol: ?SetViewportExt@CMetaFileDC@@UEAA?AVCSize@@HH@Z
extern "C" void* MS_ABI impl__SetViewportExt_CMetaFileDC__UEAA_AVCSize__HH_Z(void* /*class*/* p0, int p1, int p2) {
    return nullptr;
}

// Symbol: ?SetViewportOrg@CMetaFileDC@@UEAA?AVCPoint@@HH@Z
extern "C" void* MS_ABI impl__SetViewportOrg_CMetaFileDC__UEAA_AVCPoint__HH_Z(void* /*class*/* p0, int p1, int p2) {
    return nullptr;
}

// Symbol: ?TabbedTextOutW@CMetaFileDC@@UEAA?AVCSize@@HHPEB_WHHPEAHH@Z
extern "C" void* MS_ABI impl__TabbedTextOutW_CMetaFileDC__UEAA_AVCSize__HHPEB_WHHPEAHH_Z(void* /*class*/* p0, int p1, int p2, const wchar_t* p3, int p4, int p5, int* p6, int p7) {
    return nullptr;
}

// Symbol: ?TextOutW@CMetaFileDC@@UEAAHHHPEB_WH@Z
extern "C" int MS_ABI impl__TextOutW_CMetaFileDC__UEAAHHHPEB_WH_Z(int p0, int p1, const wchar_t* p2, int p3) {
    return 0;
}
