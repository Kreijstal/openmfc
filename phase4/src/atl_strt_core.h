// OpenMFC: ATL-binary-compatible string core shared by the exported CStringT<char>
// (CStringA) and CStringT<wchar_t> (CStringW) template instantiations.
//
// A drop-in MSVC client's CStringT object is exactly { CH* m_pszData } (8 bytes), with a
// CStringData header stored immediately before *m_pszData. This header/manager layout is
// byte-identical to ATL (see openmfc/afxstr.h, which defines CStringData/IAtlStringMgr and
// the process string manager). AtlStrT<CH> wraps that single-pointer object and provides
// the operations the exported methods forward to. It is NOT a public type: only the
// generated thunk file includes this header, so it never affects any client ABI.
#pragma once
#include "openmfc/afxstr.h"
#include <new>
#include <cstdarg>
#include <cstdlib>

#ifdef _WIN32
  #include <windows.h>
  #include <oleauto.h>
#else
// Native (Linux) build used ONLY by tests/test_atl_string_core.cpp to exercise the pure
// string algorithms on-host (this box's wine is broken for UCRT exes -- see the test).
// Minimal ASCII-fidelity shims for the Win32 surface the core touches; the real Windows
// build above uses the genuine APIs.  Win32-only features (VARIANT, FormatMessage,
// resource load) degrade to no-ops here and are not exercised by the native test.
#include <cstring>
#include <cwchar>
#include <strings.h>
typedef unsigned long DWORD;
typedef wchar_t OLECHAR;
typedef OLECHAR* BSTR;
typedef void* HINSTANCE;
typedef const char* LPCSTR; typedef char* LPSTR;
typedef const wchar_t* LPCWSTR; typedef wchar_t* LPWSTR;
struct tagVARIANT { unsigned short vt; BSTR bstrVal; };
typedef tagVARIANT VARIANT;
#define CP_THREAD_ACP 3u
#define NORM_IGNORECASE 1u
#define LOCALE_USER_DEFAULT 0u
#define FORMAT_MESSAGE_FROM_STRING 0u
#define FORMAT_MESSAGE_ALLOCATE_BUFFER 0u
#define VT_BSTR 8
#define S_OK 0L
static inline void RaiseException(unsigned long, unsigned long, unsigned long, const void*) { std::abort(); }
static inline DWORD GetThreadLocale() { return 0; }
static inline int CompareStringA(DWORD, DWORD f, LPCSTR a, int, LPCSTR b, int) {
    int r = (f & NORM_IGNORECASE) ? strcasecmp(a, b) : std::strcmp(a, b);
    return r < 0 ? 1 : (r == 0 ? 2 : 3);
}
static inline int CompareStringW(DWORD, DWORD f, LPCWSTR a, int, LPCWSTR b, int) {
    (void)f; int r = std::wcscmp(a, b); return r < 0 ? 1 : (r == 0 ? 2 : 3);
}
static inline int WideCharToMultiByte(DWORD, DWORD, const wchar_t* s, int sl, char* d, int cap, void*, void*) {
    int n = (sl < 0) ? (int)std::wcslen(s) + 1 : sl;
    if (cap == 0) return n;
    for (int i = 0; i < n && i < cap; ++i) d[i] = (char)s[i];
    return n;
}
static inline int MultiByteToWideChar(DWORD, DWORD, const char* s, int sl, wchar_t* d, int cap) {
    int n = (sl < 0) ? (int)std::strlen(s) + 1 : sl;
    if (cap == 0) return n;
    for (int i = 0; i < n && i < cap; ++i) d[i] = (wchar_t)(unsigned char)s[i];
    return n;
}
static inline BSTR SysAllocStringLen(const OLECHAR*, unsigned) { return nullptr; }
static inline int SysReAllocStringLen(BSTR*, const OLECHAR*, unsigned) { return 0; }
static inline unsigned SysStringLen(BSTR) { return 0; }
static inline void VariantInit(VARIANT*) {}
static inline long VariantClear(VARIANT*) { return 0; }
static inline long VariantChangeTypeEx(VARIANT*, VARIANT*, DWORD, unsigned short, unsigned short) { return -1; }
static inline DWORD GetEnvironmentVariableA(LPCSTR n, LPSTR b, DWORD c) { const char* v = std::getenv(n); if (!v) return 0; DWORD l = (DWORD)std::strlen(v); if (b && c > l) std::strcpy(b, v); return l; }
static inline DWORD GetEnvironmentVariableW(LPCWSTR, LPWSTR, DWORD) { return 0; }
static inline int CharToOemBuffA(LPCSTR s, LPSTR d, DWORD n) { for (DWORD i = 0; i < n; ++i) d[i] = s[i]; return 1; }
static inline int OemToCharBuffA(LPCSTR s, LPSTR d, DWORD n) { for (DWORD i = 0; i < n; ++i) d[i] = s[i]; return 1; }
static inline DWORD FormatMessageA(DWORD, LPCSTR, DWORD, DWORD, LPSTR, DWORD, va_list*) { return 0; }
static inline DWORD FormatMessageW(DWORD, LPCWSTR, DWORD, DWORD, LPWSTR, DWORD, va_list*) { return 0; }
static inline void* LocalFree(void*) { return nullptr; }
static inline int LoadStringW(HINSTANCE, unsigned, wchar_t*, int) { return 0; }
#endif

