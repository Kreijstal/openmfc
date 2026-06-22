// CWordArray - real, binary-compatible OpenMFC implementation.
//
// CWordArray is, in real MFC, `CArray<WORD, WORD>` deriving from CObject and
// declared with DECLARE_SERIAL. The faithful MSVC x64 layout is 40 bytes:
//   +0  vtable ptr (CObject base: vtbl[0]=GetRuntimeClass, vtbl[1]=dtor, vtbl[2]=Serialize)
//   +8  WORD*   m_pData
//   +16 INT_PTR m_nSize
//   +24 INT_PTR m_nMaxSize
//   +32 INT_PTR m_nGrowBy
// total sizeof == 40, 8-byte aligned.
//
// Note: include/openmfc/afx.h still carries a (wrong) `typedef CArray<WORD,WORD>
// CWordArray;`. Because that header is shared and we are forbidden from editing
// it, we cannot reuse the name `CWordArray` for a real class here. Instead the
// real class is named OpenMfcWordArray but its CRuntimeClass reports the genuine
// runtime name "CWordArray" and the genuine 40-byte object size, and every
// exported impl_ thunk operates on this faithful 40-byte layout. The exported
// mangled symbols (see // Symbol: markers) are exactly the CWordArray exports.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afx.h"

#include <cstring>
#include <cstdlib>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace openmfc_cwordarray {

// Faithful CWordArray layout. Derives from the repo's real CObject so the
// vtable base (GetRuntimeClass / dtor / Serialize) is ABI-correct.
class OpenMfcWordArray : public CObject {
public:
    DECLARE_SERIAL(OpenMfcWordArray)

    OpenMfcWordArray();
    virtual ~OpenMfcWordArray();
    virtual void Serialize(CArchive& ar) override;

    // CArray<WORD,WORD> surface used by the exports.
    INT_PTR GetSize() const { return m_nSize; }
    INT_PTR GetCount() const { return m_nSize; }
    bool IsEmpty() const { return m_nSize == 0; }
    INT_PTR GetUpperBound() const { return m_nSize - 1; }

    void SetSize(INT_PTR nNewSize, INT_PTR nGrowBy = -1);
    void FreeExtra();
    void RemoveAll() { SetSize(0, -1); }

    WORD GetAt(INT_PTR nIndex) const { return m_pData[nIndex]; }
    void SetAt(INT_PTR nIndex, WORD newElement) { m_pData[nIndex] = newElement; }
    WORD& ElementAt(INT_PTR nIndex) { return m_pData[nIndex]; }
    const WORD* GetData() const { return m_pData; }
    WORD* GetData() { return m_pData; }

    void SetAtGrow(INT_PTR nIndex, WORD newElement);
    INT_PTR Add(WORD newElement);
    INT_PTR Append(const OpenMfcWordArray& src);
    void Copy(const OpenMfcWordArray& src);

    WORD operator[](INT_PTR nIndex) const { return m_pData[nIndex]; }
    WORD& operator[](INT_PTR nIndex) { return m_pData[nIndex]; }

    void InsertAt(INT_PTR nIndex, WORD newElement, INT_PTR nCount = 1);
    void RemoveAt(INT_PTR nIndex, INT_PTR nCount = 1);
    void InsertAt(INT_PTR nStartIndex, OpenMfcWordArray* pNewArray);

private:
    WORD*   m_pData;     // +8
    INT_PTR m_nSize;     // +16
    INT_PTR m_nMaxSize;  // +24
    INT_PTR m_nGrowBy;   // +32

    void GrowBuffer(INT_PTR nNewMaxSize);
};

static_assert(sizeof(OpenMfcWordArray) == 40,
              "CWordArray must be 40 bytes (vptr + WORD* + 3 INT_PTR)");

