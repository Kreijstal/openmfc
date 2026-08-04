#include "CDumpContextSupport.h"

static_assert(sizeof(CDumpContext) == 16, "CDumpContext must be 16 bytes");
CDumpContext::CDumpContext(CFile* pFile) {
    m_nDepth = 0;
    m_pFile = pFile;
}
void CDumpContext::OutputString(LPCWSTR lpsz) {
    if (lpsz == nullptr) {
        return;
    }
    if (m_pFile != nullptr) {
        UINT nBytes = static_cast<UINT>(wcslen(lpsz) * sizeof(wchar_t));
        OpenMFC_File_Write(m_pFile, lpsz, nBytes);
    } else {
        OutputDebugStringW(lpsz);
    }
}
CDumpContext& CDumpContext::operator<<(LPCWSTR lpsz) {
    if (lpsz == nullptr) {
        OutputString(L"(NULL)");
    } else {
        OutputString(lpsz);
    }
    return *this;
}
CDumpContext& CDumpContext::operator<<(LPCSTR lpsz) {
    if (lpsz == nullptr) {
        OutputString(L"(NULL)");
        return *this;
    }
    // Widen ASCII/ANSI to wchar_t, flushing in stack-buffer-sized chunks so
    // strings longer than the buffer are emitted in full (no truncation).
    wchar_t buf[512];
    const size_t cap = (sizeof(buf) / sizeof(buf[0])) - 1;
    size_t n = 0;
    while (*lpsz != '\0') {
        buf[n++] = static_cast<wchar_t>(static_cast<unsigned char>(*lpsz++));
        if (n == cap) {
            buf[n] = L'\0';
            OutputString(buf);
            n = 0;
        }
    }
    if (n > 0) {
        buf[n] = L'\0';
        OutputString(buf);
    }
    return *this;
}
CDumpContext& CDumpContext::operator<<(const void* lp) {
    wchar_t buf[32];
    swprintf(buf, 32, L"$%llX", static_cast<unsigned long long>(reinterpret_cast<uintptr_t>(lp)));
    OutputString(buf);
    return *this;
}
CDumpContext& CDumpContext::operator<<(int n) {
    wchar_t buf[32];
    swprintf(buf, 32, L"%d", n);
    OutputString(buf);
    return *this;
}
CDumpContext& CDumpContext::operator<<(unsigned int n) {
    wchar_t buf[32];
    swprintf(buf, 32, L"%u", n);
    OutputString(buf);
    return *this;
}
CDumpContext& CDumpContext::operator<<(long n) {
    wchar_t buf[32];
    swprintf(buf, 32, L"%ld", n);
    OutputString(buf);
    return *this;
}
CDumpContext& CDumpContext::operator<<(unsigned long n) {
    wchar_t buf[32];
    swprintf(buf, 32, L"%lu", n);
    OutputString(buf);
    return *this;
}
CDumpContext& CDumpContext::operator<<(__int64 n) {
    wchar_t buf[32];
    swprintf(buf, 32, L"%lld", static_cast<long long>(n));
    OutputString(buf);
    return *this;
}
CDumpContext& CDumpContext::operator<<(unsigned __int64 n) {
    wchar_t buf[32];
    swprintf(buf, 32, L"%llu", static_cast<unsigned long long>(n));
    OutputString(buf);
    return *this;
}
CDumpContext& CDumpContext::operator<<(unsigned char ch) {
    wchar_t buf[16];
    swprintf(buf, 16, L"%u", static_cast<unsigned>(ch));
    OutputString(buf);
    return *this;
}
CDumpContext& CDumpContext::operator<<(unsigned short w) {
    wchar_t buf[16];
    swprintf(buf, 16, L"%u", static_cast<unsigned>(w));
    OutputString(buf);
    return *this;
}
CDumpContext& CDumpContext::operator<<(const CObject* pOb) {
    if (pOb == nullptr) {
        OutputString(L"(NULL)");
    } else {
        pOb->Dump();  // virtual call into CObject::Dump
    }
    return *this;
}
CDumpContext& CDumpContext::operator<<(const CObject& ob) {
    ob.Dump();  // virtual call into CObject::Dump
    return *this;
}
CDumpContext& CDumpContext::DumpAsHex(int n) {
    wchar_t buf[32];
    swprintf(buf, 32, L"0x%X", static_cast<unsigned>(n));
    OutputString(buf);
    return *this;
}
CDumpContext& CDumpContext::DumpAsHex(unsigned int n) {
    wchar_t buf[32];
    swprintf(buf, 32, L"0x%X", n);
    OutputString(buf);
    return *this;
}
CDumpContext& CDumpContext::DumpAsHex(long n) {
    wchar_t buf[32];
    swprintf(buf, 32, L"0x%lX", static_cast<unsigned long>(n));
    OutputString(buf);
    return *this;
}
CDumpContext& CDumpContext::DumpAsHex(unsigned long n) {
    wchar_t buf[32];
    swprintf(buf, 32, L"0x%lX", n);
    OutputString(buf);
    return *this;
}
CDumpContext& CDumpContext::DumpAsHex(__int64 n) {
    wchar_t buf[32];
    swprintf(buf, 32, L"0x%llX", static_cast<unsigned long long>(n));
    OutputString(buf);
    return *this;
}
CDumpContext& CDumpContext::DumpAsHex(unsigned __int64 n) {
    wchar_t buf[32];
    swprintf(buf, 32, L"0x%llX", n);
    OutputString(buf);
    return *this;
}
CDumpContext& CDumpContext::DumpAsHex(unsigned char ch) {
    wchar_t buf[16];
    swprintf(buf, 16, L"0x%X", static_cast<unsigned>(ch));
    OutputString(buf);
    return *this;
}
CDumpContext& CDumpContext::DumpAsHex(unsigned short w) {
    wchar_t buf[16];
    swprintf(buf, 16, L"0x%X", static_cast<unsigned>(w));
    OutputString(buf);
    return *this;
}
void CDumpContext::Flush() {
    if (m_pFile != nullptr) {
        OpenMFC_File_Flush(m_pFile);
    }
}
void CDumpContext::HexDump(LPCWSTR lpszLine, BYTE* pby, int nBytes, int nWidth) {
    if (lpszLine != nullptr) {
        OutputString(lpszLine);
    }
    if (pby == nullptr || nBytes <= 0) {
        return;
    }
    if (nWidth <= 0) {
        nWidth = 16;
    }
    int col = 0;
    for (int i = 0; i < nBytes; ++i) {
        wchar_t buf[8];
        swprintf(buf, 8, L"%02X ", static_cast<unsigned>(pby[i]));
        OutputString(buf);
        if (++col >= nWidth && i + 1 < nBytes) {
            OutputString(L"\r\n");
            col = 0;
        }
    }
}
