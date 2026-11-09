// Native 96-byte IDataObject source with an embedded COM part and owned cache.
#define OPENMFC_APPCORE_IMPL
#include "detail/OleDataSourceSupport.h"
#include "detail/OleProcessStateSupport.h"
#include "detail/InterfaceMapsSupport.h"
#include "detail/ModuleThreadStateSupport.h"
#include "detail/CSharedFileSupport.h"
#include <cstdlib>
#include <cstring>
#include <new>
extern "C" {
void* impl___2_YAPEAX_K_Z(size_t);
void impl___3_YAXPEAX_Z(void*);
ULONG impl__ExternalAddRef_CCmdTarget__QEAAKXZ(CCmdTarget*);
ULONG impl__ExternalRelease_CCmdTarget__QEAAKXZ(CCmdTarget*);
ULONG impl__ExternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(CCmdTarget*,const void*,void**);
ULONG impl__InternalRelease_CCmdTarget__QEAAKXZ(CCmdTarget*);
IUnknown* impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(CCmdTarget*,const void*);
AFX_MODULE_THREAD_STATE* impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ();
void* impl___0AFX_MAINTAIN_STATE2__QEAA_PEAVAFX_MODULE_STATE___Z(void*,AFX_MODULE_STATE*);
void impl___1AFX_MAINTAIN_STATE2__QEAA_XZ(void*);
void impl__AfxThrowOleException__YAXJ_Z(long);
long impl__Process_COleException__SAJPEBVCException___Z(const CException*);
}
namespace {
COleDataSource* Owner(COleDataSource::XDataObject* part) {
    return reinterpret_cast<COleDataSource*>(reinterpret_cast<BYTE*>(part)-offsetof(COleDataSource,m_xDataObject));
}
class ModuleScope {
    void* state[2];
public:
    explicit ModuleScope(AFX_MODULE_STATE* module) {impl___0AFX_MAINTAIN_STATE2__QEAA_PEAVAFX_MODULE_STATE___Z(state,module);}
    ~ModuleScope() {impl___1AFX_MAINTAIN_STATE2__QEAA_XZ(state);}
};
const AFX_INTERFACEMAP_ENTRY entries[]={{&IID_IDataObject,88},{nullptr,SIZE_MAX}};
void DefaultFormat(FORMATETC& format,CLIPFORMAT cf,DWORD tymed=TYMED_HGLOBAL) {
    // Padding is not part of the native default-format initialization.
    format.cfFormat=cf;format.ptd=nullptr;format.dwAspect=DVASPECT_CONTENT;format.lindex=-1;format.tymed=tymed;
}
HRESULT ProcessException(CException* error) {
    HRESULT result=impl__Process_COleException__SAJPEBVCException___Z(error);
    if (error) error->Delete();
    return result;
}
template<class T> T* NewFrameworkObject() {
    void* storage=impl___2_YAPEAX_K_Z(sizeof(T));
    if (!storage) return nullptr;
    try {return new(storage) T;}
    catch (...) {impl___3_YAXPEAX_Z(storage);throw;}
}
DVTARGETDEVICE* CopyTargetDevice(DVTARGETDEVICE* source) {
    if (!source) return nullptr;
    auto* copy=static_cast<DVTARGETDEVICE*>(CoTaskMemAlloc(source->tdSize));
    if (copy && source->tdSize) memcpy(copy,source,source->tdSize);
    return copy;
}
}
namespace openmfc { namespace detail { namespace interfacemaps {
const AFX_INTERFACEMAP* AFXAPI gib_COleDataSource(){return impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ();}
const AFX_INTERFACEMAP g_imap_COleDataSource={gib_COleDataSource,entries};
} } }
COleDataSource::COleDataSource():m_pDataCache(nullptr),m_nMaxSize(0),m_nSize(0),m_nGrowBy(10) {}
COleDataSource::~COleDataSource() {
    auto* state=openmfc::detail::GetOleProcessState();
    if (state->m_pClipboardSource==this) state->m_pClipboardSource=nullptr;
    Empty();
}
void COleDataSource::Empty() {
    if (!m_pDataCache) return;
    for (UINT i=0;i<m_nSize;++i) {
        CoTaskMemFree(m_pDataCache[i].m_formatEtc.ptd);
        ReleaseStgMedium(&m_pDataCache[i].m_stgMedium);
    }
    free(m_pDataCache);
    m_pDataCache=nullptr;m_nMaxSize=0;m_nSize=0;
}
// A target-device mismatch keeps the last near match; an exact device wins
// immediately. Delayed data has no index restriction until it holds a medium.
AFX_DATACACHE_ENTRY* COleDataSource::Lookup(FORMATETC* request,DATADIR direction) const {
    AFX_DATACACHE_ENTRY* last=nullptr;
    for (UINT i=0;i<m_nSize;++i) {
        auto* entry=m_pDataCache+i;auto* format=&entry->m_formatEtc;
        if (format->cfFormat!=request->cfFormat || !(format->tymed&request->tymed)) continue;
        if (format->dwAspect!=DVASPECT_THUMBNAIL && format->dwAspect!=DVASPECT_ICON &&
            entry->m_stgMedium.tymed!=TYMED_NULL && format->lindex!=request->lindex &&
            !(format->lindex==0 && request->lindex==-1) && !(format->lindex==-1 && request->lindex==0)) continue;
        if (format->dwAspect!=request->dwAspect || entry->m_nDataDir!=direction) continue;
        last=entry;
        auto* cached=format->ptd;auto* wanted=request->ptd;
        if (!cached) {if (!wanted) return entry;}
        else if (wanted && cached->tdSize==wanted->tdSize && !memcmp(cached,wanted,cached->tdSize)) return entry;
    }
    return last;
}
AFX_DATACACHE_ENTRY* COleDataSource::GetCacheEntry(FORMATETC* format,DATADIR direction) {
    auto* entry=Lookup(format,direction);
    if (entry) {
        CoTaskMemFree(entry->m_formatEtc.ptd);
        ReleaseStgMedium(&entry->m_stgMedium);
    } else {
        auto* cache=m_pDataCache;
        if (!cache || m_nSize==m_nMaxSize) {
            cache=static_cast<AFX_DATACACHE_ENTRY*>(impl___2_YAPEAX_K_Z(size_t(UINT(m_nMaxSize+m_nGrowBy))*sizeof(AFX_DATACACHE_ENTRY)));
            if (m_pDataCache) {
                size_t bytes=size_t(m_nSize)*sizeof(AFX_DATACACHE_ENTRY);
                if (bytes && memcpy_s(cache,size_t(UINT(m_nMaxSize+m_nGrowBy))*sizeof(AFX_DATACACHE_ENTRY),m_pDataCache,bytes)) AfxThrowInvalidArgException();
                free(m_pDataCache);
            }
            m_nMaxSize+=m_nGrowBy;m_pDataCache=cache;
        }
        entry=cache+m_nSize++;
    }
    entry->m_nDataDir=direction;
    memcpy(&entry->m_formatEtc,format,sizeof(FORMATETC));
    return entry;
}
void COleDataSource::CacheGlobalData(CLIPFORMAT cf,HGLOBAL global,FORMATETC* format) {
    FORMATETC local;
    if (!format && cf) {DefaultFormat(local,cf);format=&local;}
    format->tymed=TYMED_HGLOBAL;
    auto* entry=GetCacheEntry(format,DATADIR_GET);
    entry->m_stgMedium.hGlobal=global;entry->m_stgMedium.tymed=TYMED_HGLOBAL;entry->m_stgMedium.pUnkForRelease=nullptr;
}
void COleDataSource::CacheData(CLIPFORMAT cf,STGMEDIUM* medium,FORMATETC* format) {
    if (!medium) AfxThrowInvalidArgException();
    FORMATETC local;
    if (!format && cf) {DefaultFormat(local,cf);format=&local;}
    format->tymed=medium->tymed;
    auto* entry=GetCacheEntry(format,DATADIR_GET);
    memcpy(&entry->m_stgMedium,medium,sizeof(STGMEDIUM));
}
void COleDataSource::DelayRenderData(CLIPFORMAT cf,FORMATETC* format) {
    FORMATETC local;
    if (!format) {if (cf) {DefaultFormat(local,cf);format=&local;}format->tymed=TYMED_HGLOBAL;}
    if (cf) format->cfFormat=cf;
    auto* entry=GetCacheEntry(format,DATADIR_GET);
    memset(&entry->m_stgMedium,0,sizeof(STGMEDIUM));
}
void COleDataSource::DelayRenderFileData(CLIPFORMAT cf,FORMATETC* format) {
    FORMATETC local;
    if (!format && cf) {DefaultFormat(local,cf);format=&local;}
    format->tymed=TYMED_HGLOBAL|TYMED_ISTREAM;
    auto* entry=GetCacheEntry(format,DATADIR_GET);
    entry->m_stgMedium.tymed=TYMED_NULL;entry->m_stgMedium.hGlobal=nullptr;entry->m_stgMedium.pUnkForRelease=nullptr;
}
void COleDataSource::DelaySetData(CLIPFORMAT cf,FORMATETC* format) {
    FORMATETC local;
    if (!format && cf) {DefaultFormat(local,cf,UINT_MAX);format=&local;}
    auto* entry=GetCacheEntry(format,DATADIR_SET);
    entry->m_stgMedium.tymed=TYMED_NULL;entry->m_stgMedium.hGlobal=nullptr;entry->m_stgMedium.pUnkForRelease=nullptr;
}
BOOL COleDataSource::OnRenderGlobalData(FORMATETC*,HGLOBAL*) {return FALSE;}
BOOL COleDataSource::OnRenderFileData(FORMATETC*,CFile*) {return FALSE;}
BOOL COleDataSource::OnSetData(FORMATETC*,STGMEDIUM*,BOOL) {return FALSE;}
BOOL COleDataSource::OnRenderData(FORMATETC* format,STGMEDIUM* medium) {
    if (format->tymed&TYMED_HGLOBAL) {
        HGLOBAL global=medium->hGlobal;
        if (OnRenderGlobalData(format,&global)) {medium->tymed=TYMED_HGLOBAL;medium->hGlobal=global;return TRUE;}
        CSharedFile file(GMEM_MOVEABLE,4096);
        if (medium->tymed==TYMED_HGLOBAL) file.SetHandle(medium->hGlobal,FALSE);
        if (OnRenderFileData(format,&file)) {medium->tymed=TYMED_HGLOBAL;medium->hGlobal=file.Detach();return TRUE;}
        if (medium->tymed==TYMED_HGLOBAL) file.Detach();
    }
    if (format->tymed&TYMED_ISTREAM) {
        COleStreamFile file;
        if (medium->tymed==TYMED_ISTREAM) {
            if (!medium->pstm) AfxThrowInvalidArgException();
            file.m_lpStream=medium->pstm;file.m_bCloseOnDelete=FALSE;
        } else if (!file.CreateMemoryStream()) AfxThrowMemoryException();
        // CreateMemoryStream preserves the borrowed close flag. In particular,
        // a failed callback does not implicitly release a newly created stream.
        if (OnRenderFileData(format,&file)) {medium->tymed=TYMED_ISTREAM;medium->pstm=file.Detach();return TRUE;}
        if (medium->tymed==TYMED_ISTREAM) file.Detach();
    }
    return FALSE;
}
void COleDataSource::SetClipboard() {
    auto* object=impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(this,&IID_IDataObject);
    HRESULT result=OleSetClipboard(static_cast<IDataObject*>(object));
    if (result!=S_OK) impl__AfxThrowOleException__YAXJ_Z(result);
    openmfc::detail::GetOleProcessState()->m_pClipboardSource=this;
    impl__InternalRelease_CCmdTarget__QEAAKXZ(this);
}
COleDataSource* PASCAL COleDataSource::GetClipboardOwner() {
    auto* state=openmfc::detail::GetOleProcessState();
    if (!state->m_pClipboardSource) return nullptr;
    auto* object=impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(state->m_pClipboardSource,&IID_IDataObject);
    if (OleIsCurrentClipboard(static_cast<IDataObject*>(object))!=S_OK) state->m_pClipboardSource=nullptr;
    return state->m_pClipboardSource;
}
void PASCAL COleDataSource::FlushClipboard() {if (GetClipboardOwner()) OleFlushClipboard();}
IDataObject* COleDataSource::GetInterface(BOOL addRef) {
    auto* object=static_cast<IDataObject*>(impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(this,&IID_IDataObject));
    if (addRef && object) object->AddRef();
    return object;
}
DROPEFFECT COleDataSource::DoDragDrop(DWORD allowed,LPCRECT start,COleDropSource* supplied) {
    COleDropSource defaultSource;
    auto* source=supplied?supplied:&defaultSource;
    source->m_bDragStarted=FALSE;
    auto* rectangle=reinterpret_cast<RECT*>(&source->m_rectStartDrag);
    if (start) CopyRect(rectangle,start);
    else {POINT point{};GetCursorPos(&point);SetRect(rectangle,point.x,point.y,point.x,point.y);}
    if (!rectangle->left && !rectangle->right && !rectangle->top && !rectangle->bottom) source->m_bDragStarted=TRUE;
    else if (IsRectEmpty(rectangle)) InflateRect(rectangle,COleDropSource::nDragMinDist,COleDropSource::nDragMinDist);
    // MFC selects this virtual target before evaluating the main-window
    // callback, which can replace the supplied source's virtual table.
    using BeginDrag=BOOL (AFXAPI*)(COleDropSource*,CWnd*);
    auto begin=reinterpret_cast<BeginDrag>((*reinterpret_cast<void***>(source))[24]);
    auto* thread=impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()->m_pCurrentWinThread;
    CWnd* window=thread?thread->GetMainWnd():nullptr;
    if (!begin(source,window)) return DROPEFFECT_NONE;
    auto* data=static_cast<IDataObject*>(impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(this,&IID_IDataObject));
    auto* drop=static_cast<IDropSource*>(impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(source,&IID_IDropSource));
    DWORD result=0;::DoDragDrop(data,drop,allowed,&result);return result;
}
const AFX_INTERFACEMAP* AFXAPI COleDataSource::GetThisInterfaceMap(){return &openmfc::detail::interfacemaps::g_imap_COleDataSource;}
const AFX_INTERFACEMAP* COleDataSource::GetInterfaceMap() const{return GetThisInterfaceMap();}
STDMETHODIMP COleDataSource::XDataObject::QueryInterface(REFIID iid,void** result){return HRESULT(impl__ExternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(Owner(this),&iid,result));}
STDMETHODIMP_(ULONG) COleDataSource::XDataObject::AddRef(){return impl__ExternalAddRef_CCmdTarget__QEAAKXZ(Owner(this));}
STDMETHODIMP_(ULONG) COleDataSource::XDataObject::Release(){return impl__ExternalRelease_CCmdTarget__QEAAKXZ(Owner(this));}
STDMETHODIMP COleDataSource::XDataObject::GetData(FORMATETC* format,STGMEDIUM* medium) {
    auto* source=Owner(this);ModuleScope scope(source->m_pModuleState);
    if (!format || !medium) return E_INVALIDARG;
    auto* entry=source->Lookup(format,DATADIR_GET);
    if (!entry) return DATA_E_FORMATETC;
    memset(medium,0,sizeof(*medium));
    // Native EH covers virtual rendering only; copying cached media can throw
    // through this COM boundary while the module scope still restores TLS.
    if (entry->m_stgMedium.tymed!=TYMED_NULL) return openmfc::detail::CopyDataMedium(format->cfFormat,medium,&entry->m_stgMedium)?S_OK:DATA_E_FORMATETC;
    try {return source->OnRenderData(format,medium)?S_OK:DATA_E_FORMATETC;}
    catch (CException* error) {return ProcessException(error);}
}
STDMETHODIMP COleDataSource::XDataObject::GetDataHere(FORMATETC* format,STGMEDIUM* medium) {
    auto* source=Owner(this);ModuleScope scope(source->m_pModuleState);
    if (!format || !medium) return E_INVALIDARG;
    format->tymed=medium->tymed;
    auto* entry=source->Lookup(format,DATADIR_GET);
    if (!entry) return DATA_E_FORMATETC;
    if (entry->m_stgMedium.tymed!=TYMED_NULL) return openmfc::detail::CopyDataMedium(format->cfFormat,medium,&entry->m_stgMedium)?S_OK:DATA_E_FORMATETC;
    try {return source->OnRenderData(format,medium)?S_OK:DATA_E_FORMATETC;}
    catch (CException* error) {return ProcessException(error);}
}
STDMETHODIMP COleDataSource::XDataObject::QueryGetData(FORMATETC* format){return !format?E_INVALIDARG:(Owner(this)->Lookup(format,DATADIR_GET)?S_OK:DATA_E_FORMATETC);}
STDMETHODIMP COleDataSource::XDataObject::GetCanonicalFormatEtc(FORMATETC*,FORMATETC*){return DATA_S_SAMEFORMATETC;}
STDMETHODIMP COleDataSource::XDataObject::SetData(FORMATETC* format,STGMEDIUM* medium,BOOL release) {
    auto* source=Owner(this);ModuleScope scope(source->m_pModuleState);
    if (!format || !medium) return E_INVALIDARG;
    if (!source->Lookup(format,DATADIR_SET)) return DATA_E_FORMATETC;
    try {return source->OnSetData(format,medium,release)?S_OK:E_UNEXPECTED;}
    catch (CException* error) {return ProcessException(error);}
}
STDMETHODIMP COleDataSource::XDataObject::EnumFormatEtc(DWORD direction,IEnumFORMATETC** out) {
    if (!out) return E_POINTER;
    *out=nullptr;
    try {
        auto* enumerator=NewFrameworkObject<CEnumFormatEtc>();
        auto* source=Owner(this);
        for (UINT i=0;i<source->m_nSize;++i) {
            auto* entry=source->m_pDataCache+i;
            if (entry->m_nDataDir&direction) {
                FORMATETC format;
                format.cfFormat=entry->m_formatEtc.cfFormat;
                format.ptd=CopyTargetDevice(entry->m_formatEtc.ptd);
                format.dwAspect=entry->m_formatEtc.dwAspect;format.lindex=entry->m_formatEtc.lindex;format.tymed=entry->m_formatEtc.tymed;
                enumerator->AddFormat(&format);
            }
        }
        *out=reinterpret_cast<IEnumFORMATETC*>(reinterpret_cast<BYTE*>(enumerator)+0x68);
        return S_OK;
    } catch (CException* error) {if (error) error->Delete();return E_OUTOFMEMORY;}
}
STDMETHODIMP COleDataSource::XDataObject::DAdvise(FORMATETC*,DWORD,IAdviseSink*,DWORD* connection){*connection=0;return OLE_E_ADVISENOTSUPPORTED;}
STDMETHODIMP COleDataSource::XDataObject::DUnadvise(DWORD){return OLE_E_ADVISENOTSUPPORTED;}
STDMETHODIMP COleDataSource::XDataObject::EnumDAdvise(IEnumSTATDATA** out){*out=nullptr;return OLE_E_ADVISENOTSUPPORTED;}