namespace openmfc_str {

// Per-character-type nil (empty, locked: nRefs == -1, never freed). The wide nil reuses
// afxstr.h's, so wide strings we create interoperate with the existing wide thunks.
template <class CH> CStringData* NilData();

template <> inline CStringData* NilData<wchar_t>() { return GetNilStringData(); }

template <> inline CStringData* NilData<char>() {
    static struct { CStringData header; char nul; } nil = { { nullptr, 0, 0, -1 }, '\0' };
    if (nil.header.pStringMgr == nullptr) nil.header.pStringMgr = OpenMFC_GetStringMgr();
    return &nil.header;
}

template <class CH>
class AtlStrT {
public:
    // --- lifetime ---------------------------------------------------------------
    void Construct() { m_pszData = data(NilData<CH>()); }

    void ConstructMgr(IAtlStringMgr* mgr) {
        // Empty string bound to a specific manager. ATL keeps the manager even when empty;
        // we model that by allocating a zero-length buffer from it (so later growth uses it).
        if (!mgr) { Construct(); return; }
        CStringData* pd = mgr->Allocate(0, sizeof(CH));
        if (!pd) { Construct(); return; }
        data(pd)[0] = 0;
        pd->nDataLength = 0;
        m_pszData = data(pd);
    }

    void ConstructPsz(const CH* psz) {
        if (!psz || !*psz) { Construct(); return; }
        int n = Len(psz);
        AllocBuffer(n);
        Copy(m_pszData, psz, n + 1);
        GetData()->nDataLength = n;
    }

    void ConstructPszN(const CH* psz, int n) {
        if (n <= 0 || !psz) { Construct(); return; }
        AllocBuffer(n);
        Copy(m_pszData, psz, n);
        m_pszData[n] = 0;
        GetData()->nDataLength = n;
    }

    void ConstructChRep(CH ch, int nRep) {
        if (nRep <= 0) { Construct(); return; }
        AllocBuffer(nRep);
        for (int i = 0; i < nRep; ++i) m_pszData[i] = ch;
        m_pszData[nRep] = 0;
        GetData()->nDataLength = nRep;
    }

    void ConstructCopy(const AtlStrT& src) {
        CStringData* s = src.GetData();
        if (s->IsLocked()) { m_pszData = src.m_pszData; }
        else { s->AddRef(); m_pszData = src.m_pszData; }
    }

    void Destruct() {
        CStringData* pd = GetData();
        if (pd->Release()) FreeData(pd);
    }

    // --- accessors --------------------------------------------------------------
    int GetLength() const { return GetData()->nDataLength; }
    int GetAllocLength() const { return GetData()->nAllocLength; }
    bool IsEmpty() const { return GetData()->nDataLength == 0; }
    const CH* GetString() const { return m_pszData; }
    CH* Buffer() { return m_pszData; }
    CH GetAt(int i) const { return m_pszData[i]; }
    CStringData* GetData() const { return reinterpret_cast<CStringData*>(m_pszData) - 1; }
    IAtlStringMgr* GetManager() const {
        CStringData* pd = GetData();
        return pd->pStringMgr ? pd->pStringMgr : OpenMFC_GetStringMgr();
    }

