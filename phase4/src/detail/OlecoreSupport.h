#pragma once
// Shared internals of the former olecore.cpp translation unit.
// Definitions live in detail/OlecoreSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace olecore {} } }
using namespace openmfc::detail::olecore;
// OLE/COM Implementation
// Provides MFC wrappers around the OLE32/OLEAUT32 COM APIs
//
// All exports use MSVC ABI via extern "C" + .def aliasing.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"
#include <algorithm>
#include <cstring>
#include <cwchar>
#include <cstdio>
#include <docobj.h>
#include <map>
#include <new>
#include <shlobj.h>
#include <vector>
#include "openmfc/afxdao.h"

// MinGW compat: Ambient property DISPIDs
#ifndef DISPID_AMBIENT_BACKCOLOR
#define DISPID_AMBIENT_BACKCOLOR    (-701)
#define DISPID_AMBIENT_DISPLAYNAME  (-702)
#define DISPID_AMBIENT_FONT         (-703)
#define DISPID_AMBIENT_FORECOLOR    (-704)
#define DISPID_AMBIENT_LOCALEID     (-705)
#define DISPID_AMBIENT_USERMODE     (-709)
#define DISPID_AMBIENT_UIDEAD       (-710)
#define DISPID_AMBIENT_SHOWGRABHANDLES (-711)
#define DISPID_AMBIENT_SHOWHATCHING (-712)
#define DISPID_AMBIENT_DISPLAYASDEFAULT (-713)
#define DISPID_AMBIENT_SUPPORTSMNEMONICS (-714)
#define DISPID_AMBIENT_APPEARANCE   (-716)
#define DISPID_AMBIENT_SCALEUNITS   (-807)
#endif
#ifndef DISPID_AMBIENT_TEXTALIGN
#define DISPID_AMBIENT_TEXTALIGN    (-708)
#endif
#ifndef VT_COLOR
#define VT_COLOR 0x0000000CL
#endif

// Stock control event DISPIDs (from olectl.h)
#ifndef DISPID_CLICK
#include <olectl.h>
#endif

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" int MS_ABI impl__OnAmbientProperty_CWnd__UEAAHPEAVCOleControlSite__JPEAUtagVARIANT___Z(
    CWnd* pThis, COleControlSite* pSite, long dispid, VARIANT* pVar);
extern "C" void MS_ABI impl__SetFont_CFontHolder__QEAAXPEAUIFont___Z(void* self, void* font);
extern "C" void* MS_ABI impl__GetFontDispatch_CFontHolder__QEAAPEAUIFontDisp__XZ(void* self);
extern "C" HFONT MS_ABI impl__GetFontHandle_CFontHolder__QEAAPEAUHFONT____XZ(void* self);

//=============================================================================
// Base classes needed by OLE
//=============================================================================


namespace openmfc { namespace detail { namespace olecore {
struct CCmdUIShim_Olecore {
    void* vftable;
    UINT m_nID;
    CCmdUI* m_pOther;
    int m_nIndex;
    CMenu* m_pMenu;
    CMenu* m_pSubMenu;
    char _padding[8];
};
} } }

namespace openmfc { namespace detail { namespace olecore {
CMenu* CCmdUI_Menu(const CCmdUI* pThis);
} } }

namespace openmfc { namespace detail { namespace olecore {
bool CCmdUI_IsValidMenu(const CCmdUI* pThis);
} } }

namespace openmfc { namespace detail { namespace olecore {
bool CCmdUI_IdValid(UINT id);
} } }

namespace openmfc { namespace detail { namespace olecore {
bool CCmdUI_HasByPositionTarget(const CCmdUI* pThis, UINT* itemID, UINT* flags);
} } }

void CCmdUI_Delegated(CCmdUI* pThis, void (CCmdUI::*fn)(int), int arg);

void CCmdUI_DelegatedText(CCmdUI* pThis, void (CCmdUI::*fn)(const wchar_t*), const wchar_t* text);

namespace openmfc { namespace detail { namespace olecore {
void CCmdUIEnableForMenu(CCmdUI* pThis, BOOL bOn);
} } }

namespace openmfc { namespace detail { namespace olecore {
void CCmdUICheckForMenu(CCmdUI* pThis, int nCheck);
} } }

namespace openmfc { namespace detail { namespace olecore {
void CCmdUISetTextForMenu(CCmdUI* pThis, const wchar_t* lpszText);
} } }







//=============================================================================
// CControlBar exports (extern "C" MS_ABI impl_ functions)
//=============================================================================


















































//=============================================================================
// OLE State
//=============================================================================
namespace openmfc { namespace detail { namespace olecore {
extern int g_bOleInitialized;
} } }
namespace openmfc { namespace detail { namespace olecore {
extern int g_nOleLockCount;
} } }
namespace openmfc { namespace detail { namespace olecore {
extern BOOL g_bOleUserCtrl;
} } }
namespace openmfc { namespace detail { namespace olecore {
extern COleMessageFilter* g_pMessageFilter;
} } }
namespace openmfc { namespace detail { namespace olecore {
extern COleDataSource* g_pClipboardOwner;
} } }
namespace openmfc { namespace detail { namespace olecore {
extern std::map<CString, CString> g_oleFactoryLicenseKeys;
} } }
namespace openmfc { namespace detail { namespace olecore {
extern BOOL g_userOleControlMode;
} } }


namespace openmfc { namespace detail { namespace olecore {
struct DataCacheEntry {
    FORMATETC format = {};
    STGMEDIUM medium = {};
    bool hasMedium = false;
    bool delayRender = false;
    bool delayRenderFile = false;
};
} } }

namespace openmfc { namespace detail { namespace olecore {
extern std::map<void*, COleControl*> g_reflectorControls_Olecore;
} } }

namespace openmfc { namespace detail { namespace olecore {
FORMATETC MakeFormatEtc(CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc);
} } }

namespace openmfc { namespace detail { namespace olecore {
bool FormatMatches(const FORMATETC& cached, const FORMATETC& requested);
} } }

namespace openmfc { namespace detail { namespace olecore {
HGLOBAL DuplicateGlobalMemory(HGLOBAL source);
} } }

namespace openmfc { namespace detail { namespace olecore {
bool CopyStorageMedium(const STGMEDIUM& source, STGMEDIUM* dest);
} } }

namespace openmfc { namespace detail { namespace olecore {
LPOLESTR CopyOleString(const wchar_t* text);
} } }

namespace openmfc { namespace detail { namespace olecore {
bool MakeRenderedFileMedium(COleDataSource* source, FORMATETC* format, STGMEDIUM* medium);
} } }

namespace openmfc { namespace detail { namespace olecore {
class EmptyEnumConnectionPoints : public IEnumConnectionPoints {
public:
    EmptyEnumConnectionPoints() : m_refCount(1) {}
    virtual ~EmptyEnumConnectionPoints() {}

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override {
        if (!ppvObject) return E_POINTER;
        if (riid == IID_IUnknown || riid == IID_IEnumConnectionPoints) {
            *ppvObject = static_cast<IEnumConnectionPoints*>(this);
            AddRef();
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }

    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&m_refCount); }

    ULONG STDMETHODCALLTYPE Release() override {
        ULONG ref = InterlockedDecrement(&m_refCount);
        if (ref == 0) delete this;
        return ref;
    }

    HRESULT STDMETHODCALLTYPE Next(ULONG cConnections, IConnectionPoint** rgpcn, ULONG* pcFetched) override {
        if (pcFetched) *pcFetched = 0;
        if (cConnections == 0) return S_OK;
        if (cConnections > 0 && !rgpcn) return E_POINTER;
        if (cConnections > 1 && !pcFetched) return E_POINTER;
        return S_FALSE;
    }

    HRESULT STDMETHODCALLTYPE Skip(ULONG) override { return S_FALSE; }
    HRESULT STDMETHODCALLTYPE Reset() override { return S_OK; }

    HRESULT STDMETHODCALLTYPE Clone(IEnumConnectionPoints** ppEnum) override {
        if (!ppEnum) return E_POINTER;
        *ppEnum = new (std::nothrow) EmptyEnumConnectionPoints();
        return *ppEnum ? S_OK : E_OUTOFMEMORY;
    }

private:
    LONG m_refCount;
};
} } }

namespace openmfc { namespace detail { namespace olecore {
void InitializeDocumentView(COleDocObjectItem* item, IOleDocumentView* view);
} } }

namespace openmfc { namespace detail { namespace olecore {
int CountDispatchParams(const BYTE* pbParamInfo);
} } }

namespace openmfc { namespace detail { namespace olecore {
HRESULT MakeDispatchVariant(VARTYPE vt, va_list* args, VARIANTARG* var);
} } }

namespace openmfc { namespace detail { namespace olecore {
HRESULT CopyDispatchResult(VARTYPE vt, void* pvRet, VARIANT* result);
} } }

namespace openmfc { namespace detail { namespace olecore {
void ResetOleVariant(COleVariant* pThis);
} } }

namespace openmfc { namespace detail { namespace olecore {
void AssignI2Variant(COleVariant* pThis, short value, VARTYPE vt);
} } }

namespace openmfc { namespace detail { namespace olecore {
void AssignI4Variant(COleVariant* pThis, long value, VARTYPE vt);
} } }

namespace openmfc { namespace detail { namespace olecore {
void SetOleVariantString(COleVariant* pThis, const wchar_t* text, VARTYPE vt);
} } }

namespace openmfc { namespace detail { namespace olecore {
const COleVariant* AssignOleVariant(COleVariant* pThis, const VARIANT* pSrc);
} } }