// Symbol: ?SetData@XDataObject@COleDataSource@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@H@Z
extern "C" HRESULT impl__SetData_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z(COleDataSource::XDataObject* p,FORMATETC* f,STGMEDIUM* v,BOOL r) { return p->COleDataSource::XDataObject::SetData(f,v,r); }

// Symbol: ?SetClipboard@COleDataSource@@QEAAXXZ
extern "C" void impl__SetClipboard_COleDataSource__QEAAXXZ(COleDataSource* p) { p->SetClipboard(); }

// Symbol: ?QueryGetData@XDataObject@COleDataSource@@UEAAJPEAUtagFORMATETC@@@Z
extern "C" HRESULT impl__QueryGetData_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC___Z(COleDataSource::XDataObject* p,FORMATETC* f) { return p->COleDataSource::XDataObject::QueryGetData(f); }

// Symbol: ?OnSetData@COleDataSource@@UEAAHPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@H@Z
extern "C" BOOL impl__OnSetData_COleDataSource__UEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z(COleDataSource* p,FORMATETC* f,STGMEDIUM* v,BOOL r) { return p->COleDataSource::OnSetData(f,v,r); }

// Symbol: ?OnRenderGlobalData@COleDataSource@@UEAAHPEAUtagFORMATETC@@PEAPEAX@Z
extern "C" BOOL impl__OnRenderGlobalData_COleDataSource__UEAAHPEAUtagFORMATETC__PEAPEAX_Z(COleDataSource* p,FORMATETC* f,HGLOBAL* h) { return p->COleDataSource::OnRenderGlobalData(f,h); }

