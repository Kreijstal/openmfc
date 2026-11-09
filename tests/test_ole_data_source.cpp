#define OPENMFC_TEST_REAL_NOTSUPPORTED_EXCEPTION
#define OPENMFC_TEST_WINDOW_HANDLES
#define OPENMFC_TEST_REAL_MODULE_THREAD_STATE
#define OPENMFC_TEST_REAL_GLOBAL_LOCKS
#include "detail/D2DTextSupport.h"
#include "detail/GdicoreSupport.h"
#include "d2d_test_support.h"
#include "detail/OleDataSourceSupport.h"
#include "detail/OleProcessStateSupport.h"
#include "detail/CThreadLocalObjectSupport.h"
#include "detail/InterfaceMapsSupport.h"
#include "detail/OleDropSourceSupport.h"
#include "detail/ModuleThreadStateSupport.h"
#include <array>
#include <vector>
#include <string>
#include <cstdio>
#include <cstring>
#include <climits>
// Unrelated exception descriptors/diagnostics and the file-exception factory
// remain boundaries. All data-source, cache, COM, file and state owners are real.
int CFileException::GetErrorMessage(wchar_t*,UINT,UINT*) const {std::abort();}
extern "C" void impl__AfxThrowFileException__YAXHJPEB_W_Z(int cause,long os,const wchar_t* name){auto* e=new CFileException(cause,os);if(name)e->m_strFileName=name;throw e;}
extern "C" HRESULT impl__SetData_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z(COleDataSource::XDataObject* p,FORMATETC* f,STGMEDIUM* v,BOOL r);
extern "C" void impl__SetClipboard_COleDataSource__QEAAXXZ(COleDataSource* p);
extern "C" HRESULT impl__QueryGetData_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC___Z(COleDataSource::XDataObject* p,FORMATETC* f);
extern "C" BOOL impl__OnSetData_COleDataSource__UEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z(COleDataSource* p,FORMATETC* f,STGMEDIUM* v,BOOL r);
extern "C" BOOL impl__OnRenderGlobalData_COleDataSource__UEAAHPEAUtagFORMATETC__PEAPEAX_Z(COleDataSource* p,FORMATETC* f,HGLOBAL* h);
extern "C" BOOL impl__OnRenderFileData_COleDataSource__UEAAHPEAUtagFORMATETC__PEAVCFile___Z(COleDataSource* p,FORMATETC* f,CFile* v);
extern "C" BOOL impl__OnRenderData_COleDataSource__UEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(COleDataSource* p,FORMATETC* f,STGMEDIUM* v);
extern "C" AFX_DATACACHE_ENTRY* impl__Lookup_COleDataSource__IEBAPEAUAFX_DATACACHE_ENTRY__PEAUtagFORMATETC__W4tagDATADIR___Z(const COleDataSource* p,FORMATETC* f,DATADIR d);
extern "C" const AFX_INTERFACEMAP* impl__GetThisInterfaceMap_COleDataSource__KAPEBUAFX_INTERFACEMAP__XZ();
extern "C" const AFX_INTERFACEMAP* impl__GetInterfaceMap_COleDataSource__MEBAPEBUAFX_INTERFACEMAP__XZ(const COleDataSource*);
extern "C" HRESULT impl__GetDataHere_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(COleDataSource::XDataObject* p,FORMATETC* f,STGMEDIUM* v);
extern "C" HRESULT impl__GetData_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(COleDataSource::XDataObject* p,FORMATETC* f,STGMEDIUM* v);
extern "C" COleDataSource* impl__GetClipboardOwner_COleDataSource__SAPEAV1_XZ();
extern "C" HRESULT impl__GetCanonicalFormatEtc_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__0_Z(COleDataSource::XDataObject* p,FORMATETC* f,FORMATETC* out);
extern "C" AFX_DATACACHE_ENTRY* impl__GetCacheEntry_COleDataSource__IEAAPEAUAFX_DATACACHE_ENTRY__PEAUtagFORMATETC__W4tagDATADIR___Z(COleDataSource* p,FORMATETC* f,DATADIR d);
extern "C" void impl__FlushClipboard_COleDataSource__SAXXZ();
extern "C" HRESULT impl__EnumFormatEtc_XDataObject_COleDataSource__UEAAJKPEAPEAUIEnumFORMATETC___Z(COleDataSource::XDataObject* p,DWORD d,IEnumFORMATETC** out);
extern "C" HRESULT impl__EnumDAdvise_XDataObject_COleDataSource__UEAAJPEAPEAUIEnumSTATDATA___Z(COleDataSource::XDataObject* p,IEnumSTATDATA** out);
extern "C" void impl__Empty_COleDataSource__QEAAXXZ(COleDataSource* p);
extern "C" DROPEFFECT impl__DoDragDrop_COleDataSource__QEAAKKPEBUtagRECT__PEAVCOleDropSource___Z(COleDataSource* p,DWORD e,LPCRECT r,COleDropSource* d);
extern "C" void impl__DelaySetData_COleDataSource__QEAAXGPEAUtagFORMATETC___Z(COleDataSource* p,CLIPFORMAT c,FORMATETC* f);
extern "C" void impl__DelayRenderFileData_COleDataSource__QEAAXGPEAUtagFORMATETC___Z(COleDataSource* p,CLIPFORMAT c,FORMATETC* f);
extern "C" void impl__DelayRenderData_COleDataSource__QEAAXGPEAUtagFORMATETC___Z(COleDataSource* p,CLIPFORMAT c,FORMATETC* f);
extern "C" HRESULT impl__DUnadvise_XDataObject_COleDataSource__UEAAJK_Z(COleDataSource::XDataObject* p,DWORD c);
extern "C" HRESULT impl__DAdvise_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__KPEAUIAdviseSink__PEAK_Z(COleDataSource::XDataObject* p,FORMATETC* f,DWORD d,IAdviseSink* a,DWORD* c);
extern "C" void impl__CacheGlobalData_COleDataSource__QEAAXGPEAXPEAUtagFORMATETC___Z(COleDataSource* p,CLIPFORMAT c,HGLOBAL h,FORMATETC* f);
extern "C" void impl__CacheData_COleDataSource__QEAAXGPEAUtagSTGMEDIUM__PEAUtagFORMATETC___Z(COleDataSource* p,CLIPFORMAT c,STGMEDIUM* v,FORMATETC* f);
extern "C" void impl___1COleDataSource__UEAA_XZ(COleDataSource* p);
extern "C" COleDataSource* impl___0COleDataSource__QEAA_XZ(void* p);
extern "C" CRuntimeClass* impl__GetThisClass_COleException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ();
extern "C" long impl__Process_COleException__SAJPEBVCException___Z(const CException*);

