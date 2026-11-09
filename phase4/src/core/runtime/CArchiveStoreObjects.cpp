// Native class/object tags share an index space. Identity is committed before
// user serialization so recursive graphs can reference the object in progress.
#include "detail/FilecoreSupport.h"
#include "openmfc/afxwin.h"
extern "C" void impl__AfxThrowNotSupportedException__YAXXZ();
namespace {
void Error(CArchive& archive,int cause) {AfxThrowArchiveException(cause,archive.m_strFileName.GetString());}
void Reference(CArchive& archive,UINT index,bool type) {
    if(index<0x7fff)CArchiveAccess::StoreScalar(archive,WORD(index|(type?0x8000:0)));
    else {CArchiveAccess::StoreScalar(archive,WORD(0x7fff));CArchiveAccess::StoreScalar(archive,DWORD(index|(type?0x80000000:0)));}
}
}
void CArchive::WriteClass(const CRuntimeClass* type) {
    if(!type)Error(*this,6);
    if(IsLoading())Error(*this,1);
    if(type->m_wSchema==0xffff)impl__AfxThrowNotSupportedException__YAXXZ();
    MapObject(nullptr);
    UINT index=UINT(UINT_PTR((*m_pStoreMap)[const_cast<CRuntimeClass*>(type)]));
    if(index)Reference(*this,index,true);
    else {
        CArchiveAccess::StoreScalar(*this,WORD(0xffff));type->Store(*this);CheckCount();
        index=m_nMapCount++;(*m_pStoreMap)[const_cast<CRuntimeClass*>(type)]=reinterpret_cast<void*>(UINT_PTR(index));
    }
}
void CArchive::WriteObject(const CObject* object) {
    if(IsLoading())Error(*this,2);
    MapObject(nullptr);
    if(!object) {CArchiveAccess::StoreScalar(*this,WORD(0));return;}
    UINT index=UINT(UINT_PTR((*m_pStoreMap)[const_cast<CObject*>(object)]));
    if(index)Reference(*this,index,false);
    else {
        WriteClass(object->GetRuntimeClass());CheckCount();
        index=m_nMapCount++;(*m_pStoreMap)[const_cast<CObject*>(object)]=reinterpret_cast<void*>(UINT_PTR(index));
        const_cast<CObject*>(object)->Serialize(*this);
    }
}
// Symbol: ?WriteClass@CArchive@@QEAAXPEBUCRuntimeClass@@@Z
extern "C" void impl__WriteClass_CArchive__QEAAXPEBUCRuntimeClass___Z(CArchive* archive,const CRuntimeClass* type) {archive->WriteClass(type);}
// Symbol: ?WriteObject@CArchive@@QEAAXPEBVCObject@@@Z
extern "C" void impl__WriteObject_CArchive__QEAAXPEBVCObject___Z(CArchive* archive,const CObject* object) {archive->WriteObject(object);}