// Symbol: ?OnRenderFileData@COleDataSource@@UEAAHPEAUtagFORMATETC@@PEAVCFile@@@Z
extern "C" BOOL impl__OnRenderFileData_COleDataSource__UEAAHPEAUtagFORMATETC__PEAVCFile___Z(COleDataSource* p,FORMATETC* f,CFile* v) { return p->COleDataSource::OnRenderFileData(f,v); }

// Symbol: ?OnRenderData@COleDataSource@@UEAAHPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" BOOL impl__OnRenderData_COleDataSource__UEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(COleDataSource* p,FORMATETC* f,STGMEDIUM* v) { return p->COleDataSource::OnRenderData(f,v); }

// Symbol: ?Lookup@COleDataSource@@IEBAPEAUAFX_DATACACHE_ENTRY@@PEAUtagFORMATETC@@W4tagDATADIR@@@Z
extern "C" AFX_DATACACHE_ENTRY* impl__Lookup_COleDataSource__IEBAPEAUAFX_DATACACHE_ENTRY__PEAUtagFORMATETC__W4tagDATADIR___Z(const COleDataSource* p,FORMATETC* f,DATADIR d) { return openmfc::detail::OleDataSourceAccess::Lookup(p,f,d); }

// Symbol: ?GetThisInterfaceMap@COleDataSource@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* impl__GetThisInterfaceMap_COleDataSource__KAPEBUAFX_INTERFACEMAP__XZ() { return &openmfc::detail::interfacemaps::g_imap_COleDataSource; }

