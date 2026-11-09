// Native object indices, reserved schema entries and partial initialization.
#include "detail/FilecoreSupport.h"
#include "openmfc/afxwin.h"
extern "C" void* impl___2_YAPEAX_K_Z(size_t);
extern "C" void impl___3_YAXPEAX_Z(void*);
namespace {
template<class T,class... Args>T* Create(Args... args) {
    void* storage=impl___2_YAPEAX_K_Z(sizeof(T));
    if(!storage)return nullptr;
    try {return ::new(storage) T(args...);}
    catch(...){impl___3_YAXPEAX_Z(storage);throw;}
}
void* ReservedTypes() {return reinterpret_cast<void*>(UINT_PTR(1));}
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
        using Types=CArray<LoadArrayObjType,const LoadArrayObjType&>;
        if(!m_pSchemaMap) {Types* types=nullptr;EnsureSchemaMapExists(&types);types->InsertAt(0,typeUndefined);}
        if(object) {
            CheckCount();m_pLoadArray->InsertAt(m_nMapCount,const_cast<CObject*>(object));
            void* stored=nullptr;m_pSchemaMap->Lookup(ReservedTypes(),stored);
            if(!stored)AfxThrowInvalidArgException();
            static_cast<Types*>(stored)->InsertAt(m_nMapCount,typeCObject);++m_nMapCount;
        }
    }
}
// Symbol: ?MapObject@CArchive@@QEAAXPEBVCObject@@@Z
extern "C" void impl__MapObject_CArchive__QEAAXPEBVCObject___Z(CArchive* self,const CObject* object) {self->MapObject(object);}
// Symbol: ?EnsureSchemaMapExists@CArchive@@QEAAXPEAPEAV?$CArray@W4LoadArrayObjType@CArchive@@AEBW412@@@@Z
extern "C" void impl__EnsureSchemaMapExists_CArchive__QEAAXPEAPEAV__CArray_W4LoadArrayObjType_CArchive__AEBW412____Z(CArchive* self,void** output) {CArchiveAccess::InvokeEnsureSchemaMapExists(self,output);}