// ---- CRuntimeClass with the genuine "CWordArray" name + 40-byte size --------
// We cannot use IMPLEMENT_SERIAL directly because the runtime name and object
// size must report "CWordArray"/40 rather than the internal class identifier.
CObject* AFXAPI _AfxGetObjectOpenMfcWordArray(CArchive& ar) {
    (void)ar;
    return OpenMfcWordArray::CreateObject();
}
CArchive& operator>>(CArchive& ar, OpenMfcWordArray*& pOb) {
    pOb = static_cast<OpenMfcWordArray*>(_AfxGetObjectOpenMfcWordArray(ar));
    if (pOb != nullptr) pOb->Serialize(ar);
    return ar;
}
CRuntimeClass OpenMfcWordArray::classOpenMfcWordArray = {
    "CWordArray",
    static_cast<int>(sizeof(OpenMfcWordArray)),
    0xFFFF,
    &OpenMfcWordArray::CreateObject,
    nullptr,
    &CObject::classCObject,
    nullptr
};

// ---- method bodies (real CArray<WORD,WORD> algorithm) -----------------------

OpenMfcWordArray::OpenMfcWordArray()
    : m_pData(nullptr), m_nSize(0), m_nMaxSize(0), m_nGrowBy(0) {}

OpenMfcWordArray::~OpenMfcWordArray() {
    if (m_pData != nullptr) {
        free(m_pData);
        m_pData = nullptr;
    }
    m_nSize = 0;
    m_nMaxSize = 0;
}

void OpenMfcWordArray::GrowBuffer(INT_PTR nNewMaxSize) {
    if (nNewMaxSize <= m_nMaxSize) return;
    WORD* pNew = static_cast<WORD*>(
        realloc(m_pData, static_cast<size_t>(nNewMaxSize) * sizeof(WORD)));
    if (pNew == nullptr) return;  // allocation failure: leave state intact
    m_pData = pNew;
    m_nMaxSize = nNewMaxSize;
}

void OpenMfcWordArray::SetSize(INT_PTR nNewSize, INT_PTR nGrowBy) {
    if (nNewSize < 0) return;
    if (nGrowBy != -1) m_nGrowBy = nGrowBy;

    if (nNewSize == 0) {
        if (m_pData != nullptr) {
            free(m_pData);
            m_pData = nullptr;
        }
        m_nSize = 0;
        m_nMaxSize = 0;
        return;
    }

    if (nNewSize > m_nMaxSize) {
        INT_PTR nGrow = m_nGrowBy;
        if (nGrow == 0) {
            // MFC default growth heuristic.
            nGrow = m_nSize / 8;
            if (nGrow < 4) nGrow = 4;
            if (nGrow > 1024) nGrow = 1024;
        }
        INT_PTR nNewMax = m_nMaxSize + nGrow;
        if (nNewMax < nNewSize) nNewMax = nNewSize;
        GrowBuffer(nNewMax);
        if (nNewSize > m_nMaxSize) return;  // grow failed
    }

    if (nNewSize > m_nSize) {
        // Zero-fill new elements (CArray value-initializes WORD to 0).
        memset(m_pData + m_nSize, 0,
               static_cast<size_t>(nNewSize - m_nSize) * sizeof(WORD));
    }
    m_nSize = nNewSize;
}

void OpenMfcWordArray::FreeExtra() {
    if (m_nSize == m_nMaxSize) return;
    if (m_nSize == 0) {
        if (m_pData != nullptr) {
            free(m_pData);
            m_pData = nullptr;
        }
        m_nMaxSize = 0;
        return;
    }
    WORD* pNew = static_cast<WORD*>(
        realloc(m_pData, static_cast<size_t>(m_nSize) * sizeof(WORD)));
    if (pNew != nullptr) {
        m_pData = pNew;
        m_nMaxSize = m_nSize;
    }
}

void OpenMfcWordArray::SetAtGrow(INT_PTR nIndex, WORD newElement) {
    if (nIndex < 0) return;
    if (nIndex >= m_nSize) SetSize(nIndex + 1, -1);
    if (nIndex < m_nSize) m_pData[nIndex] = newElement;
}

INT_PTR OpenMfcWordArray::Add(WORD newElement) {
    INT_PTR nIndex = m_nSize;
    SetAtGrow(nIndex, newElement);
    return nIndex;
}

