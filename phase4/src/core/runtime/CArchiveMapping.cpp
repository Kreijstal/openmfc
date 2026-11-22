// Native archive class and object references share a single load index.
#include "detail/FilecoreSupport.h"
#include "openmfc/afxwin.h"
extern "C" void* impl___2_YAPEAX_K_Z(size_t);
extern "C" void impl___3_YAXPEAX_Z(void*);
extern "C" void impl__AfxThrowNotSupportedException__YAXXZ();
namespace {
constexpr WORD extendedTag=0x7fff;
constexpr WORD newClassTag=0xffff;
constexpr DWORD classTagBit=0x80000000;
constexpr DWORD indexMask=0x7fffffff;
void LoadError(const CArchive& archive,int cause) {
    AfxThrowArchiveException(cause,archive.m_strFileName.GetString());
}

template<class T,class... Args>T* Create(Args... args) {
    void* storage=impl___2_YAPEAX_K_Z(sizeof(T));
    if(!storage)return nullptr;
    try {return ::new(storage) T(args...);}
    catch(...){impl___3_YAXPEAX_Z(storage);throw;}
}
void* ReservedTypes() {return reinterpret_cast<void*>(UINT_PTR(1));}
}
void CArchive::AddLoadedEntry(void* value,LoadArrayObjType type) {
    CheckCount();
    m_pLoadArray->InsertAt(m_nMapCount,value);
    void* stored=nullptr;
    m_pSchemaMap->Lookup(ReservedTypes(),stored);
    if(!stored)AfxThrowInvalidArgException();
    static_cast<LoadTypes*>(stored)->InsertAt(m_nMapCount,type);
    ++m_nMapCount;
}

void CArchive::EnsureSchemaMapExists(CArray<LoadArrayObjType,const LoadArrayObjType&>** output) {
    using Types=CArray<LoadArrayObjType,const LoadArrayObjType&>;
    CMapPtrToPtr* schemas=m_pSchemaMap;
    if(!schemas)schemas=Create<CMapPtrToPtr>(10);
    void* stored=nullptr;
    if(!schemas->Lookup(ReservedTypes(),stored)) {
        Types* types=Create<Types>();
        try {types->SetSize(1,m_nGrowSize);(*schemas)[ReservedTypes()]=types;stored=types;}
        catch(CException* error) {delete types;if(error)error->Delete();stored=nullptr;}
    }
    m_pSchemaMap=schemas;
    if(output)*output=static_cast<Types*>(stored);
}
void CArchive::MapObject(const CObject* object) {
    if(IsStoring()) {
        if(!m_pStoreMap) {
            m_pStoreMap=Create<CMapPtrToPtr>(m_nGrowSize?INT_PTR(m_nGrowSize):INT_PTR(10));
            m_pStoreMap->InitHashTable(m_nHashSize);
            (*m_pStoreMap)[nullptr]=nullptr;m_nMapCount=1;
        }
        if(object) {CheckCount();UINT index=m_nMapCount++;(*m_pStoreMap)[const_cast<CObject*>(object)]=reinterpret_cast<void*>(UINT_PTR(index));}
    } else {
        if(!m_pLoadArray) {
            m_pLoadArray=Create<CPtrArray>();m_pLoadArray->SetSize(1,m_nGrowSize);
            m_pLoadArray->SetAt(0,nullptr);m_nMapCount=1;
        }
        if(!m_pSchemaMap) {LoadTypes* types=nullptr;EnsureSchemaMapExists(&types);types->InsertAt(0,typeUndefined);}
        if(object)AddLoadedEntry(const_cast<CObject*>(object),typeCObject);
    }
}