    void Empty() {
        CStringData* pd = GetData();
        if (pd->nDataLength == 0) return;
        if (pd->Release()) FreeData(pd);
        m_pszData = data(NilData<CH>());
    }

    void SetAt(int i, CH ch) {
        if (i < 0 || i >= GetLength()) return;
        CH* p = GetBuffer(GetLength());
        p[i] = ch;
        ReleaseBuffer(GetLength());
    }

    // --- buffer management (ATL semantics) --------------------------------------
    CH* GetBuffer(int nMin) {
        CStringData* pd = GetData();
        if (nMin < 0) nMin = pd->nDataLength;
        if (pd->IsShared() || nMin > pd->nAllocLength) {
            int nOld = pd->nDataLength;
            int nNew = nMin > nOld ? nMin : nOld;
            CStringData* nd = AllocData(nNew);
            Copy(data(nd), m_pszData, nOld + 1);
            nd->nDataLength = nOld;
            if (pd->Release()) FreeData(pd);
            m_pszData = data(nd);
        }
        GetData()->nRefs = -1; // lock while exposed
        return m_pszData;
    }
    CH* GetBufferDefault() { return GetBuffer(GetData()->nDataLength); }

    CH* GetBufferSetLength(int n) {
        CH* p = GetBuffer(n);
        GetData()->nDataLength = n;
        p[n] = 0;
        return p;
    }

    void ReleaseBuffer(int nNew) {
        CStringData* pd = GetData();
        if (nNew < 0) nNew = Len(m_pszData);
        pd->nDataLength = nNew;
        m_pszData[nNew] = 0;
        pd->nRefs = 1;
    }
    void ReleaseBufferSetLength(int n) { if (n >= 0) ReleaseBuffer(n); }

    CH* LockBuffer() {
        CH* p = GetBuffer(GetData()->nDataLength);
        GetData()->nRefs = -1;
        return p;
    }
    void UnlockBuffer() { CStringData* pd = GetData(); if (pd->nRefs == -1) pd->nRefs = 1; }

    void Preallocate(int n) { if (n > GetData()->nAllocLength) { int len = GetLength(); GetBuffer(n); GetData()->nDataLength = len; } ReleaseBuffer(GetLength()); }
    void FreeExtra() {
        CStringData* pd = GetData();
        if (pd->IsShared() || pd->IsLocked()) return;
        if (pd->nAllocLength == pd->nDataLength) return;
        int n = pd->nDataLength;
        CStringData* nd = AllocData(n);
        Copy(data(nd), m_pszData, n + 1);
        nd->nDataLength = n;
        FreeData(pd);
        m_pszData = data(nd);
    }
    void Truncate(int n) { if (n < GetLength()) { GetBuffer(GetLength()); GetData()->nDataLength = n; m_pszData[n] = 0; ReleaseBuffer(n); } }

    // --- assignment / append ----------------------------------------------------
    void AssignCopy(const AtlStrT& src) {
        if (m_pszData == src.m_pszData) return;
        CStringData* pd = GetData();
        if (pd->Release()) FreeData(pd);
        CStringData* s = src.GetData();
        if (s->IsLocked()) m_pszData = src.m_pszData;
        else { s->AddRef(); m_pszData = src.m_pszData; }
    }

    void AssignPsz(const CH* psz) {
        if (!psz || !*psz) { Empty(); return; }
        int n = Len(psz);
        CStringData* pd = GetData();
        if (pd->IsLocked() || pd->IsShared() || n > pd->nAllocLength) {
            if (pd->Release()) FreeData(pd);
            AllocBuffer(n);
        }
        Copy(m_pszData, psz, n + 1);
        GetData()->nDataLength = n;
    }

    void AssignChar(CH ch) { CH b[2] = { ch, 0 }; AssignPsz(b); }