// Symbol: ?GetInterfaceMap@COleDataSource@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* impl__GetInterfaceMap_COleDataSource__MEBAPEBUAFX_INTERFACEMAP__XZ(const COleDataSource*) { return &openmfc::detail::interfacemaps::g_imap_COleDataSource; }

// Symbol: ?GetDataHere@XDataObject@COleDataSource@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" HRESULT impl__GetDataHere_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(COleDataSource::XDataObject* p,FORMATETC* f,STGMEDIUM* v) { return p->COleDataSource::XDataObject::GetDataHere(f,v); }

// Symbol: ?GetData@XDataObject@COleDataSource@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" HRESULT impl__GetData_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(COleDataSource::XDataObject* p,FORMATETC* f,STGMEDIUM* v) { return p->COleDataSource::XDataObject::GetData(f,v); }

// Symbol: ?GetClipboardOwner@COleDataSource@@SAPEAV1@XZ
extern "C" COleDataSource* impl__GetClipboardOwner_COleDataSource__SAPEAV1_XZ() { return COleDataSource::GetClipboardOwner(); }

// Symbol: ?GetCanonicalFormatEtc@XDataObject@COleDataSource@@UEAAJPEAUtagFORMATETC@@0@Z
extern "C" HRESULT impl__GetCanonicalFormatEtc_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__0_Z(COleDataSource::XDataObject* p,FORMATETC* f,FORMATETC* out) { return p->COleDataSource::XDataObject::GetCanonicalFormatEtc(f,out); }