namespace openmfc { namespace detail { namespace olecore {
COleClientItem* FindDocumentItemByOleObject(const COleDocument* doc, IOleObject* object);
} } }

namespace openmfc { namespace detail { namespace olecore {
void CopyVariantByteArray(COleVariant* pThis, CByteArray* bytes);
} } }

namespace openmfc { namespace detail { namespace olecore {
struct OleDispatchDriverLayout {
    LPDISPATCH m_lpDispatch;
    BOOL m_bAutoRelease;
};
} } }

namespace openmfc { namespace detail { namespace olecore {
OleDispatchDriverLayout* DispatchDriverLayout(COleDispatchDriver* pThis);
} } }

namespace openmfc { namespace detail { namespace olecore {
const OleDispatchDriverLayout* DispatchDriverLayout(const COleDispatchDriver* pThis);
} } }

// Symbol: ??0COleVariant@@QEAA@AEBUtagVARIANT@@@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" COleVariant* MS_ABI impl___0COleVariant__QEAA_AEBUtagVARIANT___Z(
    COleVariant* pThis, const VARIANT* pSrc
);
} } }

// Symbol: ??0COleVariant@@QEAA@AEBV0@@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" COleVariant* MS_ABI impl___0COleVariant__QEAA_AEBV0__Z(
    COleVariant* pThis, const COleVariant* pSrc
);
} } }

// Symbol: ??0COleVariant@@QEAA@FG@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" COleVariant* MS_ABI impl___0COleVariant__QEAA_FG_Z(
    COleVariant* pThis, short value, VARTYPE vt
);
} } }

// Symbol: ??0COleVariant@@QEAA@JG@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" COleVariant* MS_ABI impl___0COleVariant__QEAA_JG_Z(
    COleVariant* pThis, long value, VARTYPE vt
);
} } }

// Symbol: ??0COleVariant@@QEAA@PEB_WG@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" COleVariant* MS_ABI impl___0COleVariant__QEAA_PEB_WG_Z(
    COleVariant* pThis, const wchar_t* text, VARTYPE vt
);
} } }

// Symbol: ??0COleVariant@@QEAA@PEBUtagVARIANT@@@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" COleVariant* MS_ABI impl___0COleVariant__QEAA_PEBUtagVARIANT___Z(
    COleVariant* pThis, const VARIANT* pSrc
);
} } }

// Symbol: ??4COleVariant@@QEAAAEBV0@AEBV0@@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_AEBV0__Z(
    COleVariant* pThis, const COleVariant* pSrc
);
} } }

// Symbol: ??4COleVariant@@QEAAAEBV0@AEBUtagVARIANT@@@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_AEBUtagVARIANT___Z(
    COleVariant* pThis, const VARIANT* pSrc
);
} } }

// Symbol: ??4COleVariant@@QEAAAEBV0@PEBUtagVARIANT@@@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_PEBUtagVARIANT___Z(
    COleVariant* pThis, const VARIANT* pSrc
);
} } }

// Symbol: ??4COleVariant@@QEAAAEBV0@F@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_F_Z(
    COleVariant* pThis, short value
);
} } }

// Symbol: ??4COleVariant@@QEAAAEBV0@J@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_J_Z(
    COleVariant* pThis, long value
);
} } }

// Symbol: ??4COleVariant@@QEAAAEBV0@M@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_M_Z(
    COleVariant* pThis, float value
);
} } }

// Symbol: ??4COleVariant@@QEAAAEBV0@N@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_N_Z(
    COleVariant* pThis, double value
);
} } }

// Symbol: ??4COleVariant@@QEAAAEBV0@QEB_W@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_QEB_W_Z(
    COleVariant* pThis, const wchar_t* text
);
} } }

// Symbol: ??4COleVariant@@QEAAAEBV0@_J@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0__J_Z(
    COleVariant* pThis, __int64 value
);
} } }

// Symbol: ??4COleVariant@@QEAAAEBV0@_K@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0__K_Z(
    COleVariant* pThis, unsigned __int64 value
);
} } }

// Symbol: ?SetString@COleVariant@@QEAAXPEB_WG@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" void MS_ABI impl__SetString_COleVariant__QEAAXPEB_WG_Z(
    COleVariant* pThis, const wchar_t* text, VARTYPE vt
);
} } }

// Symbol: ?Attach@COleVariant@@QEAAXAEAUtagVARIANT@@@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" void MS_ABI impl__Attach_COleVariant__QEAAXAEAUtagVARIANT___Z(
    COleVariant* pThis, VARIANT* pSrc
);
} } }

// Symbol: ?Detach@COleVariant@@QEAA?AUtagVARIANT@@XZ
namespace openmfc { namespace detail { namespace olecore {
extern "C" VARIANT* MS_ABI impl__Detach_COleVariant__QEAA_AUtagVARIANT__XZ(
    VARIANT* pRet, COleVariant* pThis
);
} } }

// Symbol: ?ChangeType@COleVariant@@QEAAXGPEAUtagVARIANT@@@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" void MS_ABI impl__ChangeType_COleVariant__QEAAXGPEAUtagVARIANT___Z(
    COleVariant* pThis, VARTYPE vtNew, VARIANT* pSrc
);
} } }

//----------------------------------------------------------------------------
// COleVariant – remaining operators, ctor, archive support
//----------------------------------------------------------------------------


// Minimal IStream on top of CArchive for IPersistStream Save/Load.
// Real MFC uses CArchiveStream; this provides the same contract.
namespace openmfc { namespace detail { namespace olecore {
struct CArchiveStream : IStream {
    CArchive* m_ar;
    LONG m_ref;

    explicit CArchiveStream(CArchive* ar) : m_ar(ar), m_ref(1) {}

    STDMETHOD(QueryInterface)(REFIID riid, void** ppv) {
        if (!ppv) return E_POINTER;
        if (riid == IID_IUnknown) { *ppv = static_cast<IUnknown*>(this); AddRef(); return S_OK; }
        *ppv = nullptr; return E_NOINTERFACE;
    }
    STDMETHOD_(ULONG, AddRef)() { return InterlockedIncrement(&m_ref); }
    STDMETHOD_(ULONG, Release)() {
        LONG r = InterlockedDecrement(&m_ref);
        if (r == 0) { delete this; return 0; }
        return r;
    }
    STDMETHOD(Read)(void* pv, ULONG cb, ULONG* pcbRead) {
        ULONG n = m_ar->Read(pv, cb);
        if (pcbRead) *pcbRead = n;
        return S_OK;
    }
    STDMETHOD(Write)(const void* pv, ULONG cb, ULONG* pcbWritten) {
        m_ar->Write(pv, cb);
        if (pcbWritten) *pcbWritten = cb;
        return S_OK;
    }
    STDMETHOD(Seek)(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER* plibNewPosition) {
        if (!m_ar || !m_ar->GetFile()) return STG_E_INVALIDHANDLE;
        DWORD origin = STREAM_SEEK_SET;
        if (dwOrigin == STREAM_SEEK_SET) {
            origin = CFile::begin;
        } else if (dwOrigin == STREAM_SEEK_CUR) {
            origin = CFile::current;
        } else if (dwOrigin == STREAM_SEEK_END) {
            origin = CFile::end;
        } else {
            return STG_E_INVALIDFUNCTION;
        }
        ULONGLONG pos = m_ar->GetFile()->Seek(dlibMove.QuadPart, origin);
        if (plibNewPosition) plibNewPosition->QuadPart = pos;
        return S_OK;
    }
    STDMETHOD(SetSize)(ULARGE_INTEGER libNewSize) {
        if (!m_ar || !m_ar->GetFile()) return STG_E_INVALIDHANDLE;
        m_ar->GetFile()->SetLength(libNewSize.QuadPart);
        return S_OK;
    }
    STDMETHOD(CopyTo)(IStream* pstm, ULARGE_INTEGER cb, ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten) {
        if (!pstm) return STG_E_INVALIDPOINTER;
        BYTE buffer[4096];
        ULONGLONG remaining = cb.QuadPart;
        ULONGLONG totalRead = 0, totalWritten = 0;
        while (remaining > 0) {
            ULONG toRead = static_cast<ULONG>(std::min<ULONGLONG>(remaining, sizeof(buffer)));
            ULONG readNow = 0;
            ULONG wroteNow = 0;
            HRESULT hr = Read(buffer, toRead, &readNow);
            if (FAILED(hr)) return hr;
            if (readNow == 0) break;
            totalRead += readNow;
            hr = pstm->Write(buffer, readNow, &wroteNow);
            if (FAILED(hr)) return hr;
            totalWritten += wroteNow;
            if (wroteNow != readNow) return STG_E_WRITEFAULT;
            remaining -= readNow;
        }
        if (pcbRead) pcbRead->QuadPart = totalRead;
        if (pcbWritten) pcbWritten->QuadPart = totalWritten;
        return S_OK;
    }
    STDMETHOD(Commit)(DWORD) { return S_OK; }
    STDMETHOD(Revert)() { return STG_E_INVALIDFUNCTION; }
    STDMETHOD(LockRegion)(ULARGE_INTEGER, ULARGE_INTEGER, DWORD) { return STG_E_INVALIDFUNCTION; }
    STDMETHOD(UnlockRegion)(ULARGE_INTEGER, ULARGE_INTEGER, DWORD) { return STG_E_INVALIDFUNCTION; }
    STDMETHOD(Stat)(STATSTG* pstatstg, DWORD grfStatFlag) {
        if (!pstatstg) return STG_E_INVALIDPOINTER;
        if (!m_ar || !m_ar->GetFile()) return STG_E_INVALIDHANDLE;
        ZeroMemory(pstatstg, sizeof(*pstatstg));
        pstatstg->type = STGTY_STREAM;
        pstatstg->cbSize.QuadPart = m_ar->GetFile()->GetLength();
        pstatstg->grfMode = 0;
        pstatstg->grfLocksSupported = 0;
        pstatstg->grfStateBits = 0;
        if ((grfStatFlag & STATFLAG_NONAME) == 0) {
            pstatstg->pwcsName = nullptr;
        }
        return S_OK;
    }
    STDMETHOD(Clone)(IStream** ppstm) {
        if (!ppstm) return STG_E_INVALIDPOINTER;
        AddRef();
        *ppstm = this;
        return S_OK;
    }
};
} } }

