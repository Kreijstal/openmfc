// CArchive — OpenMFC implementation.
// Sources: filecore.cpp, global_ctordtor_param.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/FilecoreSupport.h"

// Real parameterized-constructor export thunks for header-defined, publicly
// constructible MFC classes. Each viewed-as MS_ABI thunk placement-constructs
// the object with the real ctor body (defined in filecore/olecore/dlgcommon/
// inetcore or inline in headers) -- NOT a stub. Completes classes that were one
// export short of full coverage.
#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxdb.h"
#include "openmfc/afxinet.h"
#include <new>
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


















// Symbol: ?FillBuffer@CArchive@@QEAAXI@Z
extern "C" void MS_ABI impl__FillBuffer_CArchive__QEAAXI_Z(CArchive* pThis, unsigned int nBytesNeeded) {
    CArchiveAccess::InvokeFillBuffer(pThis, nBytesNeeded);
}
// Symbol: ?GetObjectSchema@CArchive@@QEAAIXZ
extern "C" unsigned int MS_ABI impl__GetObjectSchema_CArchive__QEAAIXZ(CArchive* pThis) {
    return pThis ? pThis->GetObjectSchema() : 0;
}
// Symbol: ?WriteCount@CArchive@@QEAAX_K@Z
extern "C" void MS_ABI impl__WriteCount_CArchive__QEAAX_K_Z(CArchive* pThis, unsigned long long count) {
    if (!pThis) return;
    pThis->Write(&count, sizeof(count));
}
// Symbol: ?ReadCount@CArchive@@QEAA_KXZ
extern "C" unsigned long long MS_ABI impl__ReadCount_CArchive__QEAA_KXZ(CArchive* pThis) {
    unsigned long long count = 0;
    if (pThis) pThis->Read(&count, sizeof(count));
    return count;
}
// Symbol: ?CheckCount@CArchive@@QEAAXXZ
extern "C" void MS_ABI impl__CheckCount_CArchive__QEAAXXZ(CArchive* pThis) {
    if (pThis) pThis->Flush();
}
// Symbol: ?WriteClass@CArchive@@QEAAXPEBUCRuntimeClass@@@Z
extern "C" void MS_ABI impl__WriteClass_CArchive__QEAAXPEBUCRuntimeClass___Z(CArchive* pThis, const CRuntimeClass* pClass) {
    if (pThis && pClass) impl__Store_CRuntimeClass__QEBAXAEAVCArchive___Z(pClass, pThis);
}
// Symbol: ?SerializeClass@CArchive@@QEAAXPEBUCRuntimeClass@@@Z
extern "C" void MS_ABI impl__SerializeClass_CArchive__QEAAXPEBUCRuntimeClass___Z(CArchive* pThis, const CRuntimeClass* pClass) {
    if (pThis && pThis->IsStoring()) impl__WriteClass_CArchive__QEAAXPEBUCRuntimeClass___Z(pThis, pClass);
    else if (pThis) {
        unsigned int schema = 0;
        (void)impl__Load_CRuntimeClass__SAPEAU1_AEAVCArchive__PEAI_Z(pThis, &schema);
        pThis->SetObjectSchema(schema);
    }
}
// Symbol: ?ReadClass@CArchive@@QEAAPEAUCRuntimeClass@@PEBU2@PEAIPEAK@Z
extern "C" CRuntimeClass* MS_ABI impl__ReadClass_CArchive__QEAAPEAUCRuntimeClass__PEBU2_PEAIPEAK_Z(
    CArchive* pThis, const CRuntimeClass*, unsigned int* schema, unsigned long*) {
    return pThis ? impl__Load_CRuntimeClass__SAPEAU1_AEAVCArchive__PEAI_Z(pThis, schema) : nullptr;
}
// Symbol: ?WriteObject@CArchive@@QEAAXPEBVCObject@@@Z
extern "C" void MS_ABI impl__WriteObject_CArchive__QEAAXPEBVCObject___Z(CArchive* pThis, const CObject* object) {
    if (pThis) (*pThis) << object;
}
// Symbol: ?ReadObject@CArchive@@QEAAPEAVCObject@@PEBUCRuntimeClass@@@Z
extern "C" CObject* MS_ABI impl__ReadObject_CArchive__QEAAPEAVCObject__PEBUCRuntimeClass___Z(CArchive* pThis, const CRuntimeClass*) {
    CObject* object = nullptr;
    if (pThis) (*pThis) >> object;
    return object;
}
// Symbol: ?ReadString@CArchive@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__ReadString_CArchive__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(CArchive* pThis, CString* stringOut) {
    return (pThis && stringOut) ? pThis->ReadString(*stringOut) : 0;
}
// Symbol: ?MapObject@CArchive@@QEAAXPEBVCObject@@@Z
extern "C" void MS_ABI impl__MapObject_CArchive__QEAAXPEBVCObject___Z(CArchive*, const CObject*) {
}
// Symbol: ?EnsureSchemaMapExists@CArchive@@QEAAXPEAPEAV?$CArray@W4LoadArrayObjType@CArchive@@AEBW412@@@@Z
extern "C" void MS_ABI impl__EnsureSchemaMapExists_CArchive__QEAAXPEAPEAV__CArray_W4LoadArrayObjType_CArchive__AEBW412____Z(CArchive*, void**) {
}
CArchive::CArchive(CFile* pFile, UINT nMode, int nBufSize, void* lpBuf)
    : m_pFile(pFile), m_nMode(nMode), m_nBufSize(nBufSize),
      m_nObjectSchema(0), m_bForceFlat(false)
{
    if (lpBuf) {
        m_lpBufStart = static_cast<unsigned char*>(lpBuf);
        m_bUserBuf = true;
    } else {
        m_lpBufStart = new unsigned char[nBufSize];
        m_bUserBuf = false;
    }

    if (IsLoading()) {
        m_lpBufCur = m_lpBufStart;
        m_lpBufMax = m_lpBufStart;  // Empty buffer, will be filled on first read
    } else {
        m_lpBufCur = m_lpBufStart;
        m_lpBufMax = m_lpBufStart + nBufSize;
    }
}
CArchive::~CArchive() {
    if (!(m_nMode & bNoFlushOnDelete)) {
        Flush();
    }
    if (!m_bUserBuf) {
        delete[] m_lpBufStart;
    }
}
void CArchive::FillBuffer(UINT nBytesNeeded) {
    if (!m_pFile || !m_lpBufStart || m_nBufSize <= 0) return;

    // Move remaining data to start of buffer
    UINT nRemaining = (UINT)(m_lpBufMax - m_lpBufCur);
    if (nRemaining > 0 && m_lpBufCur != m_lpBufStart) {
        memmove(m_lpBufStart, m_lpBufCur, nRemaining);
    }
    m_lpBufCur = m_lpBufStart;

    if (nRemaining >= nBytesNeeded) {
        m_lpBufMax = m_lpBufStart + nRemaining;
        return;
    }

    UINT capacity = static_cast<UINT>(m_nBufSize);
    UINT bytesToRead = capacity - nRemaining;

    UINT nRead = OpenMFC_File_Read(m_pFile, m_lpBufStart + nRemaining, bytesToRead);
    m_lpBufMax = m_lpBufStart + nRemaining + nRead;
}
void CArchive::WriteBuffer() {
    if (!m_pFile) return;

    UINT nBytes = (UINT)(m_lpBufCur - m_lpBufStart);
    if (nBytes > 0) {
        OpenMFC_File_Write(m_pFile, m_lpBufStart, nBytes);
        m_lpBufCur = m_lpBufStart;
    }
}
UINT CArchive::Read(void* lpBuf, UINT nMax) {
    if (!IsLoading() || !lpBuf || nMax == 0) return 0;

    UINT nRead = 0;
    unsigned char* pDest = static_cast<unsigned char*>(lpBuf);

    while (nRead < nMax) {
        UINT nAvail = (UINT)(m_lpBufMax - m_lpBufCur);
        if (nAvail == 0) {
            FillBuffer(nMax - nRead);
            nAvail = (UINT)(m_lpBufMax - m_lpBufCur);
            if (nAvail == 0) break;  // EOF
        }

        UINT nCopy = (nMax - nRead < nAvail) ? (nMax - nRead) : nAvail;
        memcpy(pDest + nRead, m_lpBufCur, nCopy);
        m_lpBufCur += nCopy;
        nRead += nCopy;
    }

    return nRead;
}
void CArchive::Write(const void* lpBuf, UINT nMax) {
    if (!IsStoring() || !lpBuf || nMax == 0) return;

    const unsigned char* pSrc = static_cast<const unsigned char*>(lpBuf);
    UINT nWritten = 0;

    while (nWritten < nMax) {
        UINT nAvail = (UINT)(m_lpBufMax - m_lpBufCur);
        if (nAvail == 0) {
            WriteBuffer();
            nAvail = (UINT)(m_lpBufMax - m_lpBufCur);
        }

        UINT nCopy = (nMax - nWritten < nAvail) ? (nMax - nWritten) : nAvail;
        memcpy(m_lpBufCur, pSrc + nWritten, nCopy);
        m_lpBufCur += nCopy;
        nWritten += nCopy;
    }
}
void CArchive::Flush() {
    if (IsStoring()) {
        WriteBuffer();
    }
    if (m_pFile) {
        OpenMFC_File_Flush(m_pFile);
    }
}
void CArchive::Close() {
    Flush();
}
void CArchive::Abort() {
    // Reset without flushing
    m_lpBufCur = m_lpBufStart;
    if (IsLoading()) {
        m_lpBufMax = m_lpBufStart;
    }
}
CArchive& CArchive::operator>>(bool& b) {
    char ch;
    Read(&ch, sizeof(ch));
    b = (ch != 0);
    return *this;
}
CArchive& CArchive::operator>>(char& ch) {
    Read(&ch, sizeof(ch));
    return *this;
}
CArchive& CArchive::operator>>(unsigned char& ch) {
    Read(&ch, sizeof(ch));
    return *this;
}
CArchive& CArchive::operator>>(short& w) {
    Read(&w, sizeof(w));
    return *this;
}
CArchive& CArchive::operator>>(unsigned short& w) {
    Read(&w, sizeof(w));
    return *this;
}
CArchive& CArchive::operator>>(int& i) {
    Read(&i, sizeof(i));
    return *this;
}
CArchive& CArchive::operator>>(unsigned int& i) {
    Read(&i, sizeof(i));
    return *this;
}
CArchive& CArchive::operator>>(long& l) {
    Read(&l, sizeof(l));
    return *this;
}
CArchive& CArchive::operator>>(unsigned long& l) {
    Read(&l, sizeof(l));
    return *this;
}
CArchive& CArchive::operator>>(float& f) {
    Read(&f, sizeof(f));
    return *this;
}
CArchive& CArchive::operator>>(double& d) {
    Read(&d, sizeof(d));
    return *this;
}
CArchive& CArchive::operator>>(CObject*& pOb) {
    unsigned char hasObject = 0;
    if (Read(&hasObject, sizeof(hasObject)) != sizeof(hasObject) || hasObject == 0) {
        pOb = nullptr;
        return *this;
    }

    unsigned int nSchema = 0xFFFF;
    CRuntimeClass* pClass = impl__Load_CRuntimeClass__SAPEAU1_AEAVCArchive__PEAI_Z(this, &nSchema);
    SetObjectSchema(nSchema);

    if (!pClass || !pClass->m_pfnCreateObject) {
        pOb = nullptr;
        return *this;
    }

    pOb = pClass->CreateObject();
    if (pOb) {
        pOb->Serialize(*this);
    }
    return *this;
}
CArchive& CArchive::operator>>(void*& p) {
    uintptr_t value = 0;
    Read(&value, sizeof(value));
    p = reinterpret_cast<void*>(value);
    return *this;
}
CArchive& CArchive::operator>>(CString& str) {
    // Read string length
    UINT nLen = 0;
    Read(&nLen, sizeof(nLen));

    if (nLen > 0 && nLen < 0x10000000) {
        wchar_t* pBuf = str.GetBuffer(nLen + 1);
        Read(pBuf, nLen * sizeof(wchar_t));
        pBuf[nLen] = L'\0';
        str.ReleaseBuffer(nLen);
    } else {
        str.Empty();
    }
    return *this;
}
CArchive& CArchive::operator<<(bool b) {
    char ch = b ? 1 : 0;
    Write(&ch, sizeof(ch));
    return *this;
}
CArchive& CArchive::operator<<(char ch) {
    Write(&ch, sizeof(ch));
    return *this;
}
CArchive& CArchive::operator<<(unsigned char ch) {
    Write(&ch, sizeof(ch));
    return *this;
}
CArchive& CArchive::operator<<(short w) {
    Write(&w, sizeof(w));
    return *this;
}
CArchive& CArchive::operator<<(unsigned short w) {
    Write(&w, sizeof(w));
    return *this;
}
CArchive& CArchive::operator<<(int i) {
    Write(&i, sizeof(i));
    return *this;
}
CArchive& CArchive::operator<<(unsigned int i) {
    Write(&i, sizeof(i));
    return *this;
}
CArchive& CArchive::operator<<(long l) {
    Write(&l, sizeof(l));
    return *this;
}
CArchive& CArchive::operator<<(unsigned long l) {
    Write(&l, sizeof(l));
    return *this;
}
CArchive& CArchive::operator<<(float f) {
    Write(&f, sizeof(f));
    return *this;
}
CArchive& CArchive::operator<<(double d) {
    Write(&d, sizeof(d));
    return *this;
}
CArchive& CArchive::operator<<(const CObject* pOb) {
    unsigned char hasObject = (pOb != nullptr) ? 1 : 0;
    Write(&hasObject, sizeof(hasObject));

    if (!hasObject) {
        return *this;
    }

    const CRuntimeClass* pClass = pOb->GetRuntimeClass();
    if (pClass == nullptr) {
        pClass = &CObject::classCObject;
    }
    impl__Store_CRuntimeClass__QEBAXAEAVCArchive___Z(pClass, this);

    // Conservative format: only write payload if load side can create the object.
    if (pClass->m_pfnCreateObject) {
        const_cast<CObject*>(pOb)->Serialize(*this);
    }
    return *this;
}
CArchive& CArchive::operator<<(const void* p) {
    uintptr_t value = reinterpret_cast<uintptr_t>(p);
    Write(&value, sizeof(value));
    return *this;
}
CArchive& CArchive::operator<<(const CString& str) {
    // Write string length and data
    UINT nLen = str.GetLength();
    Write(&nLen, sizeof(nLen));

    if (nLen > 0) {
        Write((const wchar_t*)str, nLen * sizeof(wchar_t));
    }
    return *this;
}
wchar_t* CArchive::ReadString(wchar_t* lpsz, UINT nMax) {
    if (!lpsz || nMax == 0) return nullptr;

    UINT nRead = 0;
    while (nRead < nMax - 1) {
        wchar_t ch;
        if (Read(&ch, sizeof(ch)) != sizeof(ch)) break;
        if (ch == L'\n') {
            lpsz[nRead++] = ch;
            break;
        }
        if (ch == L'\r') continue;  // Skip CR
        lpsz[nRead++] = ch;
    }
    lpsz[nRead] = L'\0';
    return nRead != 0 ? lpsz : nullptr;
}
int CArchive::ReadString(CString& rString) {
    rString.Empty();
    wchar_t buf[256];
    int nTotal = 0;

    while (true) {
        if (!ReadString(buf, 256)) break;
        int nRead = static_cast<int>(wcslen(buf));
        rString += buf;
        nTotal += nRead;
        if (buf[nRead - 1] == L'\n') break;
    }
    return nTotal;
}
void CArchive::WriteString(const wchar_t* lpsz) {
    if (!lpsz) return;
    UINT nLen = (UINT)wcslen(lpsz);
    Write(lpsz, nLen * sizeof(wchar_t));
}
// Symbol: ??0CArchive@@QEAA@PEAVCFile@@IHPEAX@Z
extern "C" void* MS_ABI impl___0CArchive__QEAA_PEAVCFile__IHPEAX_Z(
        void* p, CFile* pFile, unsigned int nMode, int nBufSize, void* lpBuf) {
    return new (p) CArchive(pFile, nMode, nBufSize, lpBuf);
}