    void AppendPszN(const CH* psz, int n) {
        if (!psz || n <= 0) return;
        int nOld = GetData()->nDataLength;
        int nNew = nOld + n;
        CStringData* pd = GetData();
        if (pd->IsLocked() || pd->IsShared() || nNew > pd->nAllocLength) {
            CStringData* nd = AllocData(nNew);
            Copy(data(nd), m_pszData, nOld);
            if (pd->Release()) FreeData(pd);
            m_pszData = data(nd);
        }
        Copy(m_pszData + nOld, psz, n);
        m_pszData[nNew] = 0;
        GetData()->nDataLength = nNew;
    }
    void AppendPsz(const CH* psz) { if (psz) AppendPszN(psz, Len(psz)); }
    void AppendChar(CH ch) { AppendPszN(&ch, 1); }

    // --- edit -------------------------------------------------------------------
    int Insert(int nIndex, CH ch) { CH b[2] = { ch, 0 }; return InsertPsz(nIndex, b, 1); }
    int Insert(int nIndex, const CH* psz) { return InsertPsz(nIndex, psz, psz ? Len(psz) : 0); }

    int Delete(int nIndex, int nCount) {
        if (nIndex < 0) nIndex = 0;
        int nLen = GetLength();
        if (nIndex >= nLen || nCount <= 0) return nLen;
        if (nIndex + nCount > nLen) nCount = nLen - nIndex;
        int nNew = nLen - nCount;
        CH* p = GetBuffer(nLen);
        Move(p + nIndex, p + nIndex + nCount, nLen - nIndex - nCount + 1);
        ReleaseBuffer(nNew);
        return nNew;
    }

    int Remove(CH ch) {
        int nLen = GetLength();
        if (nLen == 0) return 0;
        CH* p = GetBuffer(nLen);
        CH* w = p; int removed = 0;
        for (CH* r = p; *r; ++r) { if (*r == ch) ++removed; else *w++ = *r; }
        *w = 0;
        ReleaseBuffer(nLen - removed);
        return removed;
    }

    void MakeUpper() { int n = GetLength(); if (n) { CH* p = GetBuffer(n); ToUpper(p); ReleaseBuffer(n); } }
    void MakeLower() { int n = GetLength(); if (n) { CH* p = GetBuffer(n); ToLower(p); ReleaseBuffer(n); } }
    void MakeReverse() {
        int n = GetLength();
        if (n > 1) { CH* p = GetBuffer(n); for (int i = 0; i < n/2; ++i) { CH t = p[i]; p[i] = p[n-1-i]; p[n-1-i] = t; } ReleaseBuffer(n); }
    }

    int ReplaceCh(CH chOld, CH chNew) {
        int n = 0, nLen = GetLength();
        if (nLen && chOld != chNew) { CH* p = GetBuffer(nLen); for (int i = 0; i < nLen; ++i) if (p[i] == chOld) { p[i] = chNew; ++n; } ReleaseBuffer(nLen); }
        return n;
    }

    // --- search -----------------------------------------------------------------
    int FindCh(CH ch, int nStart) const {
        if (nStart < 0) nStart = 0;
        int nLen = GetLength();
        if (nStart > nLen) return -1;
        for (const CH* p = m_pszData + nStart; *p; ++p) if (*p == ch) return int(p - m_pszData);
        return -1;
    }
    int FindPsz(const CH* sub, int nStart) const {
        if (!sub || !*sub) return -1;
        if (nStart < 0) nStart = 0;
        int nLen = GetLength();
        if (nStart > nLen) return -1;
        for (const CH* p = m_pszData + nStart; *p; ++p) {
            const CH* a = p; const CH* b = sub;
            while (*a && *b && *a == *b) { ++a; ++b; }
            if (!*b) return int(p - m_pszData);
        }
        return -1;
    }
    int ReverseFind(CH ch) const {
        int nLen = GetLength(); const CH* found = nullptr;
        for (const CH* p = m_pszData; *p; ++p) if (*p == ch) found = p;
        (void)nLen; return found ? int(found - m_pszData) : -1;
    }
    int FindOneOf(const CH* set) const {
        if (!set || !*set) return -1;
        for (const CH* p = m_pszData; *p; ++p) for (const CH* s = set; *s; ++s) if (*p == *s) return int(p - m_pszData);
        return -1;
    }