extern "C" void* impl___0COleDropSource__QEAA_XZ(void*);
extern "C" void impl___1CCmdTarget__UEAA_XZ(CCmdTarget*);
extern "C" CRuntimeClass* impl__GetThisClass_CNotSupportedException__SAPEAUCRuntimeClass__XZ();
struct API {
    decltype(&impl__SetData_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z) comSetData;
    decltype(&impl__SetClipboard_COleDataSource__QEAAXXZ) SetClipboard;
    decltype(&impl__QueryGetData_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC___Z) comQueryGetData;
    decltype(&impl__OnSetData_COleDataSource__UEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z) OnSetData;
    decltype(&impl__OnRenderGlobalData_COleDataSource__UEAAHPEAUtagFORMATETC__PEAPEAX_Z) OnRenderGlobalData;
    decltype(&impl__OnRenderFileData_COleDataSource__UEAAHPEAUtagFORMATETC__PEAVCFile___Z) OnRenderFileData;
    decltype(&impl__OnRenderData_COleDataSource__UEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z) OnRenderData;
    decltype(&impl__Lookup_COleDataSource__IEBAPEAUAFX_DATACACHE_ENTRY__PEAUtagFORMATETC__W4tagDATADIR___Z) Lookup;
    decltype(&impl__GetThisInterfaceMap_COleDataSource__KAPEBUAFX_INTERFACEMAP__XZ) GetThisInterfaceMap;
    decltype(&impl__GetInterfaceMap_COleDataSource__MEBAPEBUAFX_INTERFACEMAP__XZ) GetInterfaceMap;
    decltype(&impl__GetDataHere_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z) comGetDataHere;
    decltype(&impl__GetData_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z) comGetData;
    decltype(&impl__GetClipboardOwner_COleDataSource__SAPEAV1_XZ) GetClipboardOwner;
    decltype(&impl__GetCanonicalFormatEtc_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__0_Z) comGetCanonicalFormatEtc;
    decltype(&impl__GetCacheEntry_COleDataSource__IEAAPEAUAFX_DATACACHE_ENTRY__PEAUtagFORMATETC__W4tagDATADIR___Z) GetCacheEntry;
    decltype(&impl__FlushClipboard_COleDataSource__SAXXZ) FlushClipboard;
    decltype(&impl__EnumFormatEtc_XDataObject_COleDataSource__UEAAJKPEAPEAUIEnumFORMATETC___Z) comEnumFormatEtc;
    decltype(&impl__EnumDAdvise_XDataObject_COleDataSource__UEAAJPEAPEAUIEnumSTATDATA___Z) comEnumDAdvise;
    decltype(&impl__Empty_COleDataSource__QEAAXXZ) Empty;
    decltype(&impl__DoDragDrop_COleDataSource__QEAAKKPEBUtagRECT__PEAVCOleDropSource___Z) DoDragDrop;
    decltype(&impl__DelaySetData_COleDataSource__QEAAXGPEAUtagFORMATETC___Z) DelaySetData;
    decltype(&impl__DelayRenderFileData_COleDataSource__QEAAXGPEAUtagFORMATETC___Z) DelayRenderFileData;
    decltype(&impl__DelayRenderData_COleDataSource__QEAAXGPEAUtagFORMATETC___Z) DelayRenderData;
    decltype(&impl__DUnadvise_XDataObject_COleDataSource__UEAAJK_Z) comDUnadvise;
    decltype(&impl__DAdvise_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__KPEAUIAdviseSink__PEAK_Z) comDAdvise;
    decltype(&impl__CacheGlobalData_COleDataSource__QEAAXGPEAXPEAUtagFORMATETC___Z) CacheGlobalData;
    decltype(&impl__CacheData_COleDataSource__QEAAXGPEAUtagSTGMEDIUM__PEAUtagFORMATETC___Z) CacheData;
    decltype(&impl___1COleDataSource__UEAA_XZ) dtor;
    decltype(&impl___0COleDataSource__QEAA_XZ) ctor;
    decltype(&impl__GetThisClass_COleException__SAPEAUCRuntimeClass__XZ) oleType;
    decltype(&impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ) memoryType;
    decltype(&impl__Process_COleException__SAJPEBVCException___Z) process;
    _AFX_THREAD_STATE* (*thread)();
    decltype(&impl___1CThreadLocalObject__QEAA_XZ) destroyLocal;
    decltype(&impl___0COleDropSource__QEAA_XZ) dropCtor;
    decltype(&impl___1CCmdTarget__UEAA_XZ) dropDtor;
    AFX_MODULE_THREAD_STATE* (*state)();
    UINT* distance;
    decltype(&impl__GetThisClass_CNotSupportedException__SAPEAUCRuntimeClass__XZ) unsupportedType;
    HMODULE image;
};
API sourceAPI(){API result{};result.unsupportedType=impl__GetThisClass_CNotSupportedException__SAPEAUCRuntimeClass__XZ;
    result.comSetData=&impl__SetData_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z;
    result.SetClipboard=&impl__SetClipboard_COleDataSource__QEAAXXZ;
    result.comQueryGetData=&impl__QueryGetData_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC___Z;
    result.OnSetData=&impl__OnSetData_COleDataSource__UEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z;
    result.OnRenderGlobalData=&impl__OnRenderGlobalData_COleDataSource__UEAAHPEAUtagFORMATETC__PEAPEAX_Z;
    result.OnRenderFileData=&impl__OnRenderFileData_COleDataSource__UEAAHPEAUtagFORMATETC__PEAVCFile___Z;
    result.OnRenderData=&impl__OnRenderData_COleDataSource__UEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z;
    result.Lookup=&impl__Lookup_COleDataSource__IEBAPEAUAFX_DATACACHE_ENTRY__PEAUtagFORMATETC__W4tagDATADIR___Z;
    result.GetThisInterfaceMap=&impl__GetThisInterfaceMap_COleDataSource__KAPEBUAFX_INTERFACEMAP__XZ;
    result.GetInterfaceMap=&impl__GetInterfaceMap_COleDataSource__MEBAPEBUAFX_INTERFACEMAP__XZ;
    result.comGetDataHere=&impl__GetDataHere_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z;
    result.comGetData=&impl__GetData_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z;
    result.GetClipboardOwner=&impl__GetClipboardOwner_COleDataSource__SAPEAV1_XZ;
    result.comGetCanonicalFormatEtc=&impl__GetCanonicalFormatEtc_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__0_Z;
    result.GetCacheEntry=&impl__GetCacheEntry_COleDataSource__IEAAPEAUAFX_DATACACHE_ENTRY__PEAUtagFORMATETC__W4tagDATADIR___Z;
    result.FlushClipboard=&impl__FlushClipboard_COleDataSource__SAXXZ;
    result.comEnumFormatEtc=&impl__EnumFormatEtc_XDataObject_COleDataSource__UEAAJKPEAPEAUIEnumFORMATETC___Z;
    result.comEnumDAdvise=&impl__EnumDAdvise_XDataObject_COleDataSource__UEAAJPEAPEAUIEnumSTATDATA___Z;
    result.Empty=&impl__Empty_COleDataSource__QEAAXXZ;
    result.DoDragDrop=&impl__DoDragDrop_COleDataSource__QEAAKKPEBUtagRECT__PEAVCOleDropSource___Z;
    result.DelaySetData=&impl__DelaySetData_COleDataSource__QEAAXGPEAUtagFORMATETC___Z;
    result.DelayRenderFileData=&impl__DelayRenderFileData_COleDataSource__QEAAXGPEAUtagFORMATETC___Z;
    result.DelayRenderData=&impl__DelayRenderData_COleDataSource__QEAAXGPEAUtagFORMATETC___Z;
    result.comDUnadvise=&impl__DUnadvise_XDataObject_COleDataSource__UEAAJK_Z;
    result.comDAdvise=&impl__DAdvise_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__KPEAUIAdviseSink__PEAK_Z;
    result.CacheGlobalData=&impl__CacheGlobalData_COleDataSource__QEAAXGPEAXPEAUtagFORMATETC___Z;
    result.CacheData=&impl__CacheData_COleDataSource__QEAAXGPEAUtagSTGMEDIUM__PEAUtagFORMATETC___Z;
    result.dtor=&impl___1COleDataSource__UEAA_XZ;
    result.ctor=&impl___0COleDataSource__QEAA_XZ;
    result.oleType=&impl__GetThisClass_COleException__SAPEAUCRuntimeClass__XZ;
    result.memoryType=&impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ;
    result.process=&impl__Process_COleException__SAJPEBVCException___Z;
    result.dropCtor=impl___0COleDropSource__QEAA_XZ;result.dropDtor=impl___1CCmdTarget__UEAA_XZ;result.state=impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ;result.distance=&openmfc::detail::OleDropSourceAccess::Distance();result.thread=AfxGetThreadState;result.destroyLocal=impl___1CThreadLocalObject__QEAA_XZ;return result;
}
API nativeAPI(HMODULE image){API result{};auto* base=reinterpret_cast<BYTE*>(image);result.image=image;result.unsupportedType=reinterpret_cast<decltype(result.unsupportedType)>(base+2258384);
    result.comSetData=reinterpret_cast<decltype(result.comSetData)>(base+0x253c80);
    result.SetClipboard=reinterpret_cast<decltype(result.SetClipboard)>(base+0x2530b0);
    result.comQueryGetData=reinterpret_cast<decltype(result.comQueryGetData)>(base+0x253c40);
    result.OnSetData=reinterpret_cast<decltype(result.OnSetData)>(base+0x71e0);
    result.OnRenderGlobalData=reinterpret_cast<decltype(result.OnRenderGlobalData)>(base+0x71e0);
    result.OnRenderFileData=reinterpret_cast<decltype(result.OnRenderFileData)>(base+0x71e0);
    result.OnRenderData=reinterpret_cast<decltype(result.OnRenderData)>(base+0x2535a0);
    result.Lookup=reinterpret_cast<decltype(result.Lookup)>(base+0x2534e0);
    result.GetThisInterfaceMap=reinterpret_cast<decltype(result.GetThisInterfaceMap)>(base+0x2539a0);
    result.GetInterfaceMap=reinterpret_cast<decltype(result.GetInterfaceMap)>(base+0x2539a0);
    result.comGetDataHere=reinterpret_cast<decltype(result.comGetDataHere)>(base+0x253b30);
    result.comGetData=reinterpret_cast<decltype(result.comGetData)>(base+0x253a20);
    result.GetClipboardOwner=reinterpret_cast<decltype(result.GetClipboardOwner)>(base+0x253130);
    result.comGetCanonicalFormatEtc=reinterpret_cast<decltype(result.comGetCanonicalFormatEtc)>(base+0x253c70);
    result.GetCacheEntry=reinterpret_cast<decltype(result.GetCacheEntry)>(base+0x2531a0);
    result.FlushClipboard=reinterpret_cast<decltype(result.FlushClipboard)>(base+0x253110);
    result.comEnumFormatEtc=reinterpret_cast<decltype(result.comEnumFormatEtc)>(base+0x253d50);
    result.comEnumDAdvise=reinterpret_cast<decltype(result.comEnumDAdvise)>(base+0x253e80);
    result.Empty=reinterpret_cast<decltype(result.Empty)>(base+0x253030);
    result.DoDragDrop=reinterpret_cast<decltype(result.DoDragDrop)>(base+0x25a690);
    result.DelaySetData=reinterpret_cast<decltype(result.DelaySetData)>(base+0x253480);
    result.DelayRenderFileData=reinterpret_cast<decltype(result.DelayRenderFileData)>(base+0x2533b0);
    result.DelayRenderData=reinterpret_cast<decltype(result.DelayRenderData)>(base+0x253410);
    result.comDUnadvise=reinterpret_cast<decltype(result.comDUnadvise)>(base+0x253e70);
    result.comDAdvise=reinterpret_cast<decltype(result.comDAdvise)>(base+0x253e50);
    result.CacheGlobalData=reinterpret_cast<decltype(result.CacheGlobalData)>(base+0x2532d0);
    result.CacheData=reinterpret_cast<decltype(result.CacheData)>(base+0x253340);
    result.dtor=reinterpret_cast<decltype(result.dtor)>(base+0x252fd0);
    result.ctor=reinterpret_cast<decltype(result.ctor)>(base+0x252f90);
    result.oleType=reinterpret_cast<decltype(result.oleType)>(base+0x260870);
    result.memoryType=reinterpret_cast<decltype(result.memoryType)>(base+0x2275c0);
    result.process=reinterpret_cast<decltype(result.process)>(base+0x25f3b0);
    result.dropCtor=reinterpret_cast<decltype(result.dropCtor)>(base+0x25a350);result.dropDtor=reinterpret_cast<decltype(result.dropDtor)>(base+0x1de430);result.state=reinterpret_cast<decltype(result.state)>(base+0x133a20);result.distance=reinterpret_cast<UINT*>(base+0x3be41c);result.thread=reinterpret_cast<decltype(result.thread)>(base+0x1332a0);result.destroyLocal=reinterpret_cast<decltype(result.destroyLocal)>(base+0x14d0d0);return result;
}
const API* api;
int failures;
void check(bool ok,const char* message){if(!ok){++failures;std::printf("FAIL: %s\n",message);}}
alignas(16) BYTE modules[3][0x110]{};
void select(unsigned n){api->thread()->m_pModuleState=reinterpret_cast<AFX_MODULE_STATE*>(modules[n]);}
template<class T>T& field(void* p,size_t off){return *reinterpret_cast<T*>(static_cast<BYTE*>(p)+off);}
struct Source {
    alignas(8) BYTE bytes[96];COleDataSource* p;
    Source(){memset(bytes,0xa5,sizeof(bytes));p=api->ctor(bytes);}
    ~Source(){api->dtor(p);}
    IDataObject* object(){return reinterpret_cast<IDataObject*>(bytes+88);}
};
void smoke(){
    Source source;auto* p=source.p;
    check(sizeof(COleDataSource)==96 && field<void*>(p,64)==nullptr && field<UINT>(p,72)==0 && field<UINT>(p,76)==0 && field<UINT>(p,80)==10 && field<DWORD>(p,84)==0xa5a5a5a5,"actual data-source constructor fields and untouched padding");
    check((api->image ? !strcmp(p->GetRuntimeClass()->m_lpszClassName,"CCmdTarget") : p->GetRuntimeClass()==CCmdTarget::GetThisClass()),"data source inherits the native command-target runtime class");
    FORMATETC format{};format.cfFormat=CF_TEXT;format.dwAspect=DVASPECT_CONTENT;format.lindex=-1;format.tymed=TYMED_HGLOBAL;
    api->DelayRenderData(p,CF_TEXT,&format);check(field<UINT>(p,76)==1&&field<UINT>(p,72)==10,"delayed format creates a native cache entry");
    check(source.object()->QueryGetData(&format)==S_OK,"actual embedded IDataObject dispatches cache lookup");
}

