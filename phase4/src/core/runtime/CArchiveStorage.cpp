// Native archive representation, file dispatch and buffer ownership.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afx.h"
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cstddef>
#include <new>
extern "C" void* impl___2_YAPEAX_K_Z(size_t);
extern "C" void impl__AfxThrowArchiveException__YAXHPEB_W_Z(int,const wchar_t*);
namespace {
BYTE* Advance(BYTE* pointer,size_t count) {return reinterpret_cast<BYTE*>(uintptr_t(pointer)+count);}
UINT Distance(const BYTE* end,const BYTE* start) {return UINT(uintptr_t(end)-uintptr_t(start));}
void ArchiveError(int cause,const wchar_t* name) {impl__AfxThrowArchiveException__YAXHPEB_W_Z(cause,name);}
void Copy(void* output,size_t capacity,const void* source,size_t count) {
    if(memcpy_s(output,capacity,source,count))AfxThrowInvalidArgException();
}
void Move(void* output,size_t capacity,const void* source,size_t count) {
    if(memmove_s(output,capacity,source,count))AfxThrowInvalidArgException();
}
struct Layout: CArchive {
    static void Validate() {
        static_assert(offsetof(Layout,m_pDocument)==0 && offsetof(Layout,m_bForceFlat)==8 &&
            offsetof(Layout,m_bDirectBuffer)==12 && offsetof(Layout,m_bBlocking)==16 &&
            offsetof(Layout,m_nObjectSchema)==20 && offsetof(Layout,m_strFileName)==24 &&
            offsetof(Layout,m_nMode)==32 && offsetof(Layout,m_bUserBuf)==36 &&
            offsetof(Layout,m_nBufSize)==40 && offsetof(Layout,m_pFile)==48 &&
            offsetof(Layout,m_lpBufCur)==56 && offsetof(Layout,m_lpBufMax)==64 &&
            offsetof(Layout,m_lpBufStart)==72 && offsetof(Layout,m_nMapCount)==80 &&
            offsetof(Layout,m_pStoreMap)==88 && offsetof(Layout,m_pSchemaMap)==96 &&
            offsetof(Layout,m_nGrowSize)==104 && offsetof(Layout,m_nHashSize)==108,"native archive member offsets");
    }
};
}
CArchive::CArchive(CFile* file,UINT mode,int size,void* buffer) {
    if(!file)AfxThrowInvalidArgException();
    m_strFileName=file->GetFilePath();
    m_nMode=mode;m_pFile=file;m_pSchemaMap=nullptr;m_pStoreMap=nullptr;
    m_pDocument=nullptr;m_bForceFlat=TRUE;m_bDirectBuffer=FALSE;m_nObjectSchema=UINT32_MAX;
    m_nGrowSize=(mode&load)?64:16;m_nHashSize=137;
    m_lpBufStart=static_cast<BYTE*>(buffer);m_bUserBuf=TRUE;
    m_bBlocking=file->GetBufferPtr(CFile::bufferCheck,0,nullptr,nullptr)&CFile::bufferBlocking;
    int actual=size<128?128:size;if(size<128)m_lpBufStart=nullptr;m_nBufSize=actual;
    if(!m_lpBufStart) {
        m_bDirectBuffer=m_pFile->GetBufferPtr(CFile::bufferCheck,0,nullptr,nullptr)&CFile::bufferDirect;
        if(m_bDirectBuffer)actual=0;
        else {m_lpBufStart=static_cast<BYTE*>(impl___2_YAPEAX_K_Z(size_t(m_nBufSize)));m_bUserBuf=FALSE;}
    }
    m_lpBufMax=Advance(m_lpBufStart,UINT(actual));
    m_lpBufCur=IsLoading()?m_lpBufMax:m_lpBufStart;
}
CArchive::~CArchive() {
    if(m_pFile && !(m_nMode&bNoFlushOnDelete)){Flush();m_pFile=nullptr;}
    Abort();
}
void CArchive::Abort() {
    m_pFile=nullptr;
    if(!m_bUserBuf){std::free(m_lpBufStart);m_lpBufStart=nullptr;m_lpBufCur=nullptr;}
    void* types=nullptr;
    if(m_pSchemaMap && m_pSchemaMap->Lookup(reinterpret_cast<void*>(UINT_PTR(1)),types) && types)
        delete static_cast<CObject*>(types);
    if(m_pSchemaMap)delete m_pSchemaMap;
    m_pSchemaMap=nullptr;
    if(m_pStoreMap)delete static_cast<CObject*>(m_pStoreMap);
    m_pStoreMap=nullptr;
}
void CArchive::Close() {Flush();m_pFile=nullptr;}
void CArchive::Flush() {
    if(IsLoading()) {
        if(m_lpBufMax!=m_lpBufCur)m_pFile->Seek(int32_t(Distance(m_lpBufCur,m_lpBufMax)),CFile::current);
        m_lpBufCur=m_lpBufMax;
    } else {
        if(!m_bDirectBuffer) {
            if(m_lpBufCur!=m_lpBufStart)m_pFile->Write(m_lpBufStart,Distance(m_lpBufCur,m_lpBufStart));
        } else {
            if(m_lpBufCur!=m_lpBufStart)m_pFile->GetBufferPtr(CFile::bufferCommit,Distance(m_lpBufCur,m_lpBufStart),nullptr,nullptr);
            m_pFile->GetBufferPtr(CFile::bufferWrite,UINT(m_nBufSize),reinterpret_cast<void**>(&m_lpBufStart),reinterpret_cast<void**>(&m_lpBufMax));
        }
        m_lpBufCur=m_lpBufStart;
    }
}
void CArchive::FillBuffer(UINT additional) {
    if(!IsLoading())ArchiveError(4,m_strFileName.GetString());
    UINT remaining=Distance(m_lpBufMax,m_lpBufCur),needed=remaining+additional;
    if(!m_bDirectBuffer) {
        if(uintptr_t(m_lpBufCur)>uintptr_t(m_lpBufStart)) {
            if(int32_t(remaining)>0) {
                Move(m_lpBufStart,uintptr_t(m_lpBufMax)-uintptr_t(m_lpBufStart),m_lpBufCur,remaining);
                m_lpBufCur=m_lpBufStart;m_lpBufMax=Advance(m_lpBufStart,remaining);
            }
            UINT room=UINT(m_nBufSize)-remaining;
            if(m_bBlocking && additional<room)room=additional;
            BYTE* destination=Advance(m_lpBufStart,remaining);
            for(;;) {
                UINT count=m_pFile->Read(destination,room);remaining+=count;
                if(!count)break;room-=count;if(!room)break;
                destination=Advance(destination,count);if(remaining>=needed)break;
            }
            m_lpBufCur=m_lpBufStart;m_lpBufMax=Advance(m_lpBufStart,remaining);
        }
    } else {
        if(remaining)m_pFile->Seek(int32_t(0u-remaining),CFile::current);
        m_pFile->GetBufferPtr(CFile::bufferRead,UINT(m_nBufSize),reinterpret_cast<void**>(&m_lpBufStart),reinterpret_cast<void**>(&m_lpBufMax));
        m_lpBufCur=m_lpBufStart;
    }
    if(UINT(m_nBufSize)<needed)ArchiveError(8,nullptr);
    if(Distance(m_lpBufMax,m_lpBufCur)<needed)ArchiveError(3,nullptr);
}
UINT CArchive::Read(void* output,UINT count) {
    if(!count || !output)return 0;
    if(!IsLoading())ArchiveError(4,m_strFileName.GetString());
    BYTE* destination=static_cast<BYTE*>(output);
    UINT first=Distance(m_lpBufMax,m_lpBufCur);if(count<first)first=count;
    if(first)Copy(destination,count,m_lpBufCur,first);
    m_lpBufCur=Advance(m_lpBufCur,first);UINT remaining=count-first;
    if(remaining) {
        destination=Advance(destination,first);UINT bulk=remaining-remaining%UINT(m_nBufSize),read=0,request=bulk;
        for(;;) {
            UINT obtained=m_pFile->Read(destination,request);read+=obtained;destination=Advance(destination,obtained);
            if(!obtained)break;request-=obtained;if(!request)break;
        }
        remaining-=read;
        if(remaining && read==bulk) {
            if(!m_bDirectBuffer) {
                UINT request=m_bBlocking?remaining:(remaining>UINT(m_nBufSize)?remaining:UINT(m_nBufSize));
                BYTE* current=m_lpBufStart;UINT total=0;
                for(;;) {
                    UINT obtained=m_pFile->Read(current,request);total+=obtained;
                    if(!obtained)break;request-=obtained;if(!request)break;
                    current=Advance(current,obtained);if(total>=remaining)break;
                }
                m_lpBufMax=Advance(m_lpBufStart,total);
            } else m_pFile->GetBufferPtr(CFile::bufferRead,UINT(m_nBufSize),reinterpret_cast<void**>(&m_lpBufStart),reinterpret_cast<void**>(&m_lpBufMax));
            m_lpBufCur=m_lpBufStart;UINT copied=Distance(m_lpBufMax,m_lpBufStart);if(remaining<copied)copied=remaining;
            if(copied)Copy(destination,remaining,m_lpBufStart,copied);
            m_lpBufCur=Advance(m_lpBufCur,copied);remaining-=copied;
        }
    }
    return count-remaining;
}
void CArchive::Write(const void* input,UINT count) {
    if(!count || !input)return;
    if(IsLoading())ArchiveError(2,m_strFileName.GetString());
    UINT first=Distance(m_lpBufMax,m_lpBufCur);if(count<first)first=count;
    if(first)Copy(m_lpBufCur,uintptr_t(m_lpBufMax)-uintptr_t(m_lpBufCur),input,first);
    m_lpBufCur=Advance(m_lpBufCur,first);UINT remaining=count-first;
    if(!remaining)return;
    const BYTE* source=static_cast<const BYTE*>(input)+first;Flush();
    UINT tail=remaining%UINT(m_nBufSize),bulk=remaining-tail;
    m_pFile->Write(source,bulk);
    if(m_bDirectBuffer) {
        m_pFile->GetBufferPtr(CFile::bufferWrite,UINT(m_nBufSize),reinterpret_cast<void**>(&m_lpBufStart),reinterpret_cast<void**>(&m_lpBufMax));
        m_lpBufCur=m_lpBufStart;
    }
    if(tail>=UINT(m_nBufSize) || m_lpBufCur!=m_lpBufStart)AfxThrowInvalidArgException();
    if(tail)Copy(m_lpBufCur,tail,source+bulk,tail);
    m_lpBufCur=Advance(m_lpBufCur,tail);
}
UINT CArchive::GetObjectSchema() {UINT result=m_nObjectSchema;m_nObjectSchema=UINT32_MAX;return result;}
void CArchive::CheckCount() {if(m_nMapCount>=0x3ffffffe)ArchiveError(5,m_strFileName.GetString());}
namespace {
template<class T>T LoadCountValue(CArchive& ar,BYTE*& cursor,BYTE*& maximum) {
    if(!ar.IsLoading())ArchiveError(4,ar.m_strFileName.GetString());
    if(uintptr_t(cursor)+sizeof(T)>uintptr_t(maximum))ar.FillBuffer(UINT(uintptr_t(cursor)-uintptr_t(maximum)+sizeof(T)));
    T value;std::memcpy(&value,cursor,sizeof(value));cursor=Advance(cursor,sizeof(value));return value;
}
template<class T>void StoreCountValue(CArchive& ar,BYTE*& cursor,BYTE*& maximum,T value) {
    if(ar.IsLoading())ArchiveError(2,ar.m_strFileName.GetString());
    if(uintptr_t(cursor)+sizeof(T)>uintptr_t(maximum))ar.Flush();
    std::memcpy(cursor,&value,sizeof(value));cursor=Advance(cursor,sizeof(value));
}
}
ULONGLONG CArchive::ReadCount() {
    WORD word=LoadCountValue<WORD>(*this,m_lpBufCur,m_lpBufMax);if(word!=0xffff)return word;
    DWORD dword=LoadCountValue<DWORD>(*this,m_lpBufCur,m_lpBufMax);if(dword!=0xffffffff)return dword;
    return LoadCountValue<ULONGLONG>(*this,m_lpBufCur,m_lpBufMax);
}
void CArchive::WriteCount(ULONGLONG count) {
    if(count<0xffff)StoreCountValue(*this,m_lpBufCur,m_lpBufMax,WORD(count));
    else {StoreCountValue(*this,m_lpBufCur,m_lpBufMax,WORD(0xffff));
        if(count<0xffffffff)StoreCountValue(*this,m_lpBufCur,m_lpBufMax,DWORD(count));
        else {StoreCountValue(*this,m_lpBufCur,m_lpBufMax,DWORD(0xffffffff));StoreCountValue(*this,m_lpBufCur,m_lpBufMax,count);}}
}