    // --- compare ----------------------------------------------------------------
    int Compare(const CH* psz) const { return Cmp(m_pszData, psz ? psz : Empty0()); }
    int CompareNoCase(const CH* psz) const { return CmpNoCase(m_pszData, psz ? psz : Empty0()); }

    // --- collate (locale-aware, faithful to ATLStrTraitMFC) ---------------------
    int Collate(const CH* psz) const { return CollateImpl(m_pszData, psz ? psz : Empty0(), 0); }
    int CollateNoCase(const CH* psz) const { return CollateImpl(m_pszData, psz ? psz : Empty0(), NORM_IGNORECASE); }

    // --- printf-style formatting ------------------------------------------------
    void FormatV(const CH* fmt, va_list args) { FormatVImpl(fmt, args); }
    void Format(const CH* fmt, ...) { va_list a; va_start(a, fmt); FormatVImpl(fmt, a); va_end(a); }
    void AppendFormatV(const CH* fmt, va_list args) {
        AtlStrT<CH> tmp; tmp.Construct(); tmp.FormatVImpl(fmt, args);
        AppendPszN(tmp.GetString(), tmp.GetLength()); tmp.Destruct();
    }
    void AppendFormat(const CH* fmt, ...) { va_list a; va_start(a, fmt); AppendFormatV(fmt, a); va_end(a); }

    // --- ATL-internal buffer primitives -----------------------------------------
    void Fork(int nLength) {
        CStringData* pOld = GetData();
        int nOldLen = pOld->nDataLength;
        CStringData* nd = AllocData(nLength);
        int n = nOldLen < nLength ? nOldLen : nLength;
        Copy(data(nd), m_pszData, n);
        data(nd)[n] = 0;
        nd->nDataLength = n;
        if (pOld->Release()) FreeData(pOld);
        m_pszData = data(nd);
    }
    void SetLength(int nLength) {
        CStringData* pd = GetData();
        if (nLength < 0 || nLength > pd->nAllocLength) return;
        pd->nDataLength = nLength;
        m_pszData[nLength] = 0;
    }
    CH* PrepareWrite(int nLength) {
        CStringData* pd = GetData();
        if (pd->IsShared() || pd->IsLocked() || nLength > pd->nAllocLength) {
            int want = nLength > pd->nAllocLength ? nLength : pd->nAllocLength;
            Fork(want);
        }
        return m_pszData;
    }
    void PrepareWrite2(int nLength) { (void)PrepareWrite(nLength); }
    void Reallocate(int nLength) {
        CStringData* pd = GetData();
        if (nLength <= pd->nAllocLength) return;
        Fork(nLength);
    }
    void Attach(CStringData* pData) {
        CStringData* pd = GetData();
        if (pd->Release()) FreeData(pd);
        m_pszData = data(pData);
    }
    void SetManagerEmpty(IAtlStringMgr* mgr) { Destruct(); ConstructMgr(mgr); }

    static void ConstructStatic(AtlStrT* p) { p->Construct(); }
    static CStringData* CloneData(CStringData* pData) {
        IAtlStringMgr* mgr = pData && pData->pStringMgr ? pData->pStringMgr : OpenMFC_GetStringMgr();
        if (pData && !pData->IsShared() && !pData->IsLocked()) { pData->AddRef(); return pData; }
        int n = pData ? pData->nDataLength : 0;
        CStringData* nd = mgr->Allocate(n, sizeof(CH));
        if (!nd) { RaiseException(0xE06D7363, 0, 0, nullptr); return nullptr; }
        if (pData) Copy(data(nd), data(pData), n);
        data(nd)[n] = 0; nd->nDataLength = n;
        return nd;
    }
    static void Concatenate(AtlStrT& dest, const CH* p1, int n1, const CH* p2, int n2) {
        if (n1 < 0) n1 = 0;
        if (n2 < 0) n2 = 0;
        dest.Destruct();
        dest.AllocBuffer(n1 + n2);
        Copy(dest.m_pszData, p1, n1);
        Copy(dest.m_pszData + n1, p2, n2);
        dest.m_pszData[n1 + n2] = 0;
        dest.GetData()->nDataLength = n1 + n2;
    }