// Only external callers and OS/COM services are replaced below. The source,
// cache, COM part, enumeration, files, process holder and module scopes are real.
void** imported(HMODULE image,const char* wanted){
    auto* b=reinterpret_cast<BYTE*>(image);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);
    auto* d=reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(b+pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
    for(;d->Name;++d){auto* n=reinterpret_cast<IMAGE_THUNK_DATA*>(b+d->OriginalFirstThunk);auto* v=reinterpret_cast<IMAGE_THUNK_DATA*>(b+d->FirstThunk);
        for(;n->u1.AddressOfData;++n,++v)if(!IMAGE_SNAP_BY_ORDINAL(n->u1.Ordinal)){
            auto* name=reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(b+n->u1.AddressOfData);
            if(!strcmp(reinterpret_cast<char*>(name->Name),wanted))return reinterpret_cast<void**>(&v->u1.Function);
        }
    }return nullptr;
}
struct Patch {
    void** slot;void* old;DWORD protection;
    Patch(const char* name,void* fn){slot=imported(api->image?api->image:GetModuleHandleW(nullptr),name);check(slot!=nullptr,name);if(!slot)abort();old=*slot;VirtualProtect(slot,8,PAGE_READWRITE,&protection);*slot=fn;}
    ~Patch(){*slot=old;DWORD ignored;VirtualProtect(slot,8,protection,&ignored);}
};
FORMATETC format(CLIPFORMAT cf=CF_TEXT,DWORD medium=TYMED_HGLOBAL,LONG index=-1,DWORD aspect=DVASPECT_CONTENT){return FORMATETC{cf,nullptr,aspect,index,medium};}
AFX_DATACACHE_ENTRY* cache(Source& s){return field<AFX_DATACACHE_ENTRY*>(s.p,64);}
DVTARGETDEVICE* device(BYTE id){auto* p=static_cast<DVTARGETDEVICE*>(CoTaskMemAlloc(32));memset(p,id,32);p->tdSize=32;return p;}
HGLOBAL global(const char* bytes="hello",SIZE_T n=6){auto h=GlobalAlloc(GMEM_MOVEABLE,n);auto* p=GlobalLock(h);if(n)memcpy(p,bytes,(std::min)(n,SIZE_T(6)));GlobalUnlock(h);return h;}
struct ClientUnknown:IUnknown {
    ULONG refs=1;int queries=0,adds=0,releases=0;void* result=nullptr;
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID,void** out)override{++queries;*out=result;return result?S_OK:E_NOINTERFACE;}
    ULONG STDMETHODCALLTYPE AddRef()override{++adds;return ++refs;}
    ULONG STDMETHODCALLTYPE Release()override{++releases;return --refs;}
};
void identity(){
    Source s;auto* obj=s.object();auto* map=api->GetInterfaceMap(s.p);
    check(map==api->GetThisInterfaceMap() && map->pEntries[0].piid && IsEqualIID(*static_cast<const IID*>(map->pEntries[0].piid),IID_IDataObject) && map->pEntries[0].nOffset==88 && !map->pEntries[1].piid && map->pEntries[1].nOffset==SIZE_MAX,"real interface map owns the embedded IDataObject at 88");
    check(s.p->m_dwRef==1&&s.p->m_pModuleState==reinterpret_cast<AFX_MODULE_STATE*>(modules[0]),"native command-target reference count and module");
    for(auto& iid:{IID_IUnknown,IID_IDataObject}){void* out=nullptr;check(obj->QueryInterface(iid,&out)==S_OK&&out==obj&&s.p->m_dwRef==2&&obj->Release()==1,"COM identity shares owner reference count");}
    GUID absent={12,34,56,{78}};void* out=reinterpret_cast<void*>(1);check(obj->QueryInterface(absent,&out)==E_NOINTERFACE&&!out,"QI clears unsupported interfaces");
    ClientUnknown outer;outer.result=&outer;s.p->m_pOuterUnknown=&outer;
    check(obj->AddRef()==2&&obj->Release()==1&&s.p->m_dwRef==1,"aggregation delegates external references");out=nullptr;
    check(obj->QueryInterface(absent,&out)==S_OK&&out==&outer&&outer.queries==1,"aggregation delegates external identity");s.p->m_pOuterUnknown=nullptr;
    FORMATETC f=format(),output;memset(&output,0xa5,sizeof(output));auto saved=output;
    check(obj->GetCanonicalFormatEtc(nullptr,&output)==DATA_S_SAMEFORMATETC&&!memcmp(&output,&saved,sizeof(output)),"canonical-format constant leaves output untouched");
    DWORD connection=73;IEnumSTATDATA* advice=reinterpret_cast<IEnumSTATDATA*>(1);
    check(obj->DAdvise(&f,0,nullptr,&connection)==OLE_E_ADVISENOTSUPPORTED&&!connection&&obj->DUnadvise(7)==OLE_E_ADVISENOTSUPPORTED&&obj->EnumDAdvise(&advice)==OLE_E_ADVISENOTSUPPORTED&&!advice,"unsupported advisory methods have native output effects");
}
void cacheRules(){
    Source s;auto f=format(CF_UNICODETEXT,TYMED_FILE,19);auto h=global();api->CacheGlobalData(s.p,CF_TEXT,h,&f);
    check(f.cfFormat==CF_UNICODETEXT&&f.tymed==TYMED_HGLOBAL&&cache(s)[0].m_stgMedium.hGlobal==h&&cache(s)[0].m_formatEtc.lindex==19,"CacheGlobalData steals the handle, preserves explicit format and mutates tymed");
    check(api->Lookup(s.p,&f,DATADIR_SET)==nullptr,"lookup requires the exact data direction");
    auto set=f;api->DelaySetData(s.p,CF_TEXT,&set);check(set.cfFormat==CF_UNICODETEXT&&set.tymed==TYMED_HGLOBAL&&field<UINT>(s.p,76)==2,"SET cache is separate and explicit format survives");
    auto delay=format(CF_UNICODETEXT,TYMED_FILE);api->DelayRenderData(s.p,CF_TEXT,&delay);check(delay.cfFormat==CF_TEXT&&delay.tymed==TYMED_FILE,"DelayRenderData overrides explicit format ID but preserves medium mask");
    auto file=format(CF_UNICODETEXT,TYMED_FILE);api->DelayRenderFileData(s.p,CF_TEXT,&file);check(file.cfFormat==CF_UNICODETEXT&&file.tymed==(TYMED_HGLOBAL|TYMED_ISTREAM),"DelayRenderFileData preserves explicit ID and forces both file media");
    api->Empty(s.p);check(!cache(s)&&!field<UINT>(s.p,72)&&!field<UINT>(s.p,76)&&field<UINT>(s.p,80)==10,"Empty releases cache and preserves grow-by");
    api->DelaySetData(s.p,CF_TEXT,nullptr);check(cache(s)[0].m_formatEtc.tymed==UINT_MAX&&cache(s)[0].m_formatEtc.lindex==-1&&cache(s)[0].m_nDataDir==DATADIR_SET,"default SET format accepts every medium");api->Empty(s.p);
    for(UINT i=0;i<31;++i){auto f=format(CLIPFORMAT(0xc000+i));api->DelayRenderData(s.p,0,&f);check(field<UINT>(s.p,76)==i+1&&field<UINT>(s.p,72)==10*(1+i/10),"cache grows by ten native 64-byte entries");}
    api->Empty(s.p);field<UINT>(s.p,72)=8;field<UINT>(s.p,76)=3;api->Empty(s.p);check(field<UINT>(s.p,72)==8&&field<UINT>(s.p,76)==3,"Empty with a null cache leaves counts unchanged");field<UINT>(s.p,72)=field<UINT>(s.p,76)=0;
    ClientUnknown release;STGMEDIUM medium{};medium.tymed=TYMED_HGLOBAL;medium.hGlobal=global();medium.pUnkForRelease=&release;auto original=medium;auto f2=format();
    api->CacheData(s.p,CF_UNICODETEXT,&medium,&f2);check(!memcmp(&original,&medium,sizeof(medium))&&cache(s)[0].m_stgMedium.pUnkForRelease==&release&&!release.adds,"CacheData transfers the raw medium without modifying it or AddRef");
    auto old=medium.hGlobal;api->DelayRenderData(s.p,0,&f2);check(release.releases==1&&field<UINT>(s.p,76)==1&&cache(s)[0].m_stgMedium.tymed==TYMED_NULL,"replacing a cache entry releases its previous medium");GlobalFree(old);
}
void lookups(){
    Source s;auto f=format();
    // Build distinct IDs first, then vary each native record: GetCacheEntry's
    // own near-match replacement would otherwise intentionally coalesce them.
    for(int i=0;i<3;++i){auto q=format(CLIPFORMAT(CF_TEXT+i));api->DelayRenderData(s.p,0,&q);}
    for(int i=0;i<3;++i){cache(s)[i].m_formatEtc=f;cache(s)[i].m_formatEtc.ptd=device(BYTE(i+1));}
    auto* wanted=device(2);f.ptd=wanted;check(api->Lookup(s.p,&f,DATADIR_GET)==cache(s)+1,"lookup returns first exact target-device match");
    memset(wanted->tdData,9,8);check(api->Lookup(s.p,&f,DATADIR_GET)==cache(s)+2,"lookup returns last near target-device match");CoTaskMemFree(wanted);f.ptd=nullptr;
    check(api->Lookup(s.p,&f,DATADIR_GET)==cache(s)+2,"null requested target is a near match for device-specific data");
    auto& e=cache(s)[0];for(int i=1;i<3;++i)cache(s)[i].m_formatEtc.cfFormat=CF_UNICODETEXT;
    e.m_formatEtc.lindex=42;f.lindex=99;check(api->Lookup(s.p,&f,DATADIR_GET)==&e,"delayed entries ignore index");
    e.m_stgMedium.tymed=TYMED_HGLOBAL;e.m_stgMedium.hGlobal=global();check(!api->Lookup(s.p,&f,DATADIR_GET),"cached data filters unequal non-default indices");
    for(auto index:{-1L,0L}){e.m_formatEtc.lindex=index;f.lindex=index?0:-1;check(api->Lookup(s.p,&f,DATADIR_GET)==&e,"cached index zero and minus one are equivalent");}
    for(auto aspect:{DWORD(DVASPECT_ICON),DWORD(DVASPECT_THUMBNAIL)}){e.m_formatEtc.dwAspect=f.dwAspect=aspect;e.m_formatEtc.lindex=41;f.lindex=63;check(api->Lookup(s.p,&f,DATADIR_GET)==&e,"icon and thumbnail data ignore index");}
    f.tymed=TYMED_ISTREAM;check(!api->Lookup(s.p,&f,DATADIR_GET),"lookup intersects medium masks");
}
int destroyed;struct Error:CException {CRuntimeClass* type;LONG status;Error(BOOL own,CRuntimeClass* t=nullptr):CException(own),type(t),status(E_ACCESSDENIED){}~Error()override{++destroyed;}CRuntimeClass* GetRuntimeClass()const override{return type?type:CException::GetThisClass();}};
Error* lastError;int callbackMode,callbackCount;BOOL errorOwned;FORMATETC* expectedFormat;STGMEDIUM* expectedMedium;BOOL expectedRelease;
void callbackState(COleDataSource* p){check(api->thread()->m_pModuleState==reinterpret_cast<AFX_MODULE_STATE*>(modules[0]),"COM callback enters owner's module");p->m_pModuleState=reinterpret_cast<AFX_MODULE_STATE*>(modules[2]);select(2);++callbackCount;}
void throwCallback(){if(callbackMode==2){lastError=new Error(errorOwned);throw static_cast<CException*>(lastError);}if(callbackMode==3)throw 73;}
BOOL renderCallback(COleDataSource* p,FORMATETC* f,STGMEDIUM* m){callbackState(p);check(f==expectedFormat&&m==expectedMedium,"render callback receives original caller pointers");throwCallback();return callbackMode==1;}
BOOL setCallback(COleDataSource* p,FORMATETC* f,STGMEDIUM* m,BOOL release){callbackState(p);check(f==expectedFormat&&m==expectedMedium&&release==expectedRelease,"set callback receives original arguments");throwCallback();return callbackMode==1;}
void callbackBoundaries(){
    for(int op:{0,1,2})for(int mode:{0,1,2,3})for(BOOL own:{FALSE,TRUE}){
        select(0);Source s;auto f=format();api->DelayRenderData(s.p,0,&f);api->DelaySetData(s.p,0,&f);void* root[26];memcpy(root,*reinterpret_cast<void***>(s.p),sizeof(root));root[24]=reinterpret_cast<void*>(&renderCallback);root[25]=reinterpret_cast<void*>(&setCallback);field<void*>(s.p,0)=root;
        STGMEDIUM m{};m.tymed=op==1?TYMED_HGLOBAL:TYMED_NULL;expectedFormat=&f;expectedMedium=&m;expectedRelease=17;callbackMode=mode;callbackCount=destroyed=0;errorOwned=own;lastError=nullptr;select(1);
        HRESULT hr=0;bool escaped=false;try{hr=op==0?s.object()->GetData(&f,&m):op==1?s.object()->GetDataHere(&f,&m):s.object()->SetData(&f,&m,expectedRelease);}catch(int value){escaped=value==73;}
        check(callbackCount==1&&api->thread()->m_pModuleState==reinterpret_cast<AFX_MODULE_STATE*>(modules[1]),"COM restores caller module after success, failure or either exception family");
        check(mode==3?escaped:hr==(mode==1?S_OK:mode==2||op==2?E_UNEXPECTED:DATA_E_FORMATETC),"COM callback HRESULT and exception behavior match native");
        check(destroyed==(mode==2?int(own):0),"COM deletes only auto-delete framework exceptions");if(lastError&&!destroyed)delete lastError;
        select(0);
    }
    Source s;auto f=format();STGMEDIUM m;memset(&m,0xa5,sizeof(m));auto old=m;
    check(s.object()->GetData(&f,&m)==DATA_E_FORMATETC&&!memcmp(&m,&old,sizeof(m)),"cache miss leaves GetData output unchanged");
    check(s.object()->GetData(nullptr,&m)==E_INVALIDARG&&s.object()->GetData(&f,nullptr)==E_INVALIDARG&&s.object()->GetDataHere(nullptr,&m)==E_INVALIDARG&&s.object()->SetData(nullptr,&m,FALSE)==E_INVALIDARG&&s.object()->QueryGetData(nullptr)==E_INVALIDARG,"COM rejects null data arguments");
    m.tymed=TYMED_ISTREAM;check(s.object()->GetDataHere(&f,&m)==DATA_E_FORMATETC&&f.tymed==TYMED_ISTREAM,"GetDataHere updates request mask even on a miss");
    api->DelayRenderData(s.p,0,&f);memset(&m,0xa5,sizeof(m));check(s.object()->GetData(&f,&m)==DATA_E_FORMATETC&&m.tymed==TYMED_NULL&&!m.pstm&&!m.pUnkForRelease,"GetData clears its output before native default rendering fails");
}
void enumeration(){
    Source s;auto f=format();f.ptd=device(4);api->DelayRenderData(s.p,0,&f);auto set=format(CF_UNICODETEXT,TYMED_ISTREAM);api->DelaySetData(s.p,0,&set);
    for(DWORD direction:{0ul,1ul,2ul,3ul,7ul}){
        IEnumFORMATETC* e=reinterpret_cast<IEnumFORMATETC*>(1);select(1);check(s.object()->EnumFormatEtc(direction,&e)==S_OK&&e,"enumeration accepts bitmask directions");
        check(api->thread()->m_pModuleState==reinterpret_cast<AFX_MODULE_STATE*>(modules[1]),"enumeration retains caller module");
        FORMATETC out[3]{};ULONG count=77;auto hr=e->Next(3,out,&count);check(hr==S_FALSE&&count==((direction&1?1u:0u)+(direction&2?1u:0u)),"enumeration filters GET and SET by direction bits");
        for(ULONG i=0;i<count;++i)if(out[i].ptd){check(out[i].ptd!=f.ptd&&!memcmp(out[i].ptd,f.ptd,32),"enumeration copies each owned target device");CoTaskMemFree(out[i].ptd);}e->Release();select(0);
    }
    check(s.object()->EnumFormatEtc(1,nullptr)==E_POINTER,"enumeration rejects a null output pointer");
}
struct ClientStream:IStream {
    ULONG refs=1;std::vector<int> events;HRESULT stat=S_OK,copy=S_OK;ULONGLONG size=0x100000009ull;IStream* copiedTo=nullptr;CException* thrown=nullptr;
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID,void** p)override{*p=nullptr;return E_NOINTERFACE;}
    ULONG STDMETHODCALLTYPE AddRef()override{events.push_back(1);if(thrown)throw thrown;return ++refs;}
    ULONG STDMETHODCALLTYPE Release()override{events.push_back(2);return --refs;}
    HRESULT STDMETHODCALLTYPE Read(void*,ULONG,ULONG*)override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE Write(const void*,ULONG n,ULONG* out)override{events.push_back(7);if(out)*out=n;return S_OK;}
    HRESULT STDMETHODCALLTYPE Seek(LARGE_INTEGER n,DWORD origin,ULARGE_INTEGER* out)override{events.push_back(4);check(!n.QuadPart&&origin==STREAM_SEEK_SET&&!out,"medium copy seeks to zero without result pointer");return E_FAIL;}
    HRESULT STDMETHODCALLTYPE SetSize(ULARGE_INTEGER)override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE CopyTo(IStream* to,ULARGE_INTEGER n,ULARGE_INTEGER* a,ULARGE_INTEGER* b)override{events.push_back(5);check(n.QuadPart==size&&!a&&!b,"medium copy preserves full 64-bit length and null counts");copiedTo=to;return copy;}
    HRESULT STDMETHODCALLTYPE Commit(DWORD)override{return E_NOTIMPL;}HRESULT STDMETHODCALLTYPE Revert()override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE LockRegion(ULARGE_INTEGER,ULARGE_INTEGER,DWORD)override{return E_NOTIMPL;}HRESULT STDMETHODCALLTYPE UnlockRegion(ULARGE_INTEGER,ULARGE_INTEGER,DWORD)override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE Stat(STATSTG* p,DWORD flags)override{events.push_back(3);check(flags==STATFLAG_NONAME,"copy Stat excludes name");p->cbSize.QuadPart=size;return stat;}
    HRESULT STDMETHODCALLTYPE Clone(IStream**)override{return E_NOTIMPL;}
};
void copiedMedia(){
    {Source s;auto f=format();auto h=global();api->CacheGlobalData(s.p,0,h,&f);STGMEDIUM out{};check(s.object()->GetData(&f,&out)==S_OK&&out.tymed==TYMED_HGLOBAL&&out.hGlobal!=h&&!out.pUnkForRelease,"GetData duplicates HGLOBAL rather than transferring cache ownership");auto* p=GlobalLock(out.hGlobal);check(!memcmp(p,"hello",6),"HGLOBAL duplicate preserves bytes");GlobalUnlock(out.hGlobal);ReleaseStgMedium(&out);
     for(SIZE_T size:{SIZE_T(2),SIZE_T(16)}){out={};out.tymed=TYMED_HGLOBAL;out.hGlobal=GlobalAlloc(GMEM_MOVEABLE,size);auto original=out.hGlobal;check(s.object()->GetDataHere(&f,&out)==(size<6?DATA_E_FORMATETC:S_OK)&&out.hGlobal==original,"GetDataHere observes existing global capacity and handle");GlobalFree(out.hGlobal);}}
    {Source s;ClientStream stream;ClientUnknown proxy;STGMEDIUM in{};in.tymed=TYMED_ISTREAM;in.pstm=&stream;in.pUnkForRelease=&proxy;auto f=format(CF_TEXT,TYMED_ISTREAM);api->CacheData(s.p,0,&in,&f);STGMEDIUM out{};
     check(s.object()->GetData(&f,&out)==S_OK&&out.pstm==&stream&&!out.pUnkForRelease&&stream.refs==2&&stream.events==std::vector<int>{1},"stream GetData AddRefs stream and drops the cache's release proxy");out.pstm->Release();stream.events.clear();
     for(auto result:{S_OK,S_FALSE,E_FAIL}){ClientStream destination;out={};out.tymed=TYMED_ISTREAM;out.pstm=&destination;out.pUnkForRelease=&proxy;stream.copy=result;
       check(s.object()->GetDataHere(&f,&out)==(result==S_OK?S_OK:DATA_E_FORMATETC)&&stream.copiedTo==&destination&&out.pUnkForRelease==&proxy,"stream CopyTo requires exact S_OK and preserves destination release proxy");
       check(stream.events==(result==S_OK?std::vector<int>{3,4,5,4}:std::vector<int>{3,4,5}),"stream copy keeps native Stat/Seek/CopyTo/Seek ordering");
       check(destination.events.size()==(result==S_OK?2:1),"failed CopyTo leaves seek progress at native point");stream.events.clear();}
     stream.stat=S_FALSE;ClientStream destination;out={};out.tymed=TYMED_ISTREAM;out.pstm=&destination;check(s.object()->GetDataHere(&f,&out)==DATA_E_FORMATETC&&stream.events==std::vector<int>{3}&&destination.events.empty(),"Stat requires exact S_OK before any seek");
     stream.events.clear();Error error(FALSE);stream.thrown=&error;bool caught=false;select(1);try{s.object()->GetData(&f,&out);}catch(CException* e){caught=e==&error;}check(caught&&api->thread()->m_pModuleState==reinterpret_cast<AFX_MODULE_STATE*>(modules[1]),"cached medium copy exceptions escape the rendering catch and restore caller module");select(0);stream.thrown=nullptr;
    }
}
std::vector<int> renderEvents;int renderMode;HGLOBAL suppliedGlobal;ClientUnknown renderProxy;
BOOL globalRender(COleDataSource*,FORMATETC*,HGLOBAL* out){renderEvents.push_back(1);*out=suppliedGlobal;return renderMode==1;}
BOOL fileRender(COleDataSource*,FORMATETC*,CFile* file){
    const auto* name=file->GetRuntimeClass()->m_lpszClassName;bool shared=!strcmp(name,"CSharedFile");check(shared||!strcmp(name,"COleStreamFile"),"file rendering receives an actual native file family");renderEvents.push_back(shared?2:3);
    if(renderMode==2||(renderMode==3&&!shared)){file->Write("hello",6);check(file->GetPosition()==6&&file->GetLength()>=6,"file callback dispatches native file write, position and length slots");return TRUE;}return FALSE;
}
void fileRendering(){
    for(int mode:{0,1,2,3})for(DWORD mask:{DWORD(TYMED_HGLOBAL),DWORD(TYMED_ISTREAM),DWORD(TYMED_HGLOBAL|TYMED_ISTREAM)}){
        Source s;auto f=format(CF_TEXT,mask);void* root[26];memcpy(root,*reinterpret_cast<void***>(s.p),sizeof(root));root[22]=reinterpret_cast<void*>(&globalRender);root[23]=reinterpret_cast<void*>(&fileRender);field<void*>(s.p,0)=root;
        STGMEDIUM out{};out.pUnkForRelease=&renderProxy;suppliedGlobal=global();renderMode=mode;renderEvents.clear();BOOL answer=api->OnRenderData(s.p,&f,&out);
        bool success=(mode==1&&(mask&TYMED_HGLOBAL))||mode==2||(mode==3&&(mask&TYMED_ISTREAM));check(bool(answer)==success&&out.pUnkForRelease==&renderProxy,"default renderer follows supported medium priority and preserves release proxy");
        check(renderEvents.front()==(mask&TYMED_HGLOBAL?1:3),"HGLOBAL callback precedes file rendering");
        if(answer){if(out.tymed==TYMED_HGLOBAL){if(out.hGlobal!=suppliedGlobal)GlobalFree(suppliedGlobal);GlobalFree(out.hGlobal);}else{out.pstm->Release();GlobalFree(suppliedGlobal);}}else GlobalFree(suppliedGlobal);
    }
    // Borrowed existing streams must bypass Attach/Stat and retain ownership.
    Source s;auto f=format(CF_TEXT,TYMED_ISTREAM);void* root[26];memcpy(root,*reinterpret_cast<void***>(s.p),sizeof(root));root[23]=reinterpret_cast<void*>(&fileRender);field<void*>(s.p,0)=root;ClientStream stream;STGMEDIUM out{};out.tymed=TYMED_ISTREAM;out.pstm=&stream;renderMode=0;renderEvents.clear();
    check(!api->OnRenderData(s.p,&f,&out)&&stream.refs==1&&stream.events.empty()&&out.pstm==&stream,"failed rendering into an existing stream neither calls Stat nor releases it");
}
COleDataSource*& clipboardOwner(){auto* state=api->image?field<void*>(api->image,0x3c4190):static_cast<void*>(openmfc::detail::GetOleProcessState());return field<COleDataSource*>(state,16);}
IDataObject* clipboardObject;HRESULT currentClipboard=S_OK,setClipboardResult=S_OK;COleDataSource* replacementClipboard;int sets,queries,flushes;
HRESULT WINAPI setClipboard(IDataObject* p){++sets;if(setClipboardResult==S_OK){clipboardObject=p;p->AddRef();}return setClipboardResult;}
HRESULT WINAPI isClipboard(IDataObject* p){++queries;check(p==clipboardObject,"clipboard checks the actual embedded interface");if(replacementClipboard)clipboardOwner()=replacementClipboard;return currentClipboard;}
HRESULT WINAPI flushClipboard(){++flushes;return E_FAIL;}
void clipboard(){
    Patch set("OleSetClipboard",reinterpret_cast<void*>(&setClipboard)),current("OleIsCurrentClipboard",reinterpret_cast<void*>(&isClipboard)),flush("OleFlushClipboard",reinterpret_cast<void*>(&flushClipboard));
    check(api->GetClipboardOwner()==nullptr,"initial process clipboard owner is null");sets=queries=flushes=0;
    {Source s;api->SetClipboard(s.p);check(sets==1&&clipboardObject==s.object()&&clipboardOwner()==s.p&&s.p->m_dwRef==1,"SetClipboard records process owner and transfers the initial reference");
     check(api->GetClipboardOwner()==s.p&&queries==1,"current clipboard returns source owner");
     {Source replacement;replacementClipboard=replacement.p;check(api->GetClipboardOwner()==replacement.p,"clipboard callback can replace process owner before GetClipboardOwner returns");replacementClipboard=nullptr;clipboardOwner()=s.p;}api->Empty(s.p);check(clipboardOwner()==s.p,"Empty leaves clipboard ownership alone");api->FlushClipboard();check(flushes==1&&clipboardOwner()==s.p,"FlushClipboard ignores flush failure and retains owner");currentClipboard=S_FALSE;check(!api->GetClipboardOwner()&&!clipboardOwner(),"clipboard ownership requires exact S_OK");int oldQueries=queries;api->FlushClipboard();check(queries==oldQueries&&flushes==1,"empty clipboard owner avoids OS calls");clipboardOwner()=s.p;}
    check(!clipboardOwner(),"source destructor clears its process clipboard owner");currentClipboard=S_OK;
    for(HRESULT hr:{S_FALSE,E_ACCESSDENIED}){Source s;setClipboardResult=hr;bool caught=false;try{api->SetClipboard(s.p);}catch(CException* e){caught=field<LONG>(e,16)==hr;e->Delete();}check(caught&&!clipboardOwner()&&s.p->m_dwRef==1,"SetClipboard requires exact S_OK and throws OLE status before transferring owner or reference");}setClipboardResult=S_OK;
}