// _AfxCompareSafeArrays_Olecore — adapted from MFC olevar.cpp.
namespace openmfc { namespace detail { namespace olecore {
BOOL _AfxCompareSafeArrays_Olecore(SAFEARRAY* parray1, SAFEARRAY* parray2);
} } }

// _AfxCreateOneDimArray — from MFC olevar.cpp.
namespace openmfc { namespace detail { namespace olecore {
void _AfxCreateOneDimArray(COleVariant* pThis, DWORD dwSize);
} } }

// _AfxCopyBinaryData — from MFC olevar.cpp.
namespace openmfc { namespace detail { namespace olecore {
void _AfxCopyBinaryData(COleVariant* pThis, const void* pvSrc, DWORD dwSize);
} } }


// Symbol: ??0COleVariant@@QEAA@PEFBU_ITEMIDLIST@@@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" COleVariant* MS_ABI impl___0COleVariant__QEAA_PEFBU_ITEMIDLIST___Z(
    COleVariant* pThis, const ITEMIDLIST* pidl);
} } }

// Symbol: ??4COleVariant@@QEAAAEBV0@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    COleVariant* pThis, const CString* strSrc);
} } }

// Symbol: ??4COleVariant@@QEAAAEBV0@AEBVCByteArray@@@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_AEBVCByteArray___Z(
    COleVariant* pThis, const CByteArray* arrSrc);
} } }

// Symbol: ??4COleVariant@@QEAAAEBV0@AEBVCLongBinary@@@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_AEBVCLongBinary___Z(
    COleVariant* pThis, const CLongBinary* lbSrc);
} } }

// Symbol: ??4COleVariant@@QEAAAEBV0@AEBVCOleCurrency@@@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_AEBVCOleCurrency___Z(
    COleVariant* pThis, const COleCurrency* curSrc);
} } }

// Symbol: ??4COleVariant@@QEAAAEBV0@AEBVCOleDateTime@ATL@@@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_AEBVCOleDateTime_ATL___Z(
    COleVariant* pThis, const COleDateTime* dateSrc);
} } }

// Symbol: ??4COleVariant@@QEAAAEBV0@E@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_E_Z(
    COleVariant* pThis, unsigned char nSrc);
} } }

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAVCOleVariant@@@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" CArchive* MS_ABI impl___5_YAAEAVCArchive__AEAV0_AEAVCOleVariant___Z(
    CArchive* ar, COleVariant* varSrc);
} } }

// Symbol: ??6@YAAEAVCArchive@@AEAV0@VCOleVariant@@@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" CArchive* MS_ABI impl___6_YAAEAVCArchive__AEAV0_VCOleVariant___Z(
    CArchive* ar, const VARIANT* varSrc);
} } }

// Symbol: ??8COleVariant@@QEBAHAEBUtagVARIANT@@@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" int MS_ABI impl___8COleVariant__QEBAHAEBUtagVARIANT___Z(
    const COleVariant* pThis, const VARIANT* var);
} } }

// Symbol: ??0COleDispatchDriver@@QEAA@XZ
namespace openmfc { namespace detail { namespace olecore {
extern "C" COleDispatchDriver* MS_ABI impl___0COleDispatchDriver__QEAA_XZ(
    COleDispatchDriver* pThis
);
} } }

// Symbol: ??0COleDispatchDriver@@QEAA@PEAUIDispatch@@H@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" COleDispatchDriver* MS_ABI impl___0COleDispatchDriver__QEAA_PEAUIDispatch__H_Z(
    COleDispatchDriver* pThis, LPDISPATCH lpDispatch, BOOL bAutoRelease
);
} } }

// Symbol: ??0COleDispatchDriver@@QEAA@AEBV0@@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" COleDispatchDriver* MS_ABI impl___0COleDispatchDriver__QEAA_AEBV0__Z(
    COleDispatchDriver* pThis, const COleDispatchDriver* pSrc
);
} } }

// Symbol: ??4COleDispatchDriver@@QEAAAEBV0@AEBV0@@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" const COleDispatchDriver* MS_ABI impl___4COleDispatchDriver__QEAAAEBV0_AEBV0__Z(
    COleDispatchDriver* pThis, const COleDispatchDriver* pSrc
);
} } }

// Symbol: ?AttachDispatch@COleDispatchDriver@@QEAAXPEAUIDispatch@@H@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" void MS_ABI impl__AttachDispatch_COleDispatchDriver__QEAAXPEAUIDispatch__H_Z(
    COleDispatchDriver* pThis, LPDISPATCH lpDispatch, BOOL bAutoRelease
);
} } }

// Symbol: ?DetachDispatch@COleDispatchDriver@@QEAAPEAUIDispatch@@XZ
namespace openmfc { namespace detail { namespace olecore {
extern "C" LPDISPATCH MS_ABI impl__DetachDispatch_COleDispatchDriver__QEAAPEAUIDispatch__XZ(
    COleDispatchDriver* pThis
);
} } }

// Symbol: ?ReleaseDispatch@COleDispatchDriver@@QEAAXXZ
namespace openmfc { namespace detail { namespace olecore {
extern "C" void MS_ABI impl__ReleaseDispatch_COleDispatchDriver__QEAAXXZ(
    COleDispatchDriver* pThis
);
} } }

// Symbol: ?CreateDispatch@COleDispatchDriver@@QEAAHAEBU_GUID@@PEAVCOleException@@@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" int MS_ABI impl__CreateDispatch_COleDispatchDriver__QEAAHAEBU_GUID__PEAVCOleException___Z(
    COleDispatchDriver* pThis, REFCLSID clsid, COleException* pError
);
} } }

// Symbol: ?CreateDispatch@COleDispatchDriver@@QEAAHPEB_WPEAVCOleException@@@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" int MS_ABI impl__CreateDispatch_COleDispatchDriver__QEAAHPEB_WPEAVCOleException___Z(
    COleDispatchDriver* pThis, const wchar_t* progId, COleException* pError
);
} } }

// Symbol: ?InvokeHelper@COleDispatchDriver@@QEAAXJGGPEAXPEBEZZ
namespace openmfc { namespace detail { namespace olecore {
extern "C" void MS_ABI impl__InvokeHelper_COleDispatchDriver__QEAAXJGGPEAXPEBEZZ(
    COleDispatchDriver* pThis, DISPID dwDispID, WORD wFlags, VARTYPE vtRet,
    void* pvRet, const BYTE* pbParamInfo, ...
);
} } }

// Symbol: ?InvokeHelperV@COleDispatchDriver@@QEAAXJGGPEAXPEBEPEAD@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" void MS_ABI impl__InvokeHelperV_COleDispatchDriver__QEAAXJGGPEAXPEBEPEAD_Z(
    COleDispatchDriver* pThis, DISPID dwDispID, WORD wFlags, VARTYPE vtRet,
    void* pvRet, const BYTE* pbParamInfo, va_list args
);
} } }

// Symbol: ?SetProperty@COleDispatchDriver@@QEAAXJGZZ
namespace openmfc { namespace detail { namespace olecore {
extern "C" void MS_ABI impl__SetProperty_COleDispatchDriver__QEAAXJGZZ(
    COleDispatchDriver* pThis, DISPID dwDispID, VARTYPE vtProp, ...
);
} } }

// Symbol: ?GetProperty@COleDispatchDriver@@QEBAXJGPEAX@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" void MS_ABI impl__GetProperty_COleDispatchDriver__QEBAXJGPEAX_Z(
    const COleDispatchDriver* pThis, DISPID dwDispID, VARTYPE vtProp, void* pvProp
);
} } }

// Symbol: ?QueryInterface@COleConnPtContainer@@UEAAJAEBU_GUID@@PEAPEAX@Z
extern "C" ULONG MS_ABI impl__AddRef_COleConnPtContainer__UEAAKXZ(COleConnPtContainer* pThis);
namespace openmfc { namespace detail { namespace olecore {
extern "C" HRESULT MS_ABI impl__QueryInterface_COleConnPtContainer__UEAAJAEBU_GUID__PEAPEAX_Z(
    COleConnPtContainer* pThis, REFIID riid, void** ppv
);
} } }

// Symbol: ?AddRef@COleConnPtContainer@@UEAAKXZ
extern "C" ULONG MS_ABI impl__AddRef_COleConnPtContainer__UEAAKXZ(
    COleConnPtContainer* pThis
);

// Symbol: ?Release@COleConnPtContainer@@UEAAKXZ
namespace openmfc { namespace detail { namespace olecore {
extern "C" ULONG MS_ABI impl__Release_COleConnPtContainer__UEAAKXZ(
    COleConnPtContainer* pThis
);
} } }