    // --- cross-char conversion (ANSI<->UTF16 via the thread ANSI codepage, as ATL) --
    // OT = the "other" character type. Narrow(char) converts from wchar_t and vice versa.
    template <class OT>
    void ConstructConvert(const OT* src) {
        if (!src || !*src) { Construct(); return; }
        int n = ConvLen(src);
        if (n <= 0) { Construct(); return; }
        AllocBuffer(n);
        int w = ConvCopy(m_pszData, n, src);
        m_pszData[w] = 0;
        GetData()->nDataLength = w;
    }
    template <class OT>
    void ConstructConvertN(const OT* src, int srcLen) {
        if (!src || srcLen <= 0) { Construct(); return; }
        int n = ConvLenN(src, srcLen);
        AllocBuffer(n > 0 ? n : 0);
        int w = n > 0 ? ConvCopyN(m_pszData, n, src, srcLen) : 0;
        m_pszData[w] = 0;
        GetData()->nDataLength = w;
    }
    template <class OT>
    void AssignConvert(const OT* src) { AtlStrT<CH> t; t.ConstructConvert(src); AssignCopy(t); t.Destruct(); }
    template <class OT>
    void AppendConvert(const OT* src) { AtlStrT<CH> t; t.ConstructConvert(src); AppendPszN(t.GetString(), t.GetLength()); t.Destruct(); }

    void AppendConvertChar(CH otherAsCh) { AppendChar(otherAsCh); }

    // --- Win32-backed --------------------------------------------------------------
    BSTR AllocSysString() const {
        int len = GetLength();
        if (sizeof(CH) == 2)
            return SysAllocStringLen(reinterpret_cast<const OLECHAR*>(m_pszData), len);
        int wl = MultiByteToWideChar(CP_THREAD_ACP, 0, (const char*)m_pszData, len, nullptr, 0);
        BSTR b = SysAllocStringLen(nullptr, wl);
        if (b) MultiByteToWideChar(CP_THREAD_ACP, 0, (const char*)m_pszData, len, b, wl);
        return b;
    }
    BSTR SetSysString(BSTR* pbstr) const {
        int len = GetLength();
        if (sizeof(CH) == 2) {
            SysReAllocStringLen(pbstr, reinterpret_cast<const OLECHAR*>(m_pszData), len);
        } else {
            int wl = MultiByteToWideChar(CP_THREAD_ACP, 0, (const char*)m_pszData, len, nullptr, 0);
            SysReAllocStringLen(pbstr, nullptr, wl);
            if (*pbstr) MultiByteToWideChar(CP_THREAD_ACP, 0, (const char*)m_pszData, len, *pbstr, wl);
        }
        return *pbstr;
    }
    int LoadEnvVar(const CH* name) {
        DWORD n = (sizeof(CH) == 1) ? GetEnvironmentVariableA((LPCSTR)name, nullptr, 0)
                                    : GetEnvironmentVariableW((LPCWSTR)name, nullptr, 0);
        if (n == 0) { Empty(); return 0; }
        CH* b = GetBuffer((int)n);
        DWORD got = (sizeof(CH) == 1) ? GetEnvironmentVariableA((LPCSTR)name, (LPSTR)b, n)
                                      : GetEnvironmentVariableW((LPCWSTR)name, (LPWSTR)b, n);
        ReleaseBuffer((int)got);
        return (int)got;
    }
    void CharToOem() { int n = GetLength(); if (n && sizeof(CH) == 1) { CH* b = GetBuffer(n); CharToOemBuffA((LPCSTR)b, (LPSTR)b, n); ReleaseBuffer(n); } }
    void OemToChar() { int n = GetLength(); if (n && sizeof(CH) == 1) { CH* b = GetBuffer(n); OemToCharBuffA((LPCSTR)b, (LPSTR)b, n); ReleaseBuffer(n); } }

    // ::FormatMessage(FROM_STRING) with a caller argument array/va_list, as CStringT does.
    void FormatMessageStr(const CH* fmt, va_list* pArgs) {
        if (!fmt) { Empty(); return; }
        CH* out = nullptr; DWORD n;
        if (sizeof(CH) == 1)
            n = FormatMessageA(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER,
                               (LPCSTR)fmt, 0, 0, (LPSTR)&out, 0, pArgs);
        else
            n = FormatMessageW(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER,
                               (LPCWSTR)fmt, 0, 0, (LPWSTR)&out, 0, pArgs);
        if (n && out) { Destruct(); ConstructPszN(out, (int)n); LocalFree(out); }
        else Empty();
    }