// Symbol: ?GetCacheEntry@COleDataSource@@IEAAPEAUAFX_DATACACHE_ENTRY@@PEAUtagFORMATETC@@W4tagDATADIR@@@Z
extern "C" AFX_DATACACHE_ENTRY* impl__GetCacheEntry_COleDataSource__IEAAPEAUAFX_DATACACHE_ENTRY__PEAUtagFORMATETC__W4tagDATADIR___Z(COleDataSource* p,FORMATETC* f,DATADIR d) { return openmfc::detail::OleDataSourceAccess::Entry(p,f,d); }

// Symbol: ?FlushClipboard@COleDataSource@@SAXXZ
extern "C" void impl__FlushClipboard_COleDataSource__SAXXZ() { COleDataSource::FlushClipboard(); }

// Symbol: ?EnumFormatEtc@XDataObject@COleDataSource@@UEAAJKPEAPEAUIEnumFORMATETC@@@Z
extern "C" HRESULT impl__EnumFormatEtc_XDataObject_COleDataSource__UEAAJKPEAPEAUIEnumFORMATETC___Z(COleDataSource::XDataObject* p,DWORD d,IEnumFORMATETC** out) { return p->COleDataSource::XDataObject::EnumFormatEtc(d,out); }

// Symbol: ?EnumDAdvise@XDataObject@COleDataSource@@UEAAJPEAPEAUIEnumSTATDATA@@@Z
extern "C" HRESULT impl__EnumDAdvise_XDataObject_COleDataSource__UEAAJPEAPEAUIEnumSTATDATA___Z(COleDataSource::XDataObject* p,IEnumSTATDATA** out) { return p->COleDataSource::XDataObject::EnumDAdvise(out); }