CRuntimeClass* CArchive::ReadClass(const CRuntimeClass* expected,UINT* schemaOut,DWORD* tagOut) {
    if(!IsLoading())LoadError(*this,CArchiveException::generic);
    if(expected && expected->m_wSchema==0xffff)impl__AfxThrowNotSupportedException__YAXXZ();
    MapObject(nullptr);

    const WORD word=CArchiveAccess::LoadScalar<WORD>(*this);
    const DWORD tag=word==extendedTag ? CArchiveAccess::LoadScalar<DWORD>(*this)
        : DWORD(word&extendedTag) | ((word&0x8000) ? classTagBit : 0);
    if(!(tag&classTagBit)) {
        if(!tagOut)LoadError(*this,CArchiveException::badIndex);
        *tagOut=tag;
        return nullptr;
    }

    UINT schema;
    CRuntimeClass* type;
    if(word==newClassTag) {
        type=CRuntimeClass::Load(*this,&schema);
        if(!type)LoadError(*this,CArchiveException::badClass);
        if((type->m_wSchema&indexMask)!=schema) {
            if(!(type->m_wSchema&VERSIONABLE_SCHEMA))LoadError(*this,CArchiveException::badSchema);
            EnsureSchemaMapExists(nullptr);
            m_pSchemaMap->SetAt(type,reinterpret_cast<void*>(UINT_PTR(schema)));
        }
        AddLoadedEntry(type,typeCRuntimeClass);
    } else {
        const DWORD index=tag&indexMask;
        if(!index || index>DWORD(m_pLoadArray->GetSize())-1)LoadError(*this,CArchiveException::badIndex);
        void* stored=nullptr;
        m_pSchemaMap->Lookup(ReservedTypes(),stored);
        if(!stored)AfxThrowInvalidArgException();
        if(static_cast<LoadTypes*>(stored)->GetAt(index)==typeCObject)LoadError(*this,CArchiveException::badIndex);
        type=static_cast<CRuntimeClass*>(m_pLoadArray->GetAt(index));
        schema=m_pSchemaMap->Lookup(type,stored) ? UINT(UINT_PTR(stored)) : type->m_wSchema&indexMask;
    }
    if(expected && !type->IsDerivedFrom(expected))LoadError(*this,CArchiveException::badClass);
    if(schemaOut)*schemaOut=schema;
    else m_nObjectSchema=schema;
    if(tagOut)*tagOut=tag;
    return type;
}

CObject* CArchive::ReadObject(const CRuntimeClass* expected) {
    if(!IsLoading())LoadError(*this,CArchiveException::writeOnly);
    UINT schema;
    DWORD tag;
    CRuntimeClass* type=ReadClass(expected,&schema,&tag);
    if(!type) {
        if(tag>DWORD(m_pLoadArray->GetSize())-1)LoadError(*this,CArchiveException::badIndex);
        void* stored=nullptr;
        if(m_pSchemaMap->Lookup(ReservedTypes(),stored)) {
            auto* types=static_cast<LoadTypes*>(stored);
            if(!types)AfxThrowInvalidArgException();
            if(tag<=DWORD(types->GetSize())-1 && types->GetAt(tag)==typeCRuntimeClass)
                LoadError(*this,CArchiveException::badIndex);
        }
        auto* object=static_cast<CObject*>(m_pLoadArray->GetAt(tag));
        if(object && expected && !object->IsKindOf(expected))LoadError(*this,CArchiveException::badClass);
        return object;
    }

    CObject* object=nullptr;
    try {
        object=type->CreateObject();
        if(!object)AfxThrowMemoryException();
        AddLoadedEntry(object,typeCObject);
        const UINT previousSchema=m_nObjectSchema;
        m_nObjectSchema=schema;
        object->Serialize(*this);
        m_nObjectSchema=previousSchema;
    } catch(CException*) {
        delete object;
        throw;
    }
    return object;
}

void CArchive::SerializeClass(const CRuntimeClass* type) {
    if(IsLoading())ReadClass(type);
    else WriteClass(type);
}

// Symbol: ?ReadClass@CArchive@@QEAAPEAUCRuntimeClass@@PEBU2@PEAIPEAK@Z
extern "C" CRuntimeClass* impl__ReadClass_CArchive__QEAAPEAUCRuntimeClass__PEBU2_PEAIPEAK_Z(
    CArchive* self,const CRuntimeClass* expected,UINT* schema,DWORD* tag) {
    return self->ReadClass(expected,schema,tag);
}
// Symbol: ?ReadObject@CArchive@@QEAAPEAVCObject@@PEBUCRuntimeClass@@@Z
extern "C" CObject* impl__ReadObject_CArchive__QEAAPEAVCObject__PEBUCRuntimeClass___Z(
    CArchive* self,const CRuntimeClass* expected) {
    return self->ReadObject(expected);
}
// Symbol: ?SerializeClass@CArchive@@QEAAXPEBUCRuntimeClass@@@Z
extern "C" void impl__SerializeClass_CArchive__QEAAXPEBUCRuntimeClass___Z(
    CArchive* self,const CRuntimeClass* type) {
    self->SerializeClass(type);
}
// Symbol: ?MapObject@CArchive@@QEAAXPEBVCObject@@@Z
extern "C" void impl__MapObject_CArchive__QEAAXPEBVCObject___Z(CArchive* self,const CObject* object) {self->MapObject(object);}
// Symbol: ?EnsureSchemaMapExists@CArchive@@QEAAXPEAPEAV?$CArray@W4LoadArrayObjType@CArchive@@AEBW412@@@@Z
extern "C" void impl__EnsureSchemaMapExists_CArchive__QEAAXPEAPEAV__CArray_W4LoadArrayObjType_CArchive__AEBW412____Z(CArchive* self,void** output) {CArchiveAccess::InvokeEnsureSchemaMapExists(self,output);}
