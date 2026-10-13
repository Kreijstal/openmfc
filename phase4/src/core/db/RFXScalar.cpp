// Native scalar RFX operations over recordset status and value-cache storage.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxdb.h"
#include <cstdlib>
#include <cstring>
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
namespace {
template<class T,class IsNull,class SetNull,class Fixup>
void Scalar(CFieldExchange* fx,const wchar_t* name,T* value,int cType,SQLULEN precision,int cacheType,
            IsNull isNull,SetNull setNull,Fixup fixup,bool ensure=false) {
    if(!fx || !name) {impl__AfxThrowInvalidArgException__YAXXZ();return;}
    UINT field;if(!fx->IsFieldType(&field))return;
    CRecordset* rs=fx->m_prs;
    if(ensure && !rs) {impl__AfxThrowInvalidArgException__YAXXZ();return;}
    DWORD index=field-1;SQLLEN* length=rs->GetFieldLengthBuffer(index,fx->m_nFieldType);
    if(ensure && !length) {impl__AfxThrowInvalidArgException__YAXXZ();return;}
    switch(fx->m_nOperation) {
    case CFieldExchange::Fixup:
        if(*length==SQL_NULL_DATA) {rs->SetNullFieldStatus(index);setNull(*value);}
        else fixup(*value);
        return;
    case CFieldExchange::MarkForAddNew:
        if(!isNull(*value)) {rs->SetDirtyFieldStatus(index);rs->ClearNullFieldStatus(index);}return;
    case CFieldExchange::MarkForUpdate:
        if(!isNull(*value))rs->ClearNullFieldStatus(index);
        break;
    case CFieldExchange::SetFieldNull:
        if((!fx->m_pvField && fx->m_nFieldType==0) || fx->m_pvField==value) {
            if(fx->m_bField) {rs->SetNullFieldStatus(index);setNull(*value);*length=SQL_NULL_DATA;}
            else {rs->ClearNullFieldStatus(index);*length=sizeof(T);}
        }
        return;
    case CFieldExchange::AllocCache: {
        auto& cache=rs->m_rgFieldInfos[index];
        if(cacheType==7 || cacheType==15 || cacheType==22) {
            void* data=std::malloc(sizeof(T));
            if(!data)impl__AfxThrowMemoryException__YAXXZ();
            cache.m_pvDataCache=data;
        }
        cache.m_nDataType=cacheType;return;
    }
    default:break;
    }
    fx->Default(name,value,length,cType,sizeof(T),precision);
}
template<class T> void Keep(T&) {}
template<class T,class Bits> T FromBits(Bits bits) {T value;std::memcpy(&value,&bits,sizeof(value));return value;}
float SingleNull() {return FromBits<float>(0xe09e4242u);}
double DoubleNull() {return FromBits<double>(0xc413c84846922b8cULL);}
bool TimestampNull(const TIMESTAMP_STRUCT& v) {return v.year==99 && v.month==99 && v.day==99 && v.hour==99 && v.minute==99 && v.second==99 && v.fraction==99;}
void SetTimestampNull(TIMESTAMP_STRUCT& v) {v.year=v.month=v.day=v.hour=v.minute=v.second=99;v.fraction=99;}
}
// Symbol: ?RFX_BigInt@@YAXPEAVCFieldExchange@@PEB_WAEA_J@Z
extern "C" void MS_ABI impl__RFX_BigInt__YAXPEAVCFieldExchange__PEB_WAEA_J_Z(CFieldExchange* fx,const wchar_t* name,__int64* value) {
    Scalar(fx,name,value,SQL_C_SBIGINT,19,22,[](__int64 v){return v==0x4a4d4120LL;},[](__int64& v){v=0x4a4d4120LL;},Keep<__int64>);
}
// Symbol: ?RFX_Bool@@YAXPEAVCFieldExchange@@PEB_WAEAH@Z
extern "C" void MS_ABI impl__RFX_Bool__YAXPEAVCFieldExchange__PEB_WAEAH_Z(CFieldExchange* fx,const wchar_t* name,int* value) {
    Scalar(fx,name,value,SQL_C_BIT,1,1,[](int v){return v==2;},[](int& v){v=2;},[](int& v){v=*reinterpret_cast<BYTE*>(&v);});
}
// Symbol: ?RFX_Byte@@YAXPEAVCFieldExchange@@PEB_WAEAE@Z
extern "C" void MS_ABI impl__RFX_Byte__YAXPEAVCFieldExchange__PEB_WAEAE_Z(CFieldExchange* fx,const wchar_t* name,BYTE* value) {
    Scalar(fx,name,value,SQL_C_TINYINT,3,2,[](BYTE v){return v==0xff;},[](BYTE& v){v=0xff;},Keep<BYTE>);
}
// Symbol: ?RFX_Int@@YAXPEAVCFieldExchange@@PEB_WAEAH@Z
extern "C" void MS_ABI impl__RFX_Int__YAXPEAVCFieldExchange__PEB_WAEAH_Z(CFieldExchange* fx,const wchar_t* name,int* value) {
    Scalar(fx,name,value,SQL_C_LONG,5,3,[](int v){return v==0x7ee4;},[](int& v){v=0x7ee4;},Keep<int>);
}
// Symbol: ?RFX_Long@@YAXPEAVCFieldExchange@@PEB_WAEAJ@Z
extern "C" void MS_ABI impl__RFX_Long__YAXPEAVCFieldExchange__PEB_WAEAJ_Z(CFieldExchange* fx,const wchar_t* name,long* value) {
    Scalar(fx,name,value,SQL_C_LONG,10,4,[](long v){return v==0x4a4d4120L;},[](long& v){v=0x4a4d4120L;},Keep<long>);
}
// Symbol: ?RFX_Single@@YAXPEAVCFieldExchange@@PEB_WAEAM@Z
extern "C" void MS_ABI impl__RFX_Single__YAXPEAVCFieldExchange__PEB_WAEAM_Z(CFieldExchange* fx,const wchar_t* name,float* value) {
    Scalar(fx,name,value,SQL_C_FLOAT,13,6,[](float v){return v==SingleNull();},[](float& v){v=SingleNull();},Keep<float>);
}
// Symbol: ?RFX_Double@@YAXPEAVCFieldExchange@@PEB_WAEAN@Z
extern "C" void MS_ABI impl__RFX_Double__YAXPEAVCFieldExchange__PEB_WAEAN_Z(CFieldExchange* fx,const wchar_t* name,double* value) {
    Scalar(fx,name,value,SQL_C_DOUBLE,22,7,[](double v){return v==DoubleNull();},[](double& v){v=DoubleNull();},Keep<double>);
}
// Symbol: ?RFX_Date@@YAXPEAVCFieldExchange@@PEB_WAEAUtagTIMESTAMP_STRUCT@@@Z
extern "C" void MS_ABI impl__RFX_Date__YAXPEAVCFieldExchange__PEB_WAEAUtagTIMESTAMP_STRUCT___Z(CFieldExchange* fx,const wchar_t* name,TIMESTAMP_STRUCT* value) {
    Scalar(fx,name,value,SQL_C_TIMESTAMP,23,15,TimestampNull,SetTimestampNull,Keep<TIMESTAMP_STRUCT>,true);
}