// Symbol: ?Empty@COleDataSource@@QEAAXXZ
extern "C" void impl__Empty_COleDataSource__QEAAXXZ(COleDataSource* p) { p->Empty(); }

// Symbol: ?DoDragDrop@COleDataSource@@QEAAKKPEBUtagRECT@@PEAVCOleDropSource@@@Z
extern "C" DROPEFFECT impl__DoDragDrop_COleDataSource__QEAAKKPEBUtagRECT__PEAVCOleDropSource___Z(COleDataSource* p,DWORD e,LPCRECT r,COleDropSource* d) { return p->DoDragDrop(e,r,d); }

// Symbol: ?DelaySetData@COleDataSource@@QEAAXGPEAUtagFORMATETC@@@Z
extern "C" void impl__DelaySetData_COleDataSource__QEAAXGPEAUtagFORMATETC___Z(COleDataSource* p,CLIPFORMAT c,FORMATETC* f) { p->DelaySetData(c,f); }

// Symbol: ?DelayRenderFileData@COleDataSource@@QEAAXGPEAUtagFORMATETC@@@Z
extern "C" void impl__DelayRenderFileData_COleDataSource__QEAAXGPEAUtagFORMATETC___Z(COleDataSource* p,CLIPFORMAT c,FORMATETC* f) { p->DelayRenderFileData(c,f); }