// Symbol: ?EnumConnectionPoints@COleConnPtContainer@@UEAAJPEAPEAUIEnumConnectionPoints@@@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" HRESULT MS_ABI impl__EnumConnectionPoints_COleConnPtContainer__UEAAJPEAPEAUIEnumConnectionPoints___Z(
    COleConnPtContainer* pThis, IEnumConnectionPoints** ppEnum
);
} } }

// Symbol: ?FindConnectionPoint@COleConnPtContainer@@UEAAJAEBU_GUID@@PEAPEAUIConnectionPoint@@@Z
namespace openmfc { namespace detail { namespace olecore {
extern "C" HRESULT MS_ABI impl__FindConnectionPoint_COleConnPtContainer__UEAAJAEBU_GUID__PEAPEAUIConnectionPoint___Z(
    COleConnPtContainer* pThis, REFIID riid, IConnectionPoint** ppCP
);
} } }

namespace openmfc { namespace detail { namespace olecore {
struct DataCacheState;
} } }

namespace openmfc { namespace detail { namespace olecore {
class DataSourceDataObject : public IDataObject {
public:
    explicit DataSourceDataObject(DataCacheState* state) : m_refCount(1), m_state(state) {}

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override;
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&m_refCount); }
    ULONG STDMETHODCALLTYPE Release() override {
        ULONG ref = InterlockedDecrement(&m_refCount);
        return ref;
    }

    HRESULT STDMETHODCALLTYPE GetData(FORMATETC* pformatetcIn, STGMEDIUM* pmedium) override;
    HRESULT STDMETHODCALLTYPE GetDataHere(FORMATETC*, STGMEDIUM*) override { return DATA_E_FORMATETC; }
    HRESULT STDMETHODCALLTYPE QueryGetData(FORMATETC* pformatetc) override;
    HRESULT STDMETHODCALLTYPE GetCanonicalFormatEtc(FORMATETC*, FORMATETC* pformatetcOut) override {
        if (!pformatetcOut) return E_POINTER;
        pformatetcOut->ptd = nullptr;
        return DATA_S_SAMEFORMATETC;
    }
    HRESULT STDMETHODCALLTYPE SetData(FORMATETC* pformatetc, STGMEDIUM* pmedium, BOOL fRelease) override;
    HRESULT STDMETHODCALLTYPE EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC** ppenumFormatEtc) override;
    HRESULT STDMETHODCALLTYPE DAdvise(FORMATETC*, DWORD, IAdviseSink*, DWORD*) override { return OLE_E_ADVISENOTSUPPORTED; }
    HRESULT STDMETHODCALLTYPE DUnadvise(DWORD) override { return OLE_E_ADVISENOTSUPPORTED; }
    HRESULT STDMETHODCALLTYPE EnumDAdvise(IEnumSTATDATA**) override { return OLE_E_ADVISENOTSUPPORTED; }

private:
    LONG m_refCount;
    DataCacheState* m_state;
};
} } }

namespace openmfc { namespace detail { namespace olecore {
struct DataCacheState {
    explicit DataCacheState(COleDataSource* source) : owner(source), dataObject(this) {}
    ~DataCacheState() {
        for (DataCacheEntry& entry : entries) {
            if (entry.hasMedium) ReleaseStgMedium(&entry.medium);
        }
    }

    COleDataSource* owner;
    DataSourceDataObject dataObject;
    std::vector<DataCacheEntry> entries;
};
} } }

namespace openmfc { namespace detail { namespace olecore {
class DropTargetAdapter : public IDropTarget {
public:
    explicit DropTargetAdapter(COleDropTarget* target) : m_refCount(1), m_target(target) {}
    virtual ~DropTargetAdapter() {}

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override {
        if (!ppvObject) return E_POINTER;
        if (riid == IID_IUnknown || riid == IID_IDropTarget) {
            *ppvObject = static_cast<IDropTarget*>(this);
            AddRef();
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }

    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&m_refCount); }

    ULONG STDMETHODCALLTYPE Release() override {
        ULONG ref = InterlockedDecrement(&m_refCount);
        if (ref == 0) delete this;
        return ref;
    }

    HRESULT STDMETHODCALLTYPE DragEnter(IDataObject* dataObject, DWORD keyState, POINTL point, DWORD* effect) override;
    HRESULT STDMETHODCALLTYPE DragOver(DWORD keyState, POINTL point, DWORD* effect) override;
    HRESULT STDMETHODCALLTYPE DragLeave() override;
    HRESULT STDMETHODCALLTYPE Drop(IDataObject* dataObject, DWORD keyState, POINTL point, DWORD* effect) override;

private:
    CPoint MakePoint(POINTL point) const { return CPoint(static_cast<int>(point.x), static_cast<int>(point.y)); }

    LONG m_refCount;
    COleDropTarget* m_target;
    COleDataObject m_dataObject;
};
} } }

namespace openmfc { namespace detail { namespace olecore {
struct DropTargetState {
    COleDropTarget* target = nullptr;
    DropTargetAdapter* adapter = nullptr;
};
} } }

namespace openmfc { namespace detail { namespace olecore {
struct DocumentState {
    COleDocument* document = nullptr;
    std::vector<COleClientItem*> items;
};
} } }

namespace openmfc { namespace detail { namespace olecore {
struct ClientItemState {
    COleClientItem* item = nullptr;
    LONG activeVerb = OLEIVERB_PRIMARY;
    BOOL modified = FALSE;
    CString hostName;
    CString hostObjectName;
    IUnknown* attachedDataObject = nullptr;
    HGLOBAL iconicMetafile = nullptr;
    HGLOBAL contentMetafile = nullptr;
    ClientItemState() = default;
    ClientItemState(const ClientItemState&) = delete;
    ClientItemState& operator=(const ClientItemState&) = delete;
    ClientItemState(ClientItemState&& other) noexcept
        : item(other.item), activeVerb(other.activeVerb), modified(other.modified),
          hostName(std::move(other.hostName)),
          hostObjectName(std::move(other.hostObjectName)),
          attachedDataObject(other.attachedDataObject),
          iconicMetafile(other.iconicMetafile), contentMetafile(other.contentMetafile) {
        other.item = nullptr;
        other.attachedDataObject = nullptr;
        other.iconicMetafile = nullptr;
        other.contentMetafile = nullptr;
        other.hostName.Empty();
        other.hostObjectName.Empty();
    }
    ClientItemState& operator=(ClientItemState&& other) noexcept {
        if (this != &other) {
            if (attachedDataObject) attachedDataObject->Release();
            if (iconicMetafile) GlobalFree(iconicMetafile);
            if (contentMetafile) GlobalFree(contentMetafile);
            item = other.item;
            activeVerb = other.activeVerb;
            modified = other.modified;
            hostName = std::move(other.hostName);
            hostObjectName = std::move(other.hostObjectName);
            attachedDataObject = other.attachedDataObject;
            iconicMetafile = other.iconicMetafile;
            contentMetafile = other.contentMetafile;
            other.item = nullptr;
            other.attachedDataObject = nullptr;
            other.hostName.Empty();
            other.hostObjectName.Empty();
            other.iconicMetafile = nullptr;
            other.contentMetafile = nullptr;
        }
        return *this;
    }
    ~ClientItemState() {
        if (attachedDataObject) attachedDataObject->Release();
        if (iconicMetafile) GlobalFree(iconicMetafile);
        if (contentMetafile) GlobalFree(contentMetafile);
    }
};
} } }

namespace openmfc { namespace detail { namespace olecore {
struct ServerDocState {
    COleServerDoc* document = nullptr;
    std::vector<COleServerItem*> items;
    CString hostName;
    CString hostObjectName;
};
} } }

namespace openmfc { namespace detail { namespace olecore {
struct ServerItemState {
    COleServerItem* item = nullptr;
    COleDataSource* dataSource = nullptr;
    SIZE contentExtent = {0, 0};
    BOOL hasExtent = FALSE;
    ~ServerItemState() { delete dataSource; }
};
} } }

namespace openmfc { namespace detail { namespace olecore {
struct OleControlEventSink {
    IID iid = IID_NULL;
    DWORD cookie = 0;
    IUnknown* sink = nullptr;
};
} } }

namespace openmfc { namespace detail { namespace olecore {
struct OleControlDataSourceCacheEntry {
    FORMATETC format = {};
    STGMEDIUM medium = {};
    bool hasMedium = false;
};
} } }

namespace openmfc { namespace detail { namespace olecore {
struct OleControlState {
    COleControl* control = nullptr;
    // OpenMFC-only associations. Retail COleControl has no member for either:
    // it reaches its container through m_pClientSite, and its m_pControlSite is
    // an IOleControlSite*, not OpenMFC's COleControlSite. Keeping them here
    // leaves the 912-byte retail member layout intact.
    COleControlContainer* container = nullptr;
    COleControlSite* mfcSite = nullptr;
    COLORREF backColor = RGB(255, 255, 255);
    COLORREF foreColor = RGB(0, 0, 0);
    BOOL enabled = TRUE;
    short appearance = 0;
    short borderStyle = 0;
    CString licenseKey;
    CString text;
    long readyState = 4;
    std::vector<OleControlEventSink> eventSinks;
    std::vector<IPropertyNotifySink*> propSinks;
    std::vector<IID> enabledConnectionPoints;
    RECT posRect = {0, 0, 0, 0};
    RECT clipRect = {0, 0, 0, 0};
    BOOL hasObjectRects = FALSE;
    std::vector<OleControlDataSourceCacheEntry> dataSourceEntries;
};
} } }