    // Load a string resource (always Unicode) into this string, converting for CStringA.
    int LoadStr(HINSTANCE hInst, UINT nID) {
        wchar_t buf[1024];
        int wl = ::LoadStringW(hInst, nID, buf, 1024);
        if (wl <= 0) { Empty(); return 0; }
        Destruct();
        if (sizeof(CH) == 2) ConstructPszN(reinterpret_cast<const CH*>(buf), wl);
        else ConstructConvertN<wchar_t>(buf, wl);
        return GetLength();
    }

    // --- VARIANT construction ---------------------------------------------------
    void ConstructVariant(const VARIANT* pv);   // defined out-of-line per CH below

    // --- static char helpers ----------------------------------------------------
    static int StringLength(const CH* psz) { return psz ? Len(psz) : 0; }
    static void CopyChars(CH* dst, const CH* src, int n) { if (n > 0) Copy(dst, src, n); }
    static void CopyCharsOverlapped(CH* dst, const CH* src, int n) { if (n > 0) Move(dst, src, n); }

    // --- data pointer for the layout --------------------------------------------
    CH* m_pszData;

    static CH* data(CStringData* pd) { return reinterpret_cast<CH*>(pd + 1); }

private:
    static const CH* Empty0() { static const CH z = 0; return &z; }

    // Locale-aware collate via the Win32 comparison used by ATLStrTraitMFC.
    static int CollateImpl(const CH* a, const CH* b, DWORD flags) {
        int r;
        if (sizeof(CH) == 1)
            r = CompareStringA(LOCALE_USER_DEFAULT, flags, (LPCSTR)a, -1, (LPCSTR)b, -1);
        else
            r = CompareStringW(LOCALE_USER_DEFAULT, flags, (LPCWSTR)a, -1, (LPCWSTR)b, -1);
        return r == 0 ? 0 : r - 2;   // CSTR_LESS_THAN=1/EQUAL=2/GREATER=3 -> -1/0/1
    }

    void FormatVImpl(const CH* fmt, va_list args) {
        if (!fmt) { Empty(); return; }
        int cap = 128;
        for (;;) {
            CH* p = GetBuffer(cap);
            va_list ac; va_copy(ac, args);
            int w;
            if (sizeof(CH) == 1)
                w = vsnprintf((char*)p, cap + 1, (const char*)fmt, ac);
            else
                w = vswprintf((wchar_t*)p, cap + 1, (const wchar_t*)fmt, ac);
            va_end(ac);
            if (w >= 0 && w <= cap) { ReleaseBuffer(w); return; }
            ReleaseBuffer(0);
            cap = (w > cap) ? w : cap * 2;
            if (cap > (1 << 20)) { Empty(); return; }
        }
    }

    // ANSI<->wide conversion primitives (thread ANSI codepage, as _AtlGetConversionACP).
    static int ConvLen(const wchar_t* s) {   // wide -> narrow (this CH==char)
        return WideCharToMultiByte(CP_THREAD_ACP, 0, s, -1, nullptr, 0, nullptr, nullptr) - 1;
    }
    static int ConvLen(const char* s) {      // narrow -> wide (this CH==wchar_t)
        return MultiByteToWideChar(CP_THREAD_ACP, 0, s, -1, nullptr, 0) - 1;
    }
    static int ConvLenN(const wchar_t* s, int n) {
        return WideCharToMultiByte(CP_THREAD_ACP, 0, s, n, nullptr, 0, nullptr, nullptr);
    }
    static int ConvLenN(const char* s, int n) {
        return MultiByteToWideChar(CP_THREAD_ACP, 0, s, n, nullptr, 0);
    }
    static int ConvCopy(CH* dst, int cap, const wchar_t* s) {
        return WideCharToMultiByte(CP_THREAD_ACP, 0, s, -1, (char*)dst, cap + 1, nullptr, nullptr) - 1;
    }
    static int ConvCopy(CH* dst, int cap, const char* s) {
        return MultiByteToWideChar(CP_THREAD_ACP, 0, s, -1, (wchar_t*)dst, cap + 1) - 1;
    }
    static int ConvCopyN(CH* dst, int cap, const wchar_t* s, int n) {
        return WideCharToMultiByte(CP_THREAD_ACP, 0, s, n, (char*)dst, cap, nullptr, nullptr);
    }
    static int ConvCopyN(CH* dst, int cap, const char* s, int n) {
        return MultiByteToWideChar(CP_THREAD_ACP, 0, s, n, (wchar_t*)dst, cap);
    }

