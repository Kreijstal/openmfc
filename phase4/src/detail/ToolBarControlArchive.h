#pragma once

#include "openmfc/afxwin.h"
#include <climits>

namespace openmfc::detail {

// Unicode MFC prefixes its CString length with ff fe ff. The original
// OpenMFC control serializers used a four-byte UTF-16 length.
inline void ReadToolBarControlText(CArchive& archive, CString& text) {
    BYTE first;
    WORD next;
    archive >> first >> next;
    ULONGLONG length;
    if (first == 0xff && next == 0xfffe) {
        int charSize;
        length = AfxReadStringLength(archive, charSize);
    } else {
        BYTE last;
        archive >> last;
        length = UINT(first) | (UINT(next) << 8) | (UINT(last) << 24);
    }
    if (length > INT_MAX / sizeof(wchar_t))
        AfxThrowArchiveException(CArchiveException::badIndex, nullptr);
    if (length) {
        const int size = static_cast<int>(length);
        archive.EnsureRead(text.GetBuffer(size), size * sizeof(wchar_t));
        text.ReleaseBuffer(size);
    } else {
        text.Empty();
    }
}

}  // namespace openmfc::detail