namespace openmfc { namespace detail { namespace olecore {
extern std::vector<DropTargetState> g_dropTargetStates;
} } }
namespace openmfc { namespace detail { namespace olecore {
extern std::vector<DocumentState> g_documentStates;
} } }
namespace openmfc { namespace detail { namespace olecore {
extern std::vector<ClientItemState> g_clientItemStates;
} } }
namespace openmfc { namespace detail { namespace olecore {
extern std::vector<ServerDocState> g_serverDocStates;
} } }
namespace openmfc { namespace detail { namespace olecore {
extern std::vector<ServerItemState> g_serverItemStates;
} } }
namespace openmfc { namespace detail { namespace olecore {
extern std::vector<OleControlState> g_oleControlStates;
} } }
namespace openmfc { namespace detail { namespace olecore {
extern std::vector<COleObjectFactory*> g_oleObjectFactories;
} } }

namespace openmfc { namespace detail { namespace olecore {
CString ClsidKey(REFCLSID clsid);
} } }

namespace openmfc { namespace detail { namespace olecore {
CString FindFactoryLicense(REFCLSID clsid, const CString& fallbackProgId = CString());
} } }

namespace openmfc { namespace detail { namespace olecore {
BSTR AllocateLicenseBstr(const CString& value);
} } }

namespace openmfc { namespace detail { namespace olecore {
DropTargetState* GetDropTargetState(COleDropTarget* target, bool create);
} } }

namespace openmfc { namespace detail { namespace olecore {
void RemoveDropTargetState(COleDropTarget* target);
} } }

namespace openmfc { namespace detail { namespace olecore {
DocumentState* GetDocumentState(COleDocument* document, bool create);
} } }

namespace openmfc { namespace detail { namespace olecore {
void RemoveDocumentState(COleDocument* document);
} } }