INT_PTR OpenMfcWordArray::Append(const OpenMfcWordArray& src) {
    if (&src == this) return m_nSize;  // self-append not supported by MFC either
    INT_PTR nOldSize = m_nSize;
    SetSize(m_nSize + src.m_nSize, -1);
    if (m_nSize == nOldSize + src.m_nSize && src.m_nSize > 0) {
        memcpy(m_pData + nOldSize, src.m_pData,
               static_cast<size_t>(src.m_nSize) * sizeof(WORD));
    }
    return nOldSize;
}

void OpenMfcWordArray::Copy(const OpenMfcWordArray& src) {
    if (&src == this) return;
    SetSize(src.m_nSize, -1);
    if (m_nSize == src.m_nSize && src.m_nSize > 0) {
        memcpy(m_pData, src.m_pData,
               static_cast<size_t>(src.m_nSize) * sizeof(WORD));
    }
}

void OpenMfcWordArray::InsertAt(INT_PTR nIndex, WORD newElement, INT_PTR nCount) {
    if (nIndex < 0 || nCount <= 0) return;
    if (nIndex >= m_nSize) {
        // Grow array to fit, then fill the inserted slots.
        SetSize(nIndex + nCount, -1);
        if (m_nSize != nIndex + nCount) return;
    } else {
        INT_PTR nOldSize = m_nSize;
        SetSize(m_nSize + nCount, -1);
        if (m_nSize != nOldSize + nCount) return;
        // Shift the tail up by nCount.
        memmove(m_pData + nIndex + nCount, m_pData + nIndex,
                static_cast<size_t>(nOldSize - nIndex) * sizeof(WORD));
    }
    for (INT_PTR i = 0; i < nCount; ++i) {
        m_pData[nIndex + i] = newElement;
    }
}

void OpenMfcWordArray::RemoveAt(INT_PTR nIndex, INT_PTR nCount) {
    if (nIndex < 0 || nCount <= 0 || nIndex >= m_nSize) return;
    if (nIndex + nCount > m_nSize) nCount = m_nSize - nIndex;
    INT_PTR nMoveCount = m_nSize - (nIndex + nCount);
    if (nMoveCount > 0) {
        memmove(m_pData + nIndex, m_pData + nIndex + nCount,
                static_cast<size_t>(nMoveCount) * sizeof(WORD));
    }
    m_nSize -= nCount;
}

void OpenMfcWordArray::InsertAt(INT_PTR nStartIndex, OpenMfcWordArray* pNewArray) {
    if (pNewArray == nullptr || nStartIndex < 0) return;
    INT_PTR nCount = pNewArray->m_nSize;
    if (nCount <= 0) return;
    // Open a hole using the value form, then overwrite with source contents.
    InsertAt(nStartIndex, static_cast<WORD>(0), nCount);
    if (nStartIndex + nCount <= m_nSize) {
        memcpy(m_pData + nStartIndex, pNewArray->m_pData,
               static_cast<size_t>(nCount) * sizeof(WORD));
    }
}

void OpenMfcWordArray::Serialize(CArchive& ar) {
    if (ar.IsStoring()) {
        ar << static_cast<unsigned int>(m_nSize);
        for (INT_PTR i = 0; i < m_nSize; ++i) {
            ar << m_pData[i];
        }
    } else {
        unsigned int nCount = 0;
        ar >> nCount;
        SetSize(static_cast<INT_PTR>(nCount), -1);
        for (INT_PTR i = 0; i < m_nSize; ++i) {
            ar >> m_pData[i];
        }
    }
}

} // namespace openmfc_cwordarray

using openmfc_cwordarray::OpenMfcWordArray;

// ===========================================================================
// Exported C thunks (MS ABI). One per research.exports symbol. The // Symbol:
// markers let the build drop the corresponding weak stub.
// ===========================================================================

// Symbol: ??0CWordArray@@QEAA@XZ
extern "C" void* MS_ABI impl___0CWordArray__QEAA_XZ(OpenMfcWordArray* pThis) {
    return pThis ? new (pThis) OpenMfcWordArray() : nullptr;
}