using DropAccess=openmfc::detail::OleDropSourceAccess;
RECT expectedRect;BOOL expectedStarted,beginResult;int beginCalls,dragCalls,metricsCalls;CRITICAL_SECTION* metricsLock;bool recordLock;
IDataObject* dragObject;IDropSource* dragSource;DWORD dragAllowed,dragResult;CWnd* mainWindow;COleDropSource* callbackDrop;
decltype(&EnterCriticalSection) realEnter;
void WINAPI enterMetrics(CRITICAL_SECTION* lock){if(recordLock)metricsLock=lock;else if(lock==metricsLock)++metricsCalls;realEnter(lock);}
BOOL changedBegin(COleDropSource*,CWnd*){check(false,"drag must select OnBeginDrag before the main-window callback changes its root");return FALSE;}
CWnd* threadWindow(CWinThread*){field<void**>(callbackDrop,0)[24]=reinterpret_cast<void*>(&changedBegin);return mainWindow;}
BOOL beginDrag(COleDropSource* p,CWnd* window){++beginCalls;check(window==mainWindow,"drag start uses current thread virtual main-window callback");check(!memcmp(&DropAccess::Rectangle(p),&expectedRect,sizeof(RECT))&&DropAccess::Started(p)==expectedStarted,"drag initializes native rectangle and raw started flag before callback");check(api->thread()->m_pModuleState==reinterpret_cast<AFX_MODULE_STATE*>(modules[1]),"drag callback retains caller module");return beginResult;}
HRESULT WINAPI dragDrop(IDataObject* object,IDropSource* source,DWORD allowed,DWORD* result){++dragCalls;check(object==dragObject&&source==dragSource&&allowed==dragAllowed&&!*result,"OLE drag receives real embedded COM interfaces, allowed effects and zero output");*result=dragResult;return E_FAIL;}
BOOL WINAPI cursor(POINT* p){p->x=40;p->y=50;return FALSE;}
void dragging(){
    Patch enter("EnterCriticalSection",reinterpret_cast<void*>(&enterMetrics)),ole("DoDragDrop",reinterpret_cast<void*>(&dragDrop)),point("GetCursorPos",reinterpret_cast<void*>(&cursor));realEnter=reinterpret_cast<decltype(realEnter)>(enter.old);
    select(0);Source source;alignas(8) BYTE bytes[104];recordLock=true;auto* drop=static_cast<COleDropSource*>(api->dropCtor(bytes));recordLock=false;
    void* root[25];memcpy(root,*reinterpret_cast<void***>(drop),sizeof(root));root[24]=reinterpret_cast<void*>(&beginDrag);field<void*>(drop,0)=root;
    void* threadRoot[32]{};threadRoot[31]=reinterpret_cast<void*>(&threadWindow);void** fakeThreadRoot=threadRoot;
    select(1);auto* state=api->state();auto* previous=state->m_pCurrentWinThread;state->m_pCurrentWinThread=reinterpret_cast<CWinThread*>(&fakeThreadRoot);mainWindow=reinterpret_cast<CWnd*>(0x1234);
    const RECT cases[]={{0,0,0,0},{40,50,40,50},{10,20,30,40},{40,50,30,20}};
    *api->distance=7;
    for(int i=0;i<5;++i)for(BOOL begin:{FALSE,TRUE}){
        RECT rectangle=i==4?RECT{40,50,40,50}:cases[i];expectedRect=rectangle;expectedStarted=i==0?TRUE:FALSE;if(i!=0&&IsRectEmpty(&expectedRect))InflateRect(&expectedRect,7,7);
        root[24]=reinterpret_cast<void*>(&beginDrag);callbackDrop=drop;beginResult=begin;metricsCalls=beginCalls=dragCalls=0;dragObject=source.object();dragSource=reinterpret_cast<IDropSource*>(bytes+64);dragAllowed=0x1234;dragResult=0x4567;
        auto result=api->DoDragDrop(source.p,dragAllowed,i==4?nullptr:&rectangle,drop);
        check(result==(begin?dragResult:DROPEFFECT_NONE)&&beginCalls==1&&dragCalls==int(begin),"drag effect comes from OLE output even when HRESULT fails, with early exit on denied begin");
        check(metricsCalls==1,"default drag source is always constructed even with an explicit source");
    }
    state->m_pCurrentWinThread=previous;api->dropDtor(drop);select(0);
}