namespace openmfc { namespace detail { namespace olecore {
class OleItemContainerAdapter : public IOleItemContainer {
public:
    explicit OleItemContainerAdapter(COleDocument* document) : m_refCount(1), m_document(document) {}

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override {
        if (!ppvObject) return E_POINTER;
        if (riid == IID_IUnknown || riid == IID_IOleContainer || riid == IID_IOleItemContainer) {
            *ppvObject = static_cast<IOleItemContainer*>(this);
            AddRef();
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&m_refCount); }
    ULONG STDMETHODCALLTYPE Release() override {
        ULONG ref = InterlockedDecrement(&m_refCount);
        if (ref == 0) delete this;
        return ref;
    }
    HRESULT STDMETHODCALLTYPE ParseDisplayName(IBindCtx*, LPOLESTR, ULONG* pchEaten, IMoniker** ppmkOut) override {
        if (pchEaten) *pchEaten = 0;
        if (ppmkOut) *ppmkOut = nullptr;
        return E_FAIL;
    }
    HRESULT STDMETHODCALLTYPE EnumObjects(DWORD, IEnumUnknown** ppenum) override {
        if (ppenum) *ppenum = nullptr;
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE LockContainer(BOOL) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE GetObject(LPOLESTR pszItem, DWORD, IBindCtx*, REFIID riid, void** ppvObject) override {
        if (!ppvObject) return E_POINTER;
        *ppvObject = nullptr;
        COleClientItem* item = m_document ? m_document->OnGetLinkedItem(pszItem) : nullptr;
        if (!item || !item->GetObject()) return MK_E_NOOBJECT;
        return item->GetObject()->QueryInterface(riid, ppvObject);
    }
    HRESULT STDMETHODCALLTYPE GetObjectStorage(LPOLESTR, IBindCtx*, REFIID, void** ppvStorage) override {
        if (!ppvStorage) return E_POINTER;
        *ppvStorage = nullptr;
        return E_FAIL;
    }
    HRESULT STDMETHODCALLTYPE IsRunning(LPOLESTR pszItem) override {
        COleClientItem* item = m_document ? m_document->OnGetLinkedItem(pszItem) : nullptr;
        return (item && item->IsRunning()) ? S_OK : S_FALSE;
    }

private:
    LONG m_refCount;
    COleDocument* m_document;
};
} } }

namespace openmfc { namespace detail { namespace olecore {
ClientItemState* GetClientItemState(COleClientItem* item, bool create);
} } }

namespace openmfc { namespace detail { namespace olecore {
ClientItemState* FindClientItemState(const COleClientItem* item);
} } }

namespace openmfc { namespace detail { namespace olecore {
void RemoveClientItemState(COleClientItem* item);
} } }

namespace openmfc { namespace detail { namespace olecore {
void AddOleObjectFactory(COleObjectFactory* factory);
} } }

namespace openmfc { namespace detail { namespace olecore {
void RemoveOleObjectFactory(COleObjectFactory* factory);
} } }

namespace openmfc { namespace detail { namespace olecore {
ServerDocState* GetServerDocState(COleServerDoc* document, bool create);
} } }

namespace openmfc { namespace detail { namespace olecore {
void RemoveServerDocState(COleServerDoc* document);
} } }

namespace openmfc { namespace detail { namespace olecore {
ServerItemState* GetServerItemState(COleServerItem* item, bool create);
} } }

namespace openmfc { namespace detail { namespace olecore {
void RemoveServerItemState(COleServerItem* item);
} } }

namespace openmfc { namespace detail { namespace olecore {
BOOL EnsureLinkingDocMoniker(COleLinkingDoc* document, const wchar_t* fileName, BOOL setModified);
} } }

namespace openmfc { namespace detail { namespace olecore {
OleControlState* GetOleControlState(COleControl* control, bool create);
} } }

namespace openmfc { namespace detail { namespace olecore {
COleControl* GetControlFromDataSource(const COleControl::CControlDataSource* pSource);
} } }

namespace openmfc { namespace detail { namespace olecore {
OleControlDataSourceCacheEntry* FindControlDataSourceEntry(OleControlState* state,
                                                                const FORMATETC& format,
                                                                bool create);
} } }

namespace openmfc { namespace detail { namespace olecore {
OleControlDataSourceCacheEntry* FindControlDataSourceEntry(COleControl* control,
                                                                const FORMATETC& format,
                                                                bool create);
} } }

namespace openmfc { namespace detail { namespace olecore {
void ReleaseDataSourceEntryMedium(OleControlDataSourceCacheEntry& entry);
} } }

// OpenMFC's COleControlSite for a control, or null. Retail keeps no such
// pointer in the object (its m_pControlSite is an IOleControlSite*), so the
// association lives in the side table rather than in a member.
namespace openmfc { namespace detail { namespace olecore {
COleControlSite* MfcSiteOf(const COleControl* control);
} } }

namespace openmfc { namespace detail { namespace olecore {
template <typename Interface>
static Interface* QueryOleControlInterface(COleControl* control, REFIID iid) {
    if (!control) return nullptr;
    Interface* iface = nullptr;
    OleControlState* state = GetOleControlState(control, false);
    if (state && state->mfcSite && state->mfcSite->m_lpObject &&
        SUCCEEDED(state->mfcSite->m_lpObject->QueryInterface(iid, reinterpret_cast<void**>(&iface)))) {
        return iface;
    }
    if (SUCCEEDED(control->InternalQueryInterface(iid, reinterpret_cast<void**>(&iface)))) {
        return iface;
    }
    return nullptr;
}
} } }

namespace openmfc { namespace detail { namespace olecore {
void RemoveOleControlState(COleControl* control);
} } }

namespace openmfc { namespace detail { namespace olecore {
void AddDocumentItem(COleDocument* document, COleClientItem* item);
} } }

namespace openmfc { namespace detail { namespace olecore {
void RemoveDocumentItem(COleDocument* document, COleClientItem* item);
} } }

namespace openmfc { namespace detail { namespace olecore {
void AddServerDocItem(COleServerDoc* document, COleServerItem* item);
} } }

namespace openmfc { namespace detail { namespace olecore {
void RemoveServerDocItem(COleServerDoc* document, COleServerItem* item);
} } }

namespace openmfc { namespace detail { namespace olecore {
size_t ParseLinkedItemIndex(const wchar_t* itemName);
} } }

namespace openmfc { namespace detail { namespace olecore {
DataCacheState* GetDataCacheState(COleDataSource* source, bool create);
} } }

namespace openmfc { namespace detail { namespace olecore {
DataCacheEntry* FindCacheEntry(DataCacheState* state, const FORMATETC& format);
} } }






namespace openmfc { namespace detail { namespace olecore {
class DropSourceAdapter : public IDropSource {
public:
    explicit DropSourceAdapter(COleDropSource* source) : m_refCount(1), m_source(source) {}
    virtual ~DropSourceAdapter() {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override {
        if (!ppvObject) return E_POINTER;
        if (riid == IID_IUnknown || riid == IID_IDropSource) {
            *ppvObject = static_cast<IDropSource*>(this);
            AddRef();
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&m_refCount); }
    ULONG STDMETHODCALLTYPE Release() override {
        ULONG ref = InterlockedDecrement(&m_refCount);
        if (ref == 0) delete this;
        return ref;
    }
    HRESULT STDMETHODCALLTYPE QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState) override {
        return m_source ? m_source->QueryContinueDrag(fEscapePressed, grfKeyState) : DRAGDROP_S_CANCEL;
    }
    HRESULT STDMETHODCALLTYPE GiveFeedback(DWORD dwEffect) override {
        return m_source ? m_source->GiveFeedback(dwEffect) : DRAGDROP_S_USEDEFAULTCURSORS;
    }

private:
    LONG m_refCount;
    COleDropSource* m_source;
};
} } }

namespace openmfc { namespace detail { namespace olecore {
class OleMessageFilterAdapter : public IMessageFilter {
public:
    explicit OleMessageFilterAdapter(COleMessageFilter* filter) : m_refCount(1), m_filter(filter) {}

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override {
        if (!ppvObject) return E_POINTER;
        if (riid == IID_IUnknown || riid == IID_IMessageFilter) {
            *ppvObject = static_cast<IMessageFilter*>(this);
            AddRef();
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&m_refCount); }
    ULONG STDMETHODCALLTYPE Release() override {
        ULONG ref = InterlockedDecrement(&m_refCount);
        if (ref == 0) delete this;
        return ref;
    }
    DWORD STDMETHODCALLTYPE HandleInComingCall(DWORD, HTASK, DWORD, LPINTERFACEINFO) override {
        return SERVERCALL_ISHANDLED;
    }
    DWORD STDMETHODCALLTYPE RetryRejectedCall(HTASK, DWORD, DWORD rejectType) override {
        if (!m_filter) return static_cast<DWORD>(-1);
        if (rejectType == SERVERCALL_RETRYLATER) {
            if (m_filter->m_nRetryReply != 0) return m_filter->m_nRetryReply;
            return m_filter->m_bEnableBusy ? 250 : static_cast<DWORD>(-1);
        }
        return static_cast<DWORD>(-1);
    }
    DWORD STDMETHODCALLTYPE MessagePending(HTASK, DWORD, DWORD) override {
        if (!m_filter) return PENDINGMSG_WAITDEFPROCESS;
        MSG msg = {};
        BOOL hasMsg = ::PeekMessageW(&msg, nullptr, 0, 0, PM_NOREMOVE);
        return m_filter->OnMessagePending(hasMsg ? &msg : nullptr)
            ? PENDINGMSG_WAITDEFPROCESS
            : PENDINGMSG_CANCELCALL;
    }

private:
    LONG m_refCount;
    COleMessageFilter* m_filter;
};
} } }

namespace openmfc { namespace detail { namespace olecore {
extern thread_local OleMessageFilterAdapter* g_messageFilterAdapter;
} } }
namespace openmfc { namespace detail { namespace olecore {
extern thread_local IMessageFilter* g_previousMessageFilter;
} } }






//=============================================================================
// OLE Initialization / Termination
//=============================================================================
extern "C" {


MS_ABI void impl__AfxOleInit();

MS_ABI void impl__AfxOleTerm(int bExplicit);

MS_ABI void impl__AfxOleLockApp();

MS_ABI void impl__AfxOleUnlockApp();

MS_ABI int impl__AfxOleCanExitApp();

MS_ABI int impl__AfxOleGetMessageFilter();

}

//=============================================================================
// COleCurrency
//=============================================================================





//=============================================================================
// COleDateTime
//=============================================================================


//=============================================================================
// COleSafeArray
//=============================================================================
// COleSafeArray : public tagVARIANT — the SAFEARRAY lives in `parray`, not
// inline. These operate on parray and keep vt / the cached dims in sync.








//=============================================================================
// COleDataObject
//=============================================================================














//=============================================================================
// COleDataSource
//=============================================================================














//=============================================================================
// COleDropTarget
//=============================================================================











//=============================================================================
// COleDropTarget::XDropTarget - nested COM IDropTarget
//=============================================================================


namespace openmfc { namespace detail { namespace olecore {
STDMETHODIMP_(ULONG) COleDropTarget::XDropTarget::AddRef();
} } }

namespace openmfc { namespace detail { namespace olecore {
STDMETHODIMP_(ULONG) COleDropTarget::XDropTarget::Release();
} } }












//=============================================================================
// COleDropSource
//=============================================================================






//=============================================================================
// COleDropSource::XDropSource - nested COM IDropSource
//=============================================================================


namespace openmfc { namespace detail { namespace olecore {
STDMETHODIMP_(ULONG) COleDropSource::XDropSource::AddRef();
} } }

namespace openmfc { namespace detail { namespace olecore {
STDMETHODIMP_(ULONG) COleDropSource::XDropSource::Release();
} } }








//=============================================================================
// COleMessageFilter
//=============================================================================

















//=============================================================================
// COleMessageFilter::XMessageFilter - nested COM IMessageFilter
//=============================================================================


namespace openmfc { namespace detail { namespace olecore {
STDMETHODIMP_(ULONG) COleMessageFilter::XMessageFilter::AddRef();
} } }

namespace openmfc { namespace detail { namespace olecore {
STDMETHODIMP_(ULONG) COleMessageFilter::XMessageFilter::Release();
} } }

namespace openmfc { namespace detail { namespace olecore {
STDMETHODIMP_(ULONG) COleMessageFilter::XMessageFilter::HandleInComingCall(
    DWORD dwCallType, HTASK htaskCaller, DWORD dwTickCount, LPINTERFACEINFO lpInterfaceInfo);
} } }

namespace openmfc { namespace detail { namespace olecore {
STDMETHODIMP_(ULONG) COleMessageFilter::XMessageFilter::RetryRejectedCall(
    HTASK htaskCallee, DWORD dwTickCount, DWORD dwRejectType);
} } }

namespace openmfc { namespace detail { namespace olecore {
STDMETHODIMP_(ULONG) COleMessageFilter::XMessageFilter::MessagePending(
    HTASK htaskCallee, DWORD dwTickCount, DWORD dwType);
} } }






//=============================================================================
// COleCmdUI
//=============================================================================






//=============================================================================
// COleFrameHook
//=============================================================================







//=============================================================================
// COleResizeBar
//=============================================================================




//=============================================================================
// COleStreamFile
//=============================================================================













//=============================================================================
// COlePropertyPage
//=============================================================================











//=============================================================================
// COleDocument
//=============================================================================



















//=============================================================================
// COleLinkingDoc
//=============================================================================









//=============================================================================
// COleServerDoc
//=============================================================================












//=============================================================================
// COleClientItem
//=============================================================================




























//=============================================================================
// COleServerItem
//=============================================================================













//=============================================================================
// COleObjectFactory
//=============================================================================
























//=============================================================================
// COleObjectFactory::XClassFactory - nested COM IClassFactory2
//=============================================================================



namespace openmfc { namespace detail { namespace olecore {
STDMETHODIMP_(ULONG) COleObjectFactory::XClassFactory::AddRef();
} } }

namespace openmfc { namespace detail { namespace olecore {
STDMETHODIMP_(ULONG) COleObjectFactory::XClassFactory::Release();
} } }






// Export wrappers for XClassFactory methods.






//=============================================================================
// COleTemplateServer
//=============================================================================









//=============================================================================
// COleControlSite
//=============================================================================

namespace openmfc { namespace detail { namespace olecore {
HWND GetSiteParentWindow(const COleControlSite* pSite);
} } }

namespace openmfc { namespace detail { namespace olecore {
RECT GetSitePositionRect(const COleControlSite* pSite);
} } }

namespace openmfc { namespace detail { namespace olecore {
void SetVariantBool(VARIANT* pVar, BOOL value);
} } }

namespace openmfc { namespace detail { namespace olecore {
BOOL TryGetAmbientOverride(COleControlSite* pSite, DISPID dispid, VARIANT* pVarResult);
} } }

namespace openmfc { namespace detail { namespace olecore {
class ControlSiteAdapter : public IOleClientSite,
                           public IOleInPlaceSite,
                           public IOleControlSite,
                           public IOleInPlaceFrame,
                           public IDispatch {
public:
    explicit ControlSiteAdapter(COleControlSite* pSite) : m_refs(1), m_site(pSite) {}
    virtual ~ControlSiteAdapter() = default;

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override {
        if (!ppvObject) {
            return E_POINTER;
        }
        *ppvObject = nullptr;
        if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IOleClientSite)) {
            *ppvObject = static_cast<IOleClientSite*>(this);
        } else if (IsEqualIID(riid, IID_IOleWindow) || IsEqualIID(riid, IID_IOleInPlaceSite)) {
            *ppvObject = static_cast<IOleInPlaceSite*>(this);
        } else if (IsEqualIID(riid, IID_IOleControlSite)) {
            *ppvObject = static_cast<IOleControlSite*>(this);
        } else if (IsEqualIID(riid, IID_IOleInPlaceFrame) ||
                   IsEqualIID(riid, IID_IOleInPlaceUIWindow)) {
            *ppvObject = static_cast<IOleInPlaceFrame*>(this);
        } else if (IsEqualIID(riid, IID_IDispatch)) {
            *ppvObject = static_cast<IDispatch*>(this);
        } else {
            return E_NOINTERFACE;
        }
        AddRef();
        return S_OK;
    }

    ULONG STDMETHODCALLTYPE AddRef() override {
        return static_cast<ULONG>(InterlockedIncrement(&m_refs));
    }

    ULONG STDMETHODCALLTYPE Release() override {
        LONG refs = InterlockedDecrement(&m_refs);
        if (refs == 0) {
            delete this;
        }
        return static_cast<ULONG>(refs);
    }

    void DetachSite() { m_site = nullptr; }

    HRESULT STDMETHODCALLTYPE SaveObject() override { return S_OK; }

    HRESULT STDMETHODCALLTYPE GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker,
                                         IMoniker** ppmk) override {
        (void)dwAssign; (void)dwWhichMoniker;
        if (ppmk) {
            *ppmk = nullptr;
        }
        return S_FALSE;
    }

    HRESULT STDMETHODCALLTYPE GetContainer(IOleContainer** ppContainer) override {
        if (!ppContainer) {
            return E_POINTER;
        }
        *ppContainer = nullptr;
        return m_site ? m_site->GetContainer(ppContainer) : E_FAIL;
    }