// Symbol: ?FillBuffer@CArchive@@QEAAXI@Z
extern "C" void impl__FillBuffer_CArchive__QEAAXI_Z(CArchive* self,UINT count) {self->FillBuffer(count);}
// Symbol: ?GetObjectSchema@CArchive@@QEAAIXZ
extern "C" UINT impl__GetObjectSchema_CArchive__QEAAIXZ(CArchive* self) {return self->GetObjectSchema();}
// Symbol: ?WriteCount@CArchive@@QEAAX_K@Z
extern "C" void impl__WriteCount_CArchive__QEAAX_K_Z(CArchive* self,ULONGLONG count) {self->WriteCount(count);}
// Symbol: ?ReadCount@CArchive@@QEAA_KXZ
extern "C" ULONGLONG impl__ReadCount_CArchive__QEAA_KXZ(CArchive* self) {return self->ReadCount();}
// Symbol: ?CheckCount@CArchive@@QEAAXXZ
extern "C" void impl__CheckCount_CArchive__QEAAXXZ(CArchive* self) {self->CheckCount();}
// Symbol: ??0CArchive@@QEAA@PEAVCFile@@IHPEAX@Z
extern "C" CArchive* impl___0CArchive__QEAA_PEAVCFile__IHPEAX_Z(void* storage,CFile* file,UINT mode,int size,void* buffer) {return ::new(storage) CArchive(file,mode,size,buffer);}
// Symbol: ??1CArchive@@QEAA@XZ
extern "C" void impl___1CArchive__QEAA_XZ(CArchive* self) {self->~CArchive();}
// Symbol: ?Abort@CArchive@@QEAAXXZ
extern "C" void impl__Abort_CArchive__QEAAXXZ(CArchive* self) {self->Abort();}
// Symbol: ?Close@CArchive@@QEAAXXZ
extern "C" void impl__Close_CArchive__QEAAXXZ(CArchive* self) {self->Close();}
// Symbol: ?Flush@CArchive@@QEAAXXZ
extern "C" void impl__Flush_CArchive__QEAAXXZ(CArchive* self) {self->Flush();}
// Symbol: ?Read@CArchive@@QEAAIPEAXI@Z
extern "C" UINT impl__Read_CArchive__QEAAIPEAXI_Z(CArchive* self,void* output,UINT count) {return self->Read(output,count);}
// Symbol: ?Write@CArchive@@QEAAXPEBXI@Z
extern "C" void impl__Write_CArchive__QEAAXPEBXI_Z(CArchive* self,const void* input,UINT count) {self->Write(input,count);}
