// OLE verbs are message-map entries, in derived-to-base declaration order.
#define OPENMFC_APPCORE_IMPL
#include "detail/OleVerbSupport.h"
#include <cstdlib>
#include <cstring>
#include <new>

extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long);
extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(UINT);

namespace {
struct Entry {
    UINT message,code,first,last;
    UINT_PTR signature;
    BOOL (MS_ABI* handler)(CCmdTarget*,MSG*,HWND,const RECT*);
};
struct Map {
    const Map* (MS_ABI* base)();
    const Entry* entries;
};
static_assert(sizeof(Entry)==32 && offsetof(Entry,handler)==24,"native message entry");
static_assert(sizeof(AFX_PMSG)==8 && sizeof(AFX_MSGMAP_ENTRY)==32,"client message-map ABI");
const Map* MessageMap(CCmdTarget* target) {
    using Getter=const Map* (MS_ABI*)(CCmdTarget*);
    auto** vtable=*reinterpret_cast<void***>(target);
    return reinterpret_cast<Getter>(vtable[12])(target);
}
bool Custom(const Entry& entry) {
    return entry.message==0xc002 && entry.code==0 && entry.first<=1 && entry.last>=1;
}
// Allocation is staged so failed resource/name/object allocation releases all
// entries already collected, including when an MFC exception unwinds the walk.
struct Verbs {
    OLEVERB* data=nullptr;
    ULONG count=0,capacity=0;
    ~Verbs() {
        for(ULONG i=0;i<count;++i)CoTaskMemFree(data[i].lpszVerbName);
        std::free(data);
    }
    void Append(UINT resource) {
        if(count==capacity) {
            size_t next=static_cast<size_t>(capacity)+10;
            if(next>ULONG_MAX || next>SIZE_MAX/sizeof(OLEVERB)) {
                impl__AfxThrowMemoryException__YAXXZ();return;
            }
            auto* grown=static_cast<OLEVERB*>(std::realloc(data,next*sizeof(OLEVERB)));
            if(!grown) { impl__AfxThrowMemoryException__YAXXZ();return; }
            data=grown;capacity=static_cast<ULONG>(next);
        }
        const wchar_t* name=nullptr;
        HINSTANCE module=static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(resource));
        int length=module ? LoadStringW(module,resource,reinterpret_cast<LPWSTR>(&name),0) : 0;
        if(!length) { name=L"<unknown verb>";length=14; }
        auto* copy=static_cast<wchar_t*>(CoTaskMemAlloc((static_cast<size_t>(length)+1)*sizeof(wchar_t)));
        if(!copy) { impl__AfxThrowMemoryException__YAXXZ();return; }
        std::memcpy(copy,name,length*sizeof(wchar_t));copy[length]=0;
        data[count]={static_cast<LONG>(count),copy,0,OLEVERBATTRIB_ONCONTAINERMENU};
        ++count;
    }
};
}

// Retail mfc140u RVA 0x2711e0. Negative verb IDs compare as UINT ranges;
// nonnegative IDs count custom entries across the inherited message maps.
// Symbol: ?DoOleVerb@CCmdTarget@@QEAAHJPEAUtagMSG@@PEAUHWND__@@PEBUtagRECT@@@Z
extern "C" int MS_ABI impl__DoOleVerb_CCmdTarget__QEAAHJPEAUtagMSG__PEAUHWND____PEBUtagRECT___Z(
    CCmdTarget* target,long verb,void* message,void* parent,const void* rectangle) {
    LONG ordinal=-1;
    for(const Map* map=MessageMap(target);map && map->base;map=map->base()) {
        for(const Entry* entry=map->entries;entry->signature;++entry) {
            if(entry->message!=0xc002 || entry->code!=0)continue;
            bool match=verb<0 ? entry->first<=static_cast<UINT>(verb) && entry->last>=static_cast<UINT>(verb)
                              : Custom(*entry) && ++ordinal==verb;
            if(!match)continue;
            if(entry->handler(target,static_cast<MSG*>(message),static_cast<HWND>(parent),static_cast<const RECT*>(rectangle)))return TRUE;
            impl__AfxThrowOleException__YAXJ_Z(S_OK);
            return FALSE;
        }
    }
    return FALSE;
}

// Retail mfc140u RVA 0x270f70. Only custom entries appear in this enumerator.
// Symbol: ?EnumOleVerbs@CCmdTarget@@QEAAHPEAPEAUIEnumOLEVERB@@@Z
extern "C" int MS_ABI impl__EnumOleVerbs_CCmdTarget__QEAAHPEAPEAUIEnumOLEVERB___Z(CCmdTarget* target,void** out) {
    if(!out) { impl__AfxThrowInvalidArgException__YAXXZ();return FALSE; }
    *out=nullptr;
    Verbs verbs;
    for(const Map* map=MessageMap(target);map && map->base;map=map->base())
        for(const Entry* entry=map->entries;entry->signature;++entry)
            if(Custom(*entry))verbs.Append(static_cast<UINT>(entry->signature));
    if(!verbs.count)return FALSE;
    auto* enumerator=new(std::nothrow)CEnumOleVerb;
    if(!enumerator) { impl__AfxThrowMemoryException__YAXXZ();return FALSE; }
    enumerator->SetVerbs(verbs.data,verbs.count);
    verbs.data=nullptr;verbs.count=0;
    *out=enumerator->Interface();return TRUE;
}

BOOL CCmdTarget::DoOleVerb(LONG verb,MSG* message,HWND parent,const RECT* rectangle) {
    return impl__DoOleVerb_CCmdTarget__QEAAHJPEAUtagMSG__PEAUHWND____PEBUtagRECT___Z(this,verb,message,parent,rectangle);
}
BOOL CCmdTarget::EnumOleVerbs(IEnumOLEVERB** out) {
    return impl__EnumOleVerbs_CCmdTarget__QEAAHPEAPEAUIEnumOLEVERB___Z(this,reinterpret_cast<void**>(out));
}
