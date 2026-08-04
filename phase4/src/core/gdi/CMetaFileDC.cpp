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