// Symbol: ??1CWordArray@@UEAA@XZ
extern "C" void MS_ABI impl___1CWordArray__UEAA_XZ(OpenMfcWordArray* pThis) {
    if (pThis) pThis->~OpenMfcWordArray();
}

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCWordArray@@@Z
extern "C" CArchive* MS_ABI impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCWordArray___Z(
        CArchive* ar, OpenMfcWordArray** pOb) {
    if (!ar || !pOb) return ar;
    return &(openmfc_cwordarray::operator>>(*ar, *pOb));
}

// Symbol: ?Append@CWordArray@@QEAA_JAEBV1@@Z
extern "C" long long MS_ABI impl__Append_CWordArray__QEAA_JAEBV1__Z(
        OpenMfcWordArray* pThis, const OpenMfcWordArray* pSrc) {
    return (pThis && pSrc) ? pThis->Append(*pSrc) : 0;
}

// Symbol: ?Copy@CWordArray@@QEAAXAEBV1@@Z
extern "C" void MS_ABI impl__Copy_CWordArray__QEAAXAEBV1__Z(
        OpenMfcWordArray* pThis, const OpenMfcWordArray* pSrc) {
    if (pThis && pSrc) pThis->Copy(*pSrc);
}

// Symbol: ?CreateObject@CWordArray@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CWordArray__SAPEAVCObject__XZ() {
    return OpenMfcWordArray::CreateObject();
}

// Symbol: ?FreeExtra@CWordArray@@QEAAXXZ
extern "C" void MS_ABI impl__FreeExtra_CWordArray__QEAAXXZ(OpenMfcWordArray* pThis) {
    if (pThis) pThis->FreeExtra();
}

// Symbol: ?GetRuntimeClass@CWordArray@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CWordArray__UEBAPEAUCRuntimeClass__XZ(
        const OpenMfcWordArray* pThis) {
    return OpenMfcWordArray::GetThisClass();
}

// Symbol: ?GetThisClass@CWordArray@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWordArray__SAPEAUCRuntimeClass__XZ() {
    return OpenMfcWordArray::GetThisClass();
}

// Symbol: ?InsertAt@CWordArray@@QEAAX_JG0@Z
extern "C" void MS_ABI impl__InsertAt_CWordArray__QEAAX_JG0_Z(
        OpenMfcWordArray* pThis, long long nIndex, unsigned short value, long long nCount) {
    if (pThis) pThis->InsertAt(nIndex, value, nCount);
}

// Symbol: ?InsertAt@CWordArray@@QEAAX_JPEAV1@@Z
extern "C" void MS_ABI impl__InsertAt_CWordArray__QEAAX_JPEAV1__Z(
        OpenMfcWordArray* pThis, long long nIndex, OpenMfcWordArray* pNewArray) {
    if (pThis) pThis->InsertAt(nIndex, pNewArray);
}

// Symbol: ?RemoveAt@CWordArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__RemoveAt_CWordArray__QEAAX_J0_Z(
        OpenMfcWordArray* pThis, long long nIndex, long long nCount) {
    if (pThis) pThis->RemoveAt(nIndex, nCount);
}

// Symbol: ?Serialize@CWordArray@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CWordArray__UEAAXAEAVCArchive___Z(
        OpenMfcWordArray* pThis, CArchive& ar) {
    if (pThis) pThis->Serialize(ar);
}

// Symbol: ?SetAtGrow@CWordArray@@QEAAX_JG@Z
extern "C" void MS_ABI impl__SetAtGrow_CWordArray__QEAAX_JG_Z(
        OpenMfcWordArray* pThis, long long nIndex, unsigned short value) {
    if (pThis) pThis->SetAtGrow(nIndex, value);
}

// Symbol: ?SetSize@CWordArray@@QEAAX_J0@Z
extern "C" void MS_ABI impl__SetSize_CWordArray__QEAAX_J0_Z(
        OpenMfcWordArray* pThis, long long nNewSize, long long nGrowBy) {
    if (pThis) pThis->SetSize(nNewSize, nGrowBy);
}
