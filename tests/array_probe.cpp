#define _AFX_NOFORCE_LIBS
#ifdef OPENMFC_USE_SDK_MFC
#include <afx.h>
#include <afxtempl.h>
#else
#include "openmfc/afxwin.h"
#endif
#include "array_probe.h"
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <type_traits>
#include <utility>

namespace {
ArrayReport* report;
void* active;
bool tracking;
bool archiveActive,failAsInteger,allocationNull;
int failAllocation,failConstructor,failAssignment;
struct Block {unsigned char* data;size_t bytes;bool live;};
Block blocks[1024];unsigned blockCount;
using Free=void(__cdecl*)(void*);Free originalFree;
template<class T>T field(const void* object,size_t offset) {T result{};if(object)memcpy(&result,static_cast<const char*>(object)+offset,sizeof(T));return result;}
long long pointer(const void* p) {
    if(!p)return 0;
    for(unsigned i=0;i<blockCount;++i)if(uintptr_t(p)>=uintptr_t(blocks[i].data)&&uintptr_t(p)<=uintptr_t(blocks[i].data)+blocks[i].bytes)
        return 100000+static_cast<long long>(i)*16384+static_cast<long long>(uintptr_t(p)-uintptr_t(blocks[i].data));
    return -1;
}
void event(long long kind,long long a=0,long long b=0,long long c=0) {
    if(!tracking)return;
    if(report->count==131072){++report->overflow;return;}
    if(archiveActive)report->events[report->count++]={kind,a,b,c,field<UINT>(active,80),pointer(field<void*>(active,88)),pointer(field<void*>(active,96)),field<UINT>(active,104)};
    else report->events[report->count++]={kind,a,b,c,field<INT_PTR>(active,16),field<INT_PTR>(active,24),field<INT_PTR>(active,32),pointer(field<void*>(active,8))};
}
void __cdecl invalidParameter(const wchar_t*,const wchar_t*,const wchar_t*,unsigned,uintptr_t) {event(94);}
void* allocate(size_t bytes) {
    if(!tracking)return malloc(bytes);
    event(1,bytes);
    if(failAllocation>0&&!--failAllocation){if(allocationNull)return nullptr;if(failAsInteger)throw 73;AfxThrowMemoryException();}
    if(bytes>1024*1024)AfxThrowMemoryException();
    auto* p=static_cast<unsigned char*>(malloc(bytes+16));if(!p)AfxThrowMemoryException();
    memset(p,0xa5,bytes);memset(p+bytes,0xde,16);
    if(blockCount==1024)abort();blocks[blockCount++]={p,bytes,true};return p;
}
void release(void* p) {
    if(tracking) {
        event(2,pointer(p));
        for(unsigned i=0;i<blockCount;++i)if(p==blocks[i].data) {event(3,i,blocks[i].live);blocks[i].live=false;return;}
    }
    if(originalFree)originalFree(p);else free(p);
}
// Observe allocation at the actual DLL boundary; all collection code remains
// inside its production or native owner. Ordinal 1489 is native operator new.
struct AllocationHook {
    BYTE* entry;BYTE saved[12];DWORD protection;void** freeSlot;void* savedFree;DWORD freeProtection;
    AllocationHook() {
#ifdef OPENMFC_USE_SDK_MFC
        HMODULE image=GetModuleHandleW(L"mfc140u.dll");
#else
        HMODULE image=GetModuleHandleW(L"openmfc_archive.dll");
#endif
        entry=reinterpret_cast<BYTE*>(GetProcAddress(image,MAKEINTRESOURCEA(1489)));if(!entry)abort();
        memcpy(saved,entry,12);VirtualProtect(entry,12,PAGE_EXECUTE_READWRITE,&protection);
        BYTE jump[12]={0x48,0xb8};auto target=&allocate;memcpy(jump+2,&target,8);jump[10]=0xff;jump[11]=0xe0;
        memcpy(entry,jump,12);FlushInstructionCache(GetCurrentProcess(),entry,12);
        auto* base=reinterpret_cast<BYTE*>(image);
        auto* nt=reinterpret_cast<IMAGE_NT_HEADERS*>(base+reinterpret_cast<IMAGE_DOS_HEADER*>(base)->e_lfanew);
        auto* dll=reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(base+nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
        freeSlot=nullptr;
        for(;dll->Name;++dll) {
            auto* names=reinterpret_cast<IMAGE_THUNK_DATA*>(base+dll->OriginalFirstThunk);
            auto* values=reinterpret_cast<IMAGE_THUNK_DATA*>(base+dll->FirstThunk);
            for(;names->u1.AddressOfData;++names,++values)
                if(!IMAGE_SNAP_BY_ORDINAL(names->u1.Ordinal)&&!strcmp(reinterpret_cast<char*>(reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(base+names->u1.AddressOfData)->Name),"free"))freeSlot=reinterpret_cast<void**>(&values->u1.Function);
        }
        if(!freeSlot)abort();savedFree=*freeSlot;originalFree=reinterpret_cast<Free>(savedFree);
        VirtualProtect(freeSlot,8,PAGE_READWRITE,&freeProtection);*freeSlot=reinterpret_cast<void*>(&release);
    }
    ~AllocationHook() {
        memcpy(entry,saved,12);FlushInstructionCache(GetCurrentProcess(),entry,12);DWORD ignored;
        VirtualProtect(entry,12,protection,&ignored);*freeSlot=savedFree;VirtualProtect(freeSlot,8,freeProtection,&ignored);originalFree=nullptr;
    }
};
struct Element {
    int value,tag;
    Element() {event(4,pointer(this),field<int>(this,0),field<int>(this,4));if(failConstructor>0&&!--failConstructor)throw 71;value=7;tag=9;}
    ~Element() {event(5,pointer(this),value,tag);}
    Element& operator=(const Element& from) {event(6,pointer(this),from.value,from.tag);if(failAssignment>0&&!--failAssignment)throw 72;value=from.value;tag=from.tag;return *this;}
};
struct Managed {
    CString text;
    int value,tag;
    Managed():value(7),tag(9) {event(4,pointer(this));if(failConstructor>0&&!--failConstructor)throw 71;}
    ~Managed() {event(5,pointer(this),value,text.GetLength());}
    Managed& operator=(const Managed& from) {event(6,pointer(this),from.value,from.text.GetLength());if(failAssignment>0&&!--failAssignment)throw 72;text=from.text;value=from.value;tag=from.tag;return *this;}
};
template<class T>void setValue(T& v,int number) {v.value=number;v.tag=number*3;}
template<>void setValue(int& v,int number) {v=number;}
template<>void setValue(Managed& v,int number) {v.value=number;v.tag=number*3;v.text=number&1?L"managed payload":L"other";}
template<class T>void values(const T& v,INT_PTR index) {event(9,index,v.value,v.tag);}
template<>void values(const int& v,INT_PTR index) {event(9,index,v);}
template<>void values(const Managed& v,INT_PTR index) {
    event(9,index,v.value,v.tag);event(10,index,v.text.GetLength());
    for(int i=0;i<v.text.GetLength();++i)event(11,i,v.text[i]);
}
template<class T>void snapshot(CArray<T,const T&>& a) {
    active=&a;event(8,a.GetCount(),a.GetUpperBound(),a.IsEmpty());
    auto* type=static_cast<CObject*>(&a)->GetRuntimeClass();
    event(7,strcmp(type->m_lpszClassName,"CObject"),type->m_nObjectSize,type->m_wSchema);
    for(INT_PTR i=0;i<a.GetSize();++i)values(a.GetAt(i),i);
    if constexpr(!std::is_same_v<T,Managed>) {
        INT_PTR capacity=field<INT_PTR>(&a,24);
        for(size_t i=0;i<size_t(capacity)*sizeof(T);++i)event(12,i,reinterpret_cast<const unsigned char*>(a.GetData())[i]);
    }
}
void snapshot(CPtrArray& a) {
    active=&a;event(8,a.GetCount(),a.GetUpperBound(),a.IsEmpty());
    auto* type=static_cast<CObject*>(&a)->GetRuntimeClass();
    event(7,strcmp(type->m_lpszClassName,"CPtrArray"),type->m_nObjectSize,type->m_wSchema);
    event(13,type==CPtrArray::GetThisClass());
    for(INT_PTR i=0;i<a.GetSize();++i)event(9,i,reinterpret_cast<INT_PTR>(a.GetAt(i)));
    for(size_t i=0;i<size_t(field<INT_PTR>(&a,24))*sizeof(void*);++i)event(12,i,reinterpret_cast<const unsigned char*>(a.GetData())[i]);
}
template<class ARRAY,class F>void operation(ARRAY& a,int label,F fn) {
    active=&a;event(20,label);
    try {fn();event(21);}
    catch(CInvalidArgException* e){event(22,1);e->Delete();}
    catch(CMemoryException* e){event(22,2);e->Delete();}
    catch(CArchiveException* e){event(22,100+e->m_cause);e->Delete();}
    catch(CException* e){event(22,3);e->Delete();}
    catch(int code){event(22,code);}
    snapshot(a);
}
template<class T>void probe(int scenario,int failure) {
    using A=CArray<T,const T&>;
    static_assert(sizeof(A)==40&&std::is_base_of_v<CObject,A>);
    static_assert(!std::is_copy_constructible_v<A>&&!std::is_copy_assignable_v<A>);
    static_assert(std::is_same_v<decltype(std::declval<A&>().GetAt(0)),T&>);
    static_assert(std::is_same_v<decltype(std::declval<const A&>().GetAt(0)),const T&>);
    alignas(A) unsigned char first[sizeof(A)],second[sizeof(A)];
    active=first;auto& a=*::new(first) A;active=second;auto& b=*::new(second) A;
    alignas(T) unsigned char input[sizeof(T)]{};
    T& value=*::new(input) T;setValue(value,43);
    failAllocation=failure>=1&&failure<=3?failure:0;
    failConstructor=failure>=4&&failure<=6?failure-3:0;
    failAssignment=failure>=7&&failure<=9?failure-6:0;
    if(scenario==0) {
        for(INT_PTR grow:{INT_PTR(0),INT_PTR(4),INT_PTR(17)}) {
            for(INT_PTR size:{INT_PTR(1),INT_PTR(3),INT_PTR(6),INT_PTR(2),INT_PTR(19),INT_PTR(1),INT_PTR(0)})
                operation(a,1,[&]{a.SetSize(size,grow);});
            operation(a,2,[&]{a.SetSize(7,grow);});operation(a,3,[&]{a.SetSize(2);});operation(a,4,[&]{a.FreeExtra();});operation(a,5,[&]{a.RemoveAll();});
        }
    } else if(scenario==1) {
        operation(a,1,[&]{a.SetSize(4,3);});
        operation(a,2,[&]{a.InsertAt(1,value,2);});
        operation(a,3,[&]{a.RemoveAt(2,3);});
        operation(a,4,[&]{a.InsertAt(9,value,2);});
        operation(a,5,[&]{a.SetAtGrow(15,value);});
        operation(a,6,[&]{event(23,a.Add(value));});
        operation(b,7,[&]{b.Copy(a);});
        operation(a,8,[&]{a.InsertAt(1,&b);});
        operation(a,9,[&]{event(23,a.Append(b));});
        operation(a,10,[&]{a.Copy(a);});operation(a,11,[&]{a.Append(a);});
        operation(a,12,[&]{a.RemoveAt(a.GetSize(),0);});
        operation(a,13,[&]{a.FreeExtra();});
    } else if(scenario==2) {
        operation(a,1,[&]{a.SetSize(3,5);});
        for(INT_PTR index:{INT_PTR(-1),INT_PTR(0),INT_PTR(2),INT_PTR(3),INT_PTR(4),INT_PTR(0x7fffffffffffffffLL)}) {
            operation(a,2,[&]{values(a.GetAt(index),index);});
            operation(a,3,[&]{a.SetAt(index,value);});
            for(INT_PTR count:{INT_PTR(-1),INT_PTR(0),INT_PTR(1),INT_PTR(4),INT_PTR(0x7fffffffffffffffLL)})
                operation(a,4,[&]{a.RemoveAt(index,count);});
        }
        operation(a,5,[&]{a.SetSize(-1,99);});
        operation(a,6,[&]{a.InsertAt(-1,&b);});
        operation(a,7,[&]{a.InsertAt(0,static_cast<A*>(nullptr));});
        operation(a,8,[&]{a.InsertAt(0,value,0);});
        operation(a,9,[&]{a.InsertAt(-1,value,1);});
        operation(a,10,[&]{a.SetAtGrow(-1,value);});
        operation(a,11,[&]{a.SetAtGrow(INT_PTR(0x7fffffffffffffffLL),value);});
    } else if(scenario==3) {
        operation(a,1,[&]{a.SetSize(3,1);});
        operation(a,2,[&]{a.SetAtGrow(7,a.GetAt(0));});
        operation(a,3,[&]{a.InsertAt(2,a.GetAt(0),3);});
        operation(a,4,[&]{a.InsertAt(0,&a);});
        operation(a,5,[&]{a.Copy(a);});
        operation(a,6,[&]{a.InsertAt(1,&a);});
    } else {
        unsigned random=unsigned(scenario)*19337+1;
        for(int i=0;i<80;++i) {
            random=random*1664525+1013904223;unsigned choice=random%10;
            INT_PTR at=(random>>8)%24,count=(random>>17)%4;
            operation(a,i,[&]{switch(choice) {
                case 0:a.SetSize(at,count);break;case 1:a.SetAtGrow(at,value);break;
                case 2:a.InsertAt(at,value,count);break;
                case 3:if(a.GetSize())a.RemoveAt(at%a.GetSize(),count);break;
                case 4:a.FreeExtra();break;case 5:b.Copy(a);break;
                case 6:if(a.GetSize()+b.GetSize()<64)a.Append(b);break;
                case 7:if(a.GetSize()+b.GetSize()<64)a.InsertAt(at,&b);break;
                case 8:if(a.GetSize())setValue(a[a.GetSize()-1],i);break;
                case 9:a.RemoveAll();break;
            }});
        }
    }
    failAllocation=failConstructor=failAssignment=0;
    active=&a;a.~A();active=&b;b.~A();active=nullptr;
    value.~T();
}
void serialization(int scenario) {
    const INT_PTR counts[]={0,1,3,65534,65535,65536};INT_PTR count=counts[scenario%6];
    CArray<int,const int&> a;a.SetSize(count);for(INT_PTR i=0;i<count;++i)a[i]=int(i*13+7);
    CMemFile file;
    {CArchive store(&file,CArchive::store,128);static_cast<CObject*>(&a)->Serialize(store);store.Close();}
    event(30,file.GetLength());file.Seek(0,CFile::begin);
    unsigned char bytes[257];UINT n;unsigned long long at=0;
    while((n=file.Read(bytes,sizeof(bytes)))!=0)for(UINT i=0;i<n;++i){ // Bounded report: hash the full wire payload.
        at=(at*131+bytes[i])&0x7fffffffffffffffLL;
    }
    event(31,at);file.Seek(0,CFile::begin);
    CArray<int,const int&> b;
    {CArchive load(&file,CArchive::load,128);b.Serialize(load);load.Close();}
    unsigned long long hash=0;for(INT_PTR i=0;i<b.GetSize();++i)hash=(hash*131+b[i])&0x7fffffffffffffffLL;
    event(32,b.GetSize(),hash);file.SetLength(file.GetLength()?file.GetLength()-1:0);file.Seek(0,CFile::begin);
    try {CArchive load(&file,CArchive::load|CArchive::bNoFlushOnDelete,128);b.Serialize(load);event(33);}
    catch(CArchiveException* e){event(33,100+e->m_cause);e->Delete();}
}
void largeArithmetic(int scenario) {
    CArray<int> a;active=&a;
    for(INT_PTR size:{INT_PTR(37),INT_PTR(38),INT_PTR(2048),INT_PTR(2049),INT_PTR(8192),INT_PTR(8193),INT_PTR(10000),INT_PTR(0)}) {
        event(50,size);a.SetSize(size);event(51);
        unsigned long long hash=0;
        for(INT_PTR i=0;i<a.GetSize();++i)hash=(hash*131+unsigned(a[i]))&0x7fffffffffffffffLL;
        event(52,hash); // All newly exposed integers must be zero, even after relocation.
    }
    for(INT_PTR grow:{INT_PTR(0),INT_PTR(0x100000003LL)}) {
        a.SetSize(0,grow);
        try {a.SetSize(scenario?INT_PTR(0x100000001LL):INT_PTR(1));event(53);}
        catch(CMemoryException* e){event(53,2);e->Delete();}
        a.RemoveAll();
    }
    active=nullptr;
}
void pointerArray(int scenario,int failure) {
    AllocationHook hook;
    alignas(CPtrArray) unsigned char first[sizeof(CPtrArray)],second[sizeof(CPtrArray)];
    active=first;CPtrArray& a=*::new(first) CPtrArray;active=second;CPtrArray& b=*::new(second) CPtrArray;
    void* value=reinterpret_cast<void*>(UINT_PTR(0x1234));
    failAllocation=failure;
    if(scenario==0) {
        for(INT_PTR grow:{INT_PTR(0),INT_PTR(4),INT_PTR(17)}) {
            for(INT_PTR size:{INT_PTR(1),INT_PTR(3),INT_PTR(6),INT_PTR(2),INT_PTR(19),INT_PTR(1),INT_PTR(0)})
                operation(a,1,[&]{a.SetSize(size,grow);});
            operation(a,2,[&]{a.SetSize(7,grow);});operation(a,3,[&]{a.SetSize(2);});operation(a,4,[&]{a.FreeExtra();});operation(a,5,[&]{a.RemoveAll();});
        }
    } else if(scenario==1) {
        operation(a,1,[&]{a.SetSize(4,3);});operation(a,2,[&]{a.InsertAt(1,value,2);});
        operation(a,3,[&]{a.RemoveAt(2,3);});operation(a,4,[&]{a.InsertAt(9,value,2);});
        operation(a,5,[&]{a.SetAtGrow(15,value);});operation(a,6,[&]{event(23,a.Add(value));});
        operation(b,7,[&]{b.Copy(a);});operation(a,8,[&]{a.InsertAt(1,&b);});
        operation(a,9,[&]{event(23,a.Append(b));});operation(a,10,[&]{a.Copy(a);});
        operation(a,11,[&]{a.Append(a);});operation(a,12,[&]{a.RemoveAt(a.GetSize(),0);});
        operation(a,13,[&]{a.FreeExtra();});
        operation(a,14,[&]{a.InsertAt(0,&a);});operation(a,15,[&]{a.InsertAt(1,&a);});
    } else if(scenario==2) {
        operation(a,1,[&]{a.SetSize(3,5);});
        for(INT_PTR index:{INT_PTR(-1),INT_PTR(0),INT_PTR(2),INT_PTR(3),INT_PTR(4),INT_PTR(0x7fffffffffffffffLL)}) {
            operation(a,2,[&]{event(23,reinterpret_cast<INT_PTR>(a.GetAt(index)));});
            operation(a,3,[&]{a.SetAt(index,value);});
            for(INT_PTR count:{INT_PTR(-1),INT_PTR(0),INT_PTR(1),INT_PTR(4),INT_PTR(0x7fffffffffffffffLL)})
                operation(a,4,[&]{a.RemoveAt(index,count);});
        }
        operation(a,5,[&]{a.SetSize(-1,99);});operation(a,6,[&]{a.InsertAt(-1,&b);});
        operation(a,7,[&]{a.InsertAt(0,static_cast<CPtrArray*>(nullptr));});
        operation(a,8,[&]{a.InsertAt(0,value,0);});operation(a,9,[&]{a.InsertAt(-1,value,1);});
        operation(a,10,[&]{a.SetAtGrow(-1,value);});operation(a,11,[&]{a.SetAtGrow(INT_PTR(0x7fffffffffffffffLL),value);});
    } else if(scenario==3) {
        operation(a,1,[&]{a.SetSize(3,INT_PTR(0x100000000LL));});
        operation(a,2,[&]{a.SetSize(4);});
        operation(a,3,[&]{a.SetSize(0,0);});
        operation(a,4,[&]{a.SetSize(INT_PTR(0x100000001LL));});
        operation(a,5,[&]{a.SetSize(-1,99);});
    } else {
        unsigned random=unsigned(scenario)*19337+1;
        for(int i=0;i<80;++i) {
            random=random*1664525+1013904223;unsigned choice=random%10;
            INT_PTR at=(random>>8)%24,count=(random>>17)%4;
            operation(a,i,[&]{switch(choice) {
                case 0:a.SetSize(at,count);break;case 1:a.SetAtGrow(at,value);break;
                case 2:a.InsertAt(at,value,count);break;
                case 3:if(a.GetSize())a.RemoveAt(at%a.GetSize(),count);break;
                case 4:a.FreeExtra();break;case 5:b.Copy(a);break;
                case 6:if(a.GetSize()+b.GetSize()<64)a.Append(b);break;
                case 7:if(a.GetSize()+b.GetSize()<64)a.InsertAt(at,&b);break;
                case 8:if(a.GetSize())a[a.GetSize()-1]=reinterpret_cast<void*>(UINT_PTR(i));break;
                case 9:a.RemoveAll();break;
            }});
        }
    }
    failAllocation=0;
    // CPtrArray inherits CObject's empty Serialize slot; it has no wire format.
    operation(a,70,[&]{CMemFile file;CArchive store(&file,CArchive::store,128);static_cast<CObject*>(&a)->Serialize(store);store.Close();event(71,file.GetLength());});
    active=&a;a.~CPtrArray();active=&b;b.~CPtrArray();active=nullptr;
}
struct ArchiveView:CArchive {
    using CArchive::CArchive;
    using Types=CArray<LoadArrayObjType,const LoadArrayObjType&>;
    void seed(UINT n) {m_nMapCount=n;}
    void** schemaSlot() {return reinterpret_cast<void**>(&m_pSchemaMap);}
};
void mapSnapshot(CMapPtrToPtr* map,bool schema) {
    event(80,pointer(map));if(!map)return;
    event(81,field<UINT>(map,16),field<INT_PTR>(map,24),field<INT_PTR>(map,48));
    event(82,pointer(field<void*>(map,8)),pointer(field<void*>(map,32)),pointer(field<void*>(map,40)));
    for(UINT_PTR key:{UINT_PTR(0),UINT_PTR(1),UINT_PTR(0x1234),UINT_PTR(0x5678)}) {
        void* value=nullptr;BOOL found=map->Lookup(reinterpret_cast<void*>(key),value);
        event(83,key,found,schema?pointer(value):reinterpret_cast<INT_PTR>(value));
        if(schema&&key==1&&value) {
            event(84,field<INT_PTR>(value,16),field<INT_PTR>(value,24),field<INT_PTR>(value,32));
            auto* data=field<int*>(value,8);event(85,pointer(data));
            for(INT_PTR i=0;i<field<INT_PTR>(value,16);++i)event(86,i,data[i]);
        }
    }
}
void archiveSnapshot(ArchiveView& ar) {
    if(ar.IsStoring())mapSnapshot(field<CMapPtrToPtr*>(&ar,88),false);
    else if(void* array=field<void*>(&ar,88)) {
        event(87,field<INT_PTR>(array,16),field<INT_PTR>(array,24),field<INT_PTR>(array,32));
        auto** data=field<void**>(array,8);event(88,pointer(data));
        for(INT_PTR i=0;i<field<INT_PTR>(array,16);++i)event(89,i,reinterpret_cast<INT_PTR>(data[i]));
    }
    mapSnapshot(field<CMapPtrToPtr*>(&ar,96),true);
}
int guarded(void(*action)(void*),void* parameter) {
    __try {action(parameter);return 0;}
    __except(GetExceptionCode()==EXCEPTION_ACCESS_VIOLATION?EXCEPTION_EXECUTE_HANDLER:EXCEPTION_CONTINUE_SEARCH) {return 1;}
}
struct MappingCall {
    ArchiveView* archive;int method;const CObject* object;ArchiveView::Types* output;
    static void invoke(void* data) {
        auto& c=*static_cast<MappingCall*>(data);
        if(c.method==0)c.archive->EnsureSchemaMapExists(&c.output);
        else if(c.method==1)c.archive->EnsureSchemaMapExists(nullptr);
        else c.archive->MapObject(c.object);
    }
};
void mapping(int scenario,int failure) {
    allocationNull=scenario>=26;
    if(allocationNull)scenario-=26;
    tracking=false;CMemFile file;BYTE buffer[128];ArchiveView ar(&file,(scenario&1)?3:2,128,buffer);
    ar.seed(0);ar.SetStoreParams(17,3);ar.SetLoadParams(3);
    tracking=true;archiveActive=true;active=&ar;AllocationHook hook;
    ArchiveView::Types* detached=nullptr;
    if(scenario>=8&&scenario<20) {
        tracking=false;
        if(scenario<16) {
            ar.MapObject(nullptr);
            ar.seed(scenario<10?0:scenario<12?5:scenario<14?0x3ffffffe:0xffffffff);
        } else {
            ar.EnsureSchemaMapExists(&detached);
            auto* map=field<CMapPtrToPtr*>(&ar,96);
            if(scenario<18)(*map)[reinterpret_cast<void*>(UINT_PTR(1))]=nullptr;
            else map->RemoveKey(reinterpret_cast<void*>(UINT_PTR(1)));
        }
        tracking=true;
    }
    if(scenario>=20) {ar.SetStoreParams(29,scenario<22?0:scenario<24?65:0xffffffff);ar.SetLoadParams(scenario<22?0:scenario<24?65:0xffffffff);}
    failAllocation=failure;failAsInteger=scenario==6||scenario==7;
    MappingCall call{&ar,0,nullptr,reinterpret_cast<ArchiveView::Types*>(UINT_PTR(9))};
    for(int step=0;step<5;++step) {
        if(scenario<2||scenario>=16)call.method=step<3?0:2;
        else if(scenario<4)call.method=step==3?0:2;
        else call.method=step<2?1:2;
        call.object=step==0?nullptr:reinterpret_cast<CObject*>(UINT_PTR(step==2?0x5678:0x1234));
        event(90,step,call.method);
        try {int fault=guarded(&MappingCall::invoke,&call);event(91,fault,pointer(call.output));}
        catch(CMemoryException* e){event(92,2,pointer(call.output));e->Delete();}
        catch(CArchiveException* e){event(92,100+e->m_cause);e->Delete();}
        catch(CException* e){event(92,3);if(e)e->Delete();}
        catch(int code){event(92,code,pointer(call.output));}
        archiveSnapshot(ar);
    }
    failAllocation=0;failAsInteger=allocationNull=false;ar.Abort();delete detached;event(93);active=nullptr;archiveActive=false;
}
}
void* operator new[](size_t bytes) {return allocate(bytes);}
void operator delete[](void* p) noexcept {release(p);}
extern "C" __declspec(dllexport) void run_array_probe(int kind,int scenario,int failure,ArrayReport* output) {
    auto previousInvalidParameter=_set_thread_local_invalid_parameter_handler(&invalidParameter);
    report=output;report->count=report->overflow=0;active=nullptr;blockCount=0;tracking=true;
    failAllocation=failConstructor=failAssignment=0;
    archiveActive=failAsInteger=allocationNull=false;
    if(kind==6)mapping(scenario,failure);
    else if(kind==5)pointerArray(scenario,failure);
    else if(kind==4)largeArithmetic(scenario);
    else if(kind==3) {
        serialization(scenario);
    } else if(kind==0)probe<int>(scenario,failure);
    else if(kind==1)probe<Element>(scenario,failure);
    else probe<Managed>(scenario,failure);
    active=nullptr;
    for(unsigned i=0;i<blockCount;++i) {
        event(40,i,blocks[i].live,blocks[i].bytes);
        for(unsigned j=0;j<16;++j)if(blocks[i].data[blocks[i].bytes+j]!=0xde)event(41,i,j);
    }
    tracking=false;
    for(unsigned i=0;i<blockCount;++i)free(blocks[i].data);
    _set_thread_local_invalid_parameter_handler(previousInvalidParameter);
}