void* WINAPI failedTaskAlloc(SIZE_T){return nullptr;}
void enumAllocation(){
    Source s;auto f=format();f.ptd=device(9);api->DelayRenderData(s.p,0,&f);IEnumFORMATETC* e=nullptr;
    {Patch fail("CoTaskMemAlloc",reinterpret_cast<void*>(&failedTaskAlloc));check(s.object()->EnumFormatEtc(1,&e)==S_OK&&e,"enumeration accepts failed target-device copy allocation");}
    FORMATETC out{};ULONG count=0;check(e->Next(1,&out,&count)==S_OK&&count==1&&!out.ptd&&out.cfFormat==CF_TEXT,"failed device duplication becomes a null device in the enumerated format");e->Release();
}
struct ClassifiedError:CException {
    LONG status;CRuntimeClass* type;
    ClassifiedError(CRuntimeClass* t,LONG result):CException(FALSE),status(result),type(t){}
    CRuntimeClass* GetRuntimeClass()const override{return type;}
};
static_assert(offsetof(ClassifiedError,status)==16,"client exception carries native OLE SCODE");
void exceptionClassification(){
    for(int kind:{0,1,2,3}){auto* type=kind==1?api->oleType():kind==2?api->memoryType():kind==3?api->unsupportedType():CException::GetThisClass();ClassifiedError error(type,E_ACCESSDENIED);
        check(api->process(&error)==(kind==1?E_ACCESSDENIED:kind==2?E_OUTOFMEMORY:kind==3?E_NOTIMPL:E_UNEXPECTED),"OLE exception classification uses native runtime ancestry and OLE status");}
}
SIZE_T allocatedSize;UINT allocatedFlags;int locks,unlocks;
SIZE_T WINAPI hugeSize(HGLOBAL){return SIZE_T(0x100000000ull);}
HGLOBAL WINAPI hugeAlloc(UINT flags,SIZE_T size){allocatedFlags=flags;allocatedSize=size;return reinterpret_cast<HGLOBAL>(2);}
void* WINAPI hugeLock(HGLOBAL){++locks;static BYTE buffer[8];return buffer;}
BOOL WINAPI hugeUnlock(HGLOBAL){++unlocks;return FALSE;}
void largeGlobal(){
    Source s;auto f=format();api->CacheGlobalData(s.p,0,reinterpret_cast<HGLOBAL>(1),&f);STGMEDIUM out{};allocatedFlags=0;allocatedSize=0;locks=unlocks=0;
    {Patch a("GlobalSize",reinterpret_cast<void*>(&hugeSize)),b("GlobalAlloc",reinterpret_cast<void*>(&hugeAlloc)),c("GlobalLock",reinterpret_cast<void*>(&hugeLock)),d("GlobalUnlock",reinterpret_cast<void*>(&hugeUnlock));
        check(s.object()->GetData(&f,&out)==S_OK&&out.hGlobal==reinterpret_cast<HGLOBAL>(2)&&allocatedSize==0x100000000ull&&allocatedFlags==(GMEM_MOVEABLE|GMEM_DDESHARE)&&locks==2&&unlocks==2,"global duplication allocates full 64-bit size and copies only the low DWORD byte count");}
    cache(s)[0].m_stgMedium.tymed=TYMED_NULL;
}
void otherMedia(){
    // Actual COM storage service validates reference/copy behavior without an
    // invented framework storage object or medium adapter.
    ILockBytes* bytes=nullptr;IStorage* storage=nullptr;check(CreateILockBytesOnHGlobal(nullptr,TRUE,&bytes)==S_OK,"create real structured-storage backing");check(StgCreateDocfileOnILockBytes(bytes,STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE,0,&storage)==S_OK,"create real structured storage");
    {Source s;auto f=format(CF_TEXT,TYMED_ISTORAGE);STGMEDIUM in{};in.tymed=TYMED_ISTORAGE;in.pstg=storage;api->CacheData(s.p,0,&in,&f);STGMEDIUM out{};check(s.object()->GetData(&f,&out)==S_OK&&out.pstg==storage&&!out.pUnkForRelease,"storage GetData returns an AddRefed original interface");out.pstg->Release();}
    bytes->Release();
    {Source s;auto f=format(CF_TEXT,TYMED_FILE);const wchar_t* name=L"Z:\\native-medium.dat";auto size=(wcslen(name)+1)*2;auto* copy=static_cast<wchar_t*>(CoTaskMemAlloc(size));memcpy(copy,name,size);STGMEDIUM in{};in.tymed=TYMED_FILE;in.lpszFileName=copy;api->CacheData(s.p,0,&in,&f);STGMEDIUM out{};
        check(s.object()->GetData(&f,&out)==S_OK&&out.tymed==TYMED_FILE&&out.lpszFileName!=copy&&!wcscmp(out.lpszFileName,copy),"file medium duplicates task-allocated filename");CoTaskMemFree(out.lpszFileName);
        {Patch fail("CoTaskMemAlloc",reinterpret_cast<void*>(&failedTaskAlloc));check(s.object()->GetData(&f,&out)==DATA_E_FORMATETC&&out.tymed==TYMED_FILE&&!out.lpszFileName,"filename allocation failure preserves native partial medium type");}}
    {Source s;auto f=format(CF_BITMAP,TYMED_GDI);auto bitmap=CreateBitmap(2,2,1,1,nullptr);STGMEDIUM in{};in.tymed=TYMED_GDI;in.hBitmap=bitmap;api->CacheData(s.p,0,&in,&f);STGMEDIUM out{};check(s.object()->GetData(&f,&out)==S_OK&&out.hBitmap&&out.hBitmap!=bitmap&&out.tymed==TYMED_GDI,"GDI media use real OLE duplication");ReleaseStgMedium(&out);out={};out.tymed=TYMED_GDI;out.hBitmap=CreateBitmap(1,1,1,1,nullptr);check(s.object()->GetDataHere(&f,&out)==DATA_E_FORMATETC,"typed GDI destination rejects a pre-existing handle");DeleteObject(out.hBitmap);}
    {Source s;auto f=format(CF_ENHMETAFILE,TYMED_ENHMF);HDC dc=CreateEnhMetaFileW(nullptr,nullptr,nullptr,nullptr);MoveToEx(dc,0,0,nullptr);LineTo(dc,5,5);auto emf=CloseEnhMetaFile(dc);STGMEDIUM in{};in.tymed=TYMED_ENHMF;in.hEnhMetaFile=emf;api->CacheData(s.p,0,&in,&f);STGMEDIUM out{};check(s.object()->GetData(&f,&out)==S_OK&&out.hEnhMetaFile&&out.hEnhMetaFile!=emf&&out.tymed==TYMED_ENHMF,"enhanced metafile uses native duplication");ReleaseStgMedium(&out);}
    {Source s;auto f=format(CF_METAFILEPICT,TYMED_MFPICT);auto dc=CreateMetaFileW(nullptr);MoveToEx(dc,0,0,nullptr);LineTo(dc,5,5);auto meta=CloseMetaFile(dc);auto h=GlobalAlloc(GMEM_MOVEABLE,sizeof(METAFILEPICT));auto* pict=static_cast<METAFILEPICT*>(GlobalLock(h));pict->mm=MM_ANISOTROPIC;pict->xExt=40;pict->yExt=50;pict->hMF=meta;GlobalUnlock(h);STGMEDIUM in{};in.tymed=TYMED_MFPICT;in.hMetaFilePict=h;api->CacheData(s.p,0,&in,&f);STGMEDIUM out{};check(s.object()->GetData(&f,&out)==S_OK&&out.tymed==TYMED_MFPICT&&out.hMetaFilePict!=h,"metafile-picture medium duplicates global container");pict=static_cast<METAFILEPICT*>(GlobalLock(out.hMetaFilePict));check(pict->hMF!=meta&&pict->mm==MM_ANISOTROPIC&&pict->xExt==40&&pict->yExt==50,"metafile-picture duplicates inner metafile and preserves mapping fields");GlobalUnlock(out.hMetaFilePict);ReleaseStgMedium(&out);}
}
void run(){std::printf("%s data source\n",api->image?"Native":"Source");smoke();puts("Identity");identity();puts("Cache");cacheRules();puts("Lookup");lookups();puts("Callbacks");callbackBoundaries();puts("Enumeration");enumeration();puts("Copy");copiedMedia();puts("File rendering");fileRendering();puts("Clipboard");clipboard();puts("Drag");dragging();puts("Edge cases");enumAllocation();exceptionClassification();largeGlobal();otherMedia();}
int main(){setvbuf(stdout,nullptr,_IONBF,0);CoInitialize(nullptr);auto source=sourceAPI();api=&source;auto* old=api->thread()->m_pModuleState;select(0);run();api->thread()->m_pModuleState=old;for(auto& m:modules)api->destroyLocal(reinterpret_cast<S_Cthreadlocalobject*>(m+0xf8));memset(modules,0,sizeof(modules));
    if(auto* path=getenv("OPENMFC_NATIVE_DATA_DLL")){auto image=LoadLibraryA(path);check(image!=nullptr,"load native data source oracle");if(image){auto* b=reinterpret_cast<BYTE*>(image);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);if(pe->FileHeader.TimeDateStamp!=0x442cba30||pe->OptionalHeader.SizeOfImage!=0x55c000)return 1;auto native=nativeAPI(image);api=&native;old=api->thread()->m_pModuleState;select(0);run();api->thread()->m_pModuleState=old;for(auto& m:modules)api->destroyLocal(reinterpret_cast<S_Cthreadlocalobject*>(m+0xf8));}}
    CoUninitialize();printf("Data source: %d failures\n",failures);return failures?1:0;}