// Symbol: ?DelayRenderData@COleDataSource@@QEAAXGPEAUtagFORMATETC@@@Z
extern "C" void impl__DelayRenderData_COleDataSource__QEAAXGPEAUtagFORMATETC___Z(COleDataSource* p,CLIPFORMAT c,FORMATETC* f) { p->DelayRenderData(c,f); }

// Symbol: ?DUnadvise@XDataObject@COleDataSource@@UEAAJK@Z
extern "C" HRESULT impl__DUnadvise_XDataObject_COleDataSource__UEAAJK_Z(COleDataSource::XDataObject* p,DWORD c) { return p->COleDataSource::XDataObject::DUnadvise(c); }

// Symbol: ?DAdvise@XDataObject@COleDataSource@@UEAAJPEAUtagFORMATETC@@KPEAUIAdviseSink@@PEAK@Z
extern "C" HRESULT impl__DAdvise_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__KPEAUIAdviseSink__PEAK_Z(COleDataSource::XDataObject* p,FORMATETC* f,DWORD d,IAdviseSink* a,DWORD* c) { return p->COleDataSource::XDataObject::DAdvise(f,d,a,c); }

// Symbol: ?CacheGlobalData@COleDataSource@@QEAAXGPEAXPEAUtagFORMATETC@@@Z
extern "C" void impl__CacheGlobalData_COleDataSource__QEAAXGPEAXPEAUtagFORMATETC___Z(COleDataSource* p,CLIPFORMAT c,HGLOBAL h,FORMATETC* f) { p->CacheGlobalData(c,h,f); }

// Symbol: ?CacheData@COleDataSource@@QEAAXGPEAUtagSTGMEDIUM@@PEAUtagFORMATETC@@@Z
extern "C" void impl__CacheData_COleDataSource__QEAAXGPEAUtagSTGMEDIUM__PEAUtagFORMATETC___Z(COleDataSource* p,CLIPFORMAT c,STGMEDIUM* v,FORMATETC* f) { p->CacheData(c,v,f); }

// Symbol: ??1COleDataSource@@UEAA@XZ
extern "C" void impl___1COleDataSource__UEAA_XZ(COleDataSource* p) { p->COleDataSource::~COleDataSource(); }

// Symbol: ??0COleDataSource@@QEAA@XZ
extern "C" COleDataSource* impl___0COleDataSource__QEAA_XZ(void* p) { return new(p) COleDataSource; }