    static int Len(const CH* p) { int n = 0; while (p[n]) ++n; return n; }
    static void Copy(CH* d, const CH* s, int n) { for (int i = 0; i < n; ++i) d[i] = s[i]; }
    static void Move(CH* d, const CH* s, int n) {
        if (d < s) { for (int i = 0; i < n; ++i) d[i] = s[i]; }
        else { for (int i = n - 1; i >= 0; --i) d[i] = s[i]; }
    }
    static int Cmp(const CH* a, const CH* b) { while (*a && *a == *b) { ++a; ++b; } return (int)(unsigned)(*a) - (int)(unsigned)(*b); }
    static CH low(CH c) { return (c >= (CH)'A' && c <= (CH)'Z') ? (CH)(c + 32) : c; }
    static int CmpNoCase(const CH* a, const CH* b) { while (*a && low(*a) == low(*b)) { ++a; ++b; } return (int)(unsigned)low(*a) - (int)(unsigned)low(*b); }
    static void ToUpper(CH* p) { for (; *p; ++p) if (*p >= (CH)'a' && *p <= (CH)'z') *p -= 32; }
    static void ToLower(CH* p) { for (; *p; ++p) if (*p >= (CH)'A' && *p <= (CH)'Z') *p += 32; }

    int InsertPsz(int nIndex, const CH* psz, int nInsert) {
        if (nInsert <= 0) return GetLength();
        if (nIndex < 0) nIndex = 0;
        int nLen = GetLength();
        if (nIndex > nLen) nIndex = nLen;
        int nNew = nLen + nInsert;
        CH* p = GetBuffer(nNew);
        Move(p + nIndex + nInsert, p + nIndex, nLen - nIndex + 1);
        Copy(p + nIndex, psz, nInsert);
        ReleaseBuffer(nNew);
        return nNew;
    }

    CStringData* AllocData(int n) {
        IAtlStringMgr* mgr = OpenMFC_GetStringMgr();
        CStringData* pd = mgr->Allocate(n, sizeof(CH));
        if (!pd) { RaiseException(0xE06D7363, 0, 0, nullptr); return nullptr; }
        data(pd)[0] = 0;
        return pd;
    }
    void AllocBuffer(int n) { m_pszData = data(AllocData(n)); }
    static void FreeData(CStringData* pd) {
        if (pd != NilData<CH>() && !pd->IsLocked() && pd->pStringMgr) pd->pStringMgr->Free(pd);
    }
};

// VARIANT -> string: coerce a copy to VT_BSTR (thread locale) then take the wide text,
// converting to the narrow codepage for CStringA.  Matches CStringT's VARIANT ctor.
template <>
inline void AtlStrT<wchar_t>::ConstructVariant(const VARIANT* pv) {
    Construct();
    if (!pv) return;
    VARIANT v; VariantInit(&v);
    if (VariantChangeTypeEx(&v, const_cast<VARIANT*>(pv), GetThreadLocale(), 0, VT_BSTR) == S_OK) {
        if (v.bstrVal) ConstructPszN(v.bstrVal, (int)SysStringLen(v.bstrVal));
    }
    VariantClear(&v);
}
template <>
inline void AtlStrT<char>::ConstructVariant(const VARIANT* pv) {
    Construct();
    if (!pv) return;
    VARIANT v; VariantInit(&v);
    if (VariantChangeTypeEx(&v, const_cast<VARIANT*>(pv), GetThreadLocale(), 0, VT_BSTR) == S_OK) {
        if (v.bstrVal) ConstructConvertN<wchar_t>(v.bstrVal, (int)SysStringLen(v.bstrVal));
    }
    VariantClear(&v);
}

} // namespace openmfc_str
