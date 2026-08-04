#include "StrcoreSupport.h"

namespace openmfc { namespace detail { namespace strcore {
HINSTANCE GetResourceHandle() {
    HINSTANCE hInst = AfxGetResourceHandle();
    if (hInst == nullptr) {
        hInst = AfxGetInstanceHandle();
    }
    return hInst;
}
int CStringLoadStringImpl(CString* pThis, HINSTANCE hInst, UINT nID) {
    if (pThis == nullptr) {
        return 0;
    }
    if (hInst == nullptr) {
        hInst = GetResourceHandle();
    }

    const int nFixedBuffer = 256;
    wchar_t szFixedBuffer[nFixedBuffer];

    int nLen = ::LoadStringW(hInst, nID, szFixedBuffer, nFixedBuffer);
    if (nLen == 0) {
        pThis->Empty();
        return 0;
    }

    if (nLen < nFixedBuffer - 1) {
        *pThis = szFixedBuffer;
        return nLen;
    }

    int nSize = nFixedBuffer * 2;
    while (nSize < 32768) {
        wchar_t* pBuf = pThis->GetBuffer(nSize);
        nLen = ::LoadStringW(hInst, nID, pBuf, nSize);
        if (nLen < nSize - 1) {
            pThis->ReleaseBuffer(nLen);
            return nLen;
        }
        pThis->ReleaseBuffer(0);
        nSize *= 2;
    }

    pThis->Empty();
    return 0;
}
void AfxFormatStringsCore(CString* rString, const wchar_t* lpszFormat, const wchar_t* const* rglpsz, int nString)
{
    if (rString == nullptr) {
        return;
    }
    if (lpszFormat == nullptr) {
        rString->Empty();
        return;
    }

    CString strResult;
    const wchar_t* p = lpszFormat;
    while (*p != L'\0')
    {
        if (*p == L'%' && p[1] >= L'1' && p[1] <= L'9')
        {
            int nIndex = static_cast<int>(p[1] - L'1');
            if (rglpsz != nullptr && nIndex >= 0 && nIndex < nString && rglpsz[nIndex] != nullptr) {
                strResult += rglpsz[nIndex];
            }
            p += 2;
            continue;
        }
        wchar_t ch[2] = { *p, L'\0' };
        strResult += ch;
        ++p;
    }

    *rString = strResult;
}
wchar_t NormalizePathChar(wchar_t ch)
{
    if (ch == L'/') return L'\\';
    return static_cast<wchar_t>(std::towlower(static_cast<wint_t>(ch)));
}
} } }  // namespace openmfc::detail::strcore