    HRESULT STDMETHODCALLTYPE ShowObject() override {
        if (m_site && m_site->m_lpInPlaceObject) {
            HWND hwnd = nullptr;
            if (SUCCEEDED(m_site->m_lpInPlaceObject->GetWindow(&hwnd))) {
                m_site->m_hWnd = hwnd;
            }
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnShowWindow(BOOL fShow) override {
        (void)fShow;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE RequestNewObjectLayout() override { return S_OK; }

    HRESULT STDMETHODCALLTYPE GetWindow(HWND* phwnd) override {
        if (!phwnd) {
            return E_POINTER;
        }
        *phwnd = GetSiteParentWindow(m_site);
        return *phwnd ? S_OK : E_FAIL;
    }

    HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL fEnterMode) override {
        (void)fEnterMode;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE CanInPlaceActivate() override { return S_OK; }

    HRESULT STDMETHODCALLTYPE OnInPlaceActivate() override {
        if (m_site) {
            m_site->m_bInPlaceActive = TRUE;
            if (m_site->m_lpInPlaceObject) {
                HWND hwnd = nullptr;
                if (SUCCEEDED(m_site->m_lpInPlaceObject->GetWindow(&hwnd))) {
                    m_site->m_hWnd = hwnd;
                }
            }
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnUIActivate() override { return S_OK; }

    HRESULT STDMETHODCALLTYPE GetWindowContext(IOleInPlaceFrame** ppFrame,
                                               IOleInPlaceUIWindow** ppDoc,
                                               LPRECT lprcPosRect,
                                               LPRECT lprcClipRect,
                                               LPOLEINPLACEFRAMEINFO lpFrameInfo) override {
        if (ppFrame) {
            *ppFrame = static_cast<IOleInPlaceFrame*>(this);
            AddRef();
        }
        if (ppDoc) {
            *ppDoc = nullptr;
        }
        RECT rc = GetSitePositionRect(m_site);
        if (lprcPosRect) {
            *lprcPosRect = rc;
        }
        if (lprcClipRect) {
            HWND hwndParent = GetSiteParentWindow(m_site);
            if (hwndParent) {
                ::GetClientRect(hwndParent, lprcClipRect);
            } else {
                *lprcClipRect = rc;
            }
        }
        if (lpFrameInfo) {
            memset(lpFrameInfo, 0, sizeof(*lpFrameInfo));
            lpFrameInfo->cb = sizeof(*lpFrameInfo);
            lpFrameInfo->hwndFrame = GetSiteParentWindow(m_site);
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE GetBorder(LPRECT lprectBorder) override {
        if (!lprectBorder) {
            return E_POINTER;
        }
        HWND hwndParent = GetSiteParentWindow(m_site);
        if (hwndParent) {
            ::GetClientRect(hwndParent, lprectBorder);
        } else {
            *lprectBorder = GetSitePositionRect(m_site);
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE RequestBorderSpace(LPCBORDERWIDTHS pborderwidths) override {
        (void)pborderwidths;
        return INPLACE_E_NOTOOLSPACE;
    }

    HRESULT STDMETHODCALLTYPE SetBorderSpace(LPCBORDERWIDTHS pborderwidths) override {
        (void)pborderwidths;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE SetActiveObject(IOleInPlaceActiveObject* pActiveObject,
                                              LPCOLESTR pszObjName) override {
        (void)pActiveObject; (void)pszObjName;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE InsertMenus(HMENU hmenuShared,
                                          LPOLEMENUGROUPWIDTHS lpMenuWidths) override {
        (void)hmenuShared;
        if (lpMenuWidths) {
            memset(lpMenuWidths, 0, sizeof(*lpMenuWidths));
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE SetMenu(HMENU hmenuShared, HOLEMENU holemenu,
                                      HWND hwndActiveObject) override {
        (void)hmenuShared; (void)holemenu; (void)hwndActiveObject;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE RemoveMenus(HMENU hmenuShared) override {
        (void)hmenuShared;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE SetStatusText(LPCOLESTR pszStatusText) override {
        (void)pszStatusText;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE EnableModeless(BOOL fEnable) override {
        (void)fEnable;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE TranslateAccelerator(LPMSG lpmsg, WORD wID) override {
        (void)lpmsg; (void)wID;
        return S_FALSE;
    }

    HRESULT STDMETHODCALLTYPE Scroll(SIZE scrollExtant) override {
        (void)scrollExtant;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnUIDeactivate(BOOL fUndoable) override {
        (void)fUndoable;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate() override {
        if (m_site) {
            m_site->m_bInPlaceActive = FALSE;
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE DiscardUndoState() override { return S_OK; }

    HRESULT STDMETHODCALLTYPE DeactivateAndUndo() override {
        if (m_site) {
            m_site->Deactivate();
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnPosRectChange(LPCRECT lprcPosRect) override {
        if (!lprcPosRect) {
            return E_POINTER;
        }
        if (m_site && m_site->m_lpInPlaceObject) {
            return m_site->m_lpInPlaceObject->SetObjectRects(lprcPosRect, lprcPosRect);
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnControlInfoChanged() override { return S_OK; }

    HRESULT STDMETHODCALLTYPE LockInPlaceActive(BOOL fLock) override {
        (void)fLock;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE GetExtendedControl(IDispatch** ppDisp) override {
        if (!ppDisp) {
            return E_POINTER;
        }
        *ppDisp = nullptr;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE TransformCoords(POINTL* pPtlHimetric, POINTF* pPtfContainer,
                                               DWORD dwFlags) override {
        if (!pPtlHimetric || !pPtfContainer) {
            return E_POINTER;
        }
        HWND hwndParent = GetSiteParentWindow(m_site);
        HDC hdc = ::GetDC(hwndParent);
        int dpiX = hdc ? ::GetDeviceCaps(hdc, LOGPIXELSX) : 96;
        int dpiY = hdc ? ::GetDeviceCaps(hdc, LOGPIXELSY) : 96;
        if (hdc) {
            ::ReleaseDC(hwndParent, hdc);
        }
        if (dpiX <= 0) {
            dpiX = 96;
        }
        if (dpiY <= 0) {
            dpiY = 96;
        }

        if (dwFlags & XFORMCOORDS_HIMETRICTOCONTAINER) {
            pPtfContainer->x = static_cast<float>((pPtlHimetric->x * dpiX) / 2540.0);
            pPtfContainer->y = static_cast<float>((pPtlHimetric->y * dpiY) / 2540.0);
            return S_OK;
        }
        if (dwFlags & XFORMCOORDS_CONTAINERTOHIMETRIC) {
            pPtlHimetric->x = static_cast<LONG>((pPtfContainer->x * 2540.0f) / dpiX);
            pPtlHimetric->y = static_cast<LONG>((pPtfContainer->y * 2540.0f) / dpiY);
            return S_OK;
        }
        return E_INVALIDARG;
    }

    HRESULT STDMETHODCALLTYPE TranslateAccelerator(MSG* pMsg, DWORD grfModifiers) override {
        (void)pMsg; (void)grfModifiers;
        return S_FALSE;
    }

    HRESULT STDMETHODCALLTYPE OnFocus(BOOL fGotFocus) override {
        (void)fGotFocus;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE ShowPropertyFrame() override {
        if (m_site) {
            m_site->ShowPropertyFrame();
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT* pctinfo) override {
        if (!pctinfo) {
            return E_POINTER;
        }
        *pctinfo = 0;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo) override {
        (void)iTInfo; (void)lcid;
        if (ppTInfo) {
            *ppTInfo = nullptr;
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames,
                                            UINT cNames, LCID lcid, DISPID* rgDispId) override {
        (void)riid; (void)rgszNames; (void)cNames; (void)lcid;
        if (rgDispId && cNames) {
            for (UINT i = 0; i < cNames; ++i) {
                rgDispId[i] = DISPID_UNKNOWN;
            }
        }
        return DISP_E_UNKNOWNNAME;
    }

    HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,
                                     WORD wFlags, DISPPARAMS* pDispParams,
                                     VARIANT* pVarResult, EXCEPINFO* pExcepInfo,
                                     UINT* puArgErr) override {
        (void)riid; (void)lcid; (void)pDispParams; (void)pExcepInfo; (void)puArgErr;
        if (!(wFlags & DISPATCH_PROPERTYGET)) {
            return DISP_E_MEMBERNOTFOUND;
        }
        if (!pVarResult) {
            return E_POINTER;
        }
        if (TryGetAmbientOverride(m_site, dispIdMember, pVarResult)) {
            return S_OK;
        }

        switch (dispIdMember) {
        case DISPID_AMBIENT_USERMODE:
        case DISPID_AMBIENT_SUPPORTSMNEMONICS:
            SetVariantBool(pVarResult, TRUE);
            return S_OK;
        case DISPID_AMBIENT_UIDEAD:
        case DISPID_AMBIENT_DISPLAYASDEFAULT:
        case DISPID_AMBIENT_SHOWGRABHANDLES:
        case DISPID_AMBIENT_SHOWHATCHING:
            SetVariantBool(pVarResult, FALSE);
            return S_OK;
        case DISPID_AMBIENT_BACKCOLOR:
            VariantInit(pVarResult);
            pVarResult->vt = VT_I4;
            pVarResult->lVal = static_cast<LONG>(::GetSysColor(COLOR_WINDOW));
            return S_OK;
        case DISPID_AMBIENT_FORECOLOR:
            VariantInit(pVarResult);
            pVarResult->vt = VT_I4;
            pVarResult->lVal = static_cast<LONG>(::GetSysColor(COLOR_WINDOWTEXT));
            return S_OK;
        case DISPID_AMBIENT_LOCALEID:
            VariantInit(pVarResult);
            pVarResult->vt = VT_I4;
            pVarResult->lVal = static_cast<LONG>(::GetUserDefaultLCID());
            return S_OK;
        case DISPID_AMBIENT_DISPLAYNAME:
        case DISPID_AMBIENT_SCALEUNITS:
            VariantInit(pVarResult);
            pVarResult->vt = VT_BSTR;
            pVarResult->bstrVal = SysAllocString(L"");
            return pVarResult->bstrVal ? S_OK : E_OUTOFMEMORY;
        case DISPID_AMBIENT_APPEARANCE:
            VariantInit(pVarResult);
            pVarResult->vt = VT_I2;
            pVarResult->iVal = 0;
            return S_OK;
        default:
            return DISP_E_MEMBERNOTFOUND;
        }
    }

private:
    volatile LONG m_refs;
    COleControlSite* m_site;
};
} } }

namespace openmfc { namespace detail { namespace olecore {
struct OleControlSiteState {
    UINT controlId = 0;
    ControlSiteAdapter* adapter = nullptr;
};
} } }

namespace openmfc { namespace detail { namespace olecore {
extern std::map<const COleControlSite*, OleControlSiteState> g_oleControlSiteState;
} } }

namespace openmfc { namespace detail { namespace olecore {
void SetControlSiteId(const COleControlSite* pSite, UINT controlId);
} } }

namespace openmfc { namespace detail { namespace olecore {
void RemoveControlSiteState(const COleControlSite* pSite);
} } }

namespace openmfc { namespace detail { namespace olecore {
BOOL TryGetControlSiteId(const COleControlSite* pSite, UINT* pControlId);
} } }

namespace openmfc { namespace detail { namespace olecore {
ControlSiteAdapter* GetControlSiteAdapter(COleControlSite* pSite, bool create);
} } }
























//=============================================================================
// COleControlContainer
//=============================================================================






//=============================================================================
// COleIPFrameWnd
//=============================================================================






//=============================================================================
// COleConnPtContainer
//=============================================================================


//=============================================================================
// CEnumOleVerb
//=============================================================================



// Fetches the verb at the cursor and advances. Retail (mfc140u
// ?OnNext@CEnumOleVerb@@) runs the CEnumArray element copy first and, when that
// succeeds, replaces OLEVERB::lpszVerbName with a caller-owned duplicate --
// allocated with the COM task allocator, since the caller is expected to
// CoTaskMemFree it -- and raises a memory exception if that allocation fails.





//=============================================================================
// COleDialog
//=============================================================================





//=============================================================================
// COleBusyDialog
//=============================================================================




//=============================================================================
// COleChangeIconDialog
//=============================================================================





//=============================================================================
// COleChangeSourceDialog
//=============================================================================








//=============================================================================
// COleConvertDialog
//=============================================================================










//=============================================================================
// COleInsertDialog
//=============================================================================














//=============================================================================
// COleLinksDialog
//=============================================================================




//=============================================================================
// COlePasteSpecialDialog
//=============================================================================













//=============================================================================
// COlePropertiesDialog
//=============================================================================




//=============================================================================
// COleUpdateDialog
//=============================================================================




//=============================================================================
// COleControl
//=============================================================================


// sizeof/offset guards for the retail-transcribed layout. If any of these fire,
// a member edit has silently broken binary compatibility with mfc140u.














// COleControl::GetAmbientDispatchDriver — real implementation lives in
// global_olecontrol_batch1.cpp (decoded from retail); it lazily binds the
// embedded driver to the container's IDispatch via m_pClientSite.
extern "C" COleDispatchDriver* MS_ABI
impl__GetAmbientDispatchDriver_COleControl__IEAAPEAVCOleDispatchDriver__XZ(COleControl* pThis);







//=============================================================================
// OLE Helper Functions (extern "C" exports)
//=============================================================================
extern "C" {


MS_ABI int impl__AfxOleRegisterTypeLib(REFGUID guid, const wchar_t* lpszFileName);

MS_ABI int impl__AfxOleRegisterServerClass(REFCLSID clsid, const wchar_t* lpszClassName,
                                            const wchar_t* lpszShortTypeName,
                                            const wchar_t* lpszLongTypeName,
                                            int nAppType, const wchar_t** rglpszRegister,
                                            const wchar_t** rglpszOverwrite);

MS_ABI int impl__AfxOleRegisterControlClass(REFCLSID clsid, const wchar_t* lpszClassName,
                                             const wchar_t* lpszShortTypeName,
                                             const wchar_t* lpszLongTypeName,
                                             DWORD dwMiscStatus, DWORD dwFlags, DWORD dwVersion);

MS_ABI int impl__AfxOleUnregisterClass(REFCLSID clsid, const wchar_t* lpszClassName);

MS_ABI int impl__AfxOleSetEditMenu(COleClientItem* pItem, CMenu* pMenu, UINT iMenuItem,
                                    UINT nIDVerbMin, UINT nIDVerbMax, UINT nIDConvert);

MS_ABI int impl__AfxOleSetUserCtrl(BOOL bUserCtrl);

MS_ABI COLORREF impl__AfxOleTranslateColor(OLE_COLOR clrColor, HPALETTE hpal);

MS_ABI int impl__AfxOleUnregisterTypeLib(REFGUID guid, WORD wVerMajor, WORD wVerMinor, LCID lcid);

}

//=============================================================================
// COleClientItem - additional methods
//=============================================================================




//=============================================================================
// COleControl - additional methods
//=============================================================================
// COleControl::GetControlFlags() — retail is `mov eax,0x2 ; ret`: the base
// implementation returns clipPaintDC and nothing else, unconditionally.
// Derived controls override it to add fastBeginPaint / pointerInactive /
// windowlessActivate / canOptimizeDraw. The previous version here computed a
// flag word from m_bOptimizedDraw, m_hWnd and the control site, which meant a
// plain COleControl reported capabilities (and, with no window, omitted
// clipPaintDC) that the real base class never reports.
// m_cxExtent / m_cyExtent are the control's width and height in HIMETRIC, per
// the retail layout — one scalar each, not a pair of CSize.


// Fires the notification only. The ready state itself is owned by whoever
// transitions it (the constructor, or a derived control moving through
// LOADING/INTERACTIVE); stamping COMPLETE here would clobber an intermediate
// state the moment the container was told about it.

// Retail reads these straight out of the object: GetBackColor is
// `mov eax,[rcx+0x164]`. They used to be served from the OleControlState side
// table, which a real client cannot see -- so the constructor's
// m_clrBackColor (0x80000005) and the value this returned disagreed. Now the
// ABI-visible member is the single source of truth.
// Retail: `mov eax,[rcx+0x168]`.
// Retail: `mov eax,[rcx+0x19c]`.
// COleControl::GetHwnd() — retail gates the handle on two flag bits:
//     test DWORD PTR [rcx+0x160],0x2400 ; jne take_handle
//     xor eax,eax ; ret                 ; otherwise report no window
//   take_handle: mov rax,[rcx+0x40] ; ret
// 0x2400 is bit 10 (m_bInPlaceActive) | bit 13 (m_bOpen), both named in this
// header, so the test is spelled with the members rather than a raw word read.
// A control that has an HWND but is not activated reports 0 to its container,
// which is what an OLE host uses to decide whether the control can be talked
// to as a window at all. Returning the raw handle unconditionally, as this
// did, misreports an inactive control.
// Retail: `movzx eax,WORD PTR [rcx+0x198]` -- a 16-bit read, so the stored
// value is returned unwidened.
// Retail: `movzx eax,WORD PTR [rcx+0x19a]`.
// Retail: `mov eax,[rcx+0x1a0]`.

// Retail (?SetModifiedFlag@COleControl@@QEAAXH@Z):
//     mov   eax,[rcx+0x160]      ; movzx r8d,dl   ; shl r8d,2
//     xor   r8d,eax              ; and   r8d,4    ; xor r8d,eax
//     mov   [rcx+0x160],r8d      ; ret
// The MSVC idiom for a 1-bit bitfield store: bit 2 is set from bit 0 of the
// LOW BYTE of the argument. So the value is truncated, not normalized --
// SetModifiedFlag(2) clears the flag, and a plain assignment to the bitfield
// reproduces that exactly. Do not "fix" this into `bModified ? 1 : 0`.
//
// This used to write only the OleControlState side table, which no client can
// see, while IsModified() read the m_bModified bit -- so a control marked
// dirty by any property setter still reported itself clean. Same defect class
// as the accessor migration in ca63d6a, which corrected OnResetState but left
// the setter behind.




//=============================================================================
// COleDocObjectItem
//=============================================================================


//=============================================================================
// CEnumFormatEtc
//=============================================================================

namespace openmfc { namespace detail { namespace olecore {
STDMETHODIMP_(ULONG) CEnumFormatEtc::AddRef();
} } }
namespace openmfc { namespace detail { namespace olecore {
STDMETHODIMP_(ULONG) CEnumFormatEtc::Release();
} } }


//=============================================================================
// COleDocument OnUpdate* command-UI handlers
//=============================================================================







//=============================================================================
// COleServerDoc OnUpdate* command-UI handlers
//=============================================================================




//=============================================================================
// COleIPFrameWnd OnUpdate* command-UI handler
//=============================================================================


//=============================================================================
// COleDBRecordView OnUpdate* handlers (no header class yet)
//=============================================================================





//=============================================================================
// Frame Window Ex OnUpdate* handlers (no header class yet)
//=============================================================================



//=============================================================================
// CArray<COleVariant> template helpers
//=============================================================================


