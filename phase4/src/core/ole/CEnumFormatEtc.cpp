// CEnumFormatEtc — OpenMFC implementation.
// Sources: global_ctor_quickwins.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

// Hand-written real implementations for header-defined classes that were one
// export short of complete. Constructors placement-new into the caller-provided
// storage so the openmfc C++ constructor establishes the validated member
// layout. Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (scan of
// `// Symbol:` comments) removes the weak/typed stubs for these ordinals
// automatically.

#include "openmfc/afxwin.h"
#include "openmfc/afxinet.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxole.h"

#include <cstdio>
#include <new>
#include <cstdlib>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Runtime-class descriptors used by COleException::Process, defined alongside
// the exception RTTI tables (ole_oleexception_rtti.cpp,
// global_simple_exceptions_rtti.cpp).
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CNotSupportedException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CInvalidArgException__SAPEAUCRuntimeClass__XZ();

extern "C" void MS_ABI impl__CommonInit_CFile__IEAAXPEB_WIPEAVCAtlTransactionManager_ATL___Z(
    void* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* pTM);
extern "C" void MS_ABI impl__CommonBaseInit_CStdioFile__IEAAXPEAU_iobuf__PEAVCAtlTransactionManager_ATL___Z(
    void* pThis, FILE* pStream, void* pTM);
extern "C" void MS_ABI impl__CommonInit_CStdioFile__IEAAXPEB_WIPEAVCAtlTransactionManager_ATL___Z(
    void* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, void* pTM);















// CEnumFormatEtc::OnNext(void*) — MFC's CEnumArray hook that fetches the single
// next element into the caller's buffer and advances the cursor, returning TRUE
// while elements remain. Implemented over the class's own public IEnumFORMATETC
// ::Next, so it stays consistent with m_position/m_count bookkeeping.
// Symbol: ?OnNext@CEnumFormatEtc@@MEAAHPEAX@Z
extern "C" int MS_ABI impl__OnNext_CEnumFormatEtc__MEAAHPEAX_Z(CEnumFormatEtc* pThis, void* pv)
{
    if (!pThis || !pv) return FALSE;
    ULONG fetched = 0;
    HRESULT hr = pThis->Next(1, static_cast<FORMATETC*>(pv), &fetched);
    return (hr == S_OK && fetched == 1) ? TRUE : FALSE;
}
CEnumFormatEtc::CEnumFormatEtc() : m_refCount(1), m_formats(nullptr), m_count(0), m_capacity(0), m_position(0) { memset(_cenumformatetc_padding, 0, sizeof(_cenumformatetc_padding)); }
CEnumFormatEtc::~CEnumFormatEtc() { if (m_formats) free(m_formats); }
STDMETHODIMP CEnumFormatEtc::QueryInterface(REFIID riid, void** ppv) {
    if (riid == IID_IUnknown || riid == IID_IEnumFORMATETC) { *ppv = this; AddRef(); return S_OK; }
    *ppv = nullptr; return E_NOINTERFACE;
}
STDMETHODIMP CEnumFormatEtc::Next(ULONG celt, FORMATETC* rgelt, ULONG* pceltFetched) {
    ULONG fetched = 0;
    while (m_position < m_count && fetched < celt) {
        rgelt[fetched] = m_formats[m_position];
        if (rgelt[fetched].ptd) rgelt[fetched].ptd = nullptr; // Don't copy DVTARGETDEVICE
        m_position++;
        fetched++;
    }
    if (pceltFetched) *pceltFetched = fetched;
    return (fetched == celt) ? S_OK : S_FALSE;
}
STDMETHODIMP CEnumFormatEtc::Skip(ULONG celt) {
    if (m_position + celt > m_count) { m_position = m_count; return S_FALSE; }
    m_position += celt;
    return S_OK;
}
STDMETHODIMP CEnumFormatEtc::Reset() { m_position = 0; return S_OK; }
STDMETHODIMP CEnumFormatEtc::Clone(IEnumFORMATETC** ppEnum) {
    if (!ppEnum) return E_POINTER;
    CEnumFormatEtc* clone = new CEnumFormatEtc();
    for (ULONG i = 0; i < m_count; ++i) {
        clone->AddFormat(m_formats[i]);
    }
    clone->m_position = m_position;
    *ppEnum = clone;
    return S_OK;
}
void CEnumFormatEtc::AddFormat(const FORMATETC& formatEtc) {
    if (m_count >= m_capacity) {
        m_capacity = m_capacity ? m_capacity * 2 : 8;
        m_formats = (FORMATETC*)realloc(m_formats, m_capacity * sizeof(FORMATETC));
    }
    if (m_formats) m_formats[m_count++] = formatEtc;
}

// === Moved from ManualThunks.cpp ===
// Symbol: ?AddFormat@CEnumFormatEtc@@QEAAXPEBUtagFORMATETC@@@Z
extern "C" void MS_ABI impl__AddFormat_CEnumFormatEtc__QEAAXPEBUtagFORMATETC___Z(void* pThis, void* p0) {
    (void)pThis;
    (void)p0;
}

