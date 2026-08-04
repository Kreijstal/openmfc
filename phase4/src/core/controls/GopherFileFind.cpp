// GopherFileFind — OpenMFC implementation.
// Sources: inetcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/InetcoreSupport.h"

GopherFileFind::GopherFileFind(CFtpConnection* pFtp, DWORD_PTR dwContext)
    : CFtpFileFind(pFtp, dwContext) {}
GopherFileFind::~GopherFileFind() {}
CString GopherFileFind::GetFileName() const { return m_strFileName; }
CString GopherFileFind::GetScreenName() const { return m_strFileName; }
CGopherLocator GopherFileFind::GetLocator() const {
    CString locator = m_strRoot;
    if (locator.IsEmpty()) locator = m_strFileName;
    CGopherLocator loc;
    FillLocator_Inetcore(&loc, locator);
    return loc;
}
int GopherFileFind::GetFileLength(DWORD_PTR& dwLength) const {
    dwLength = (static_cast<DWORD_PTR>(m_findFileData.nFileSizeHigh) << 32) | m_findFileData.nFileSizeLow;
    return 1;
}
