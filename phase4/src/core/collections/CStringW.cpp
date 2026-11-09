// Native Unicode CString constructors and ANSI-to-Unicode conversion.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <new>
#include <cerrno>

CString::CString(const wchar_t* source):CString(source,OpenMFC_GetStringMgr()) {}
CString::CString(const wchar_t* source,IAtlStringMgr* manager):CSimpleStringW(manager) {
    if(source && uintptr_t(source)<0x10000)LoadString(UINT(uintptr_t(source)));
    else SetString(source);
}
CString::CString(const char* source):CString(source,OpenMFC_GetStringMgr()) {}
CString::CString(const char* source,IAtlStringMgr* manager):CSimpleStringW(manager) {
    if(source && uintptr_t(source)<0x10000)LoadString(UINT(uintptr_t(source)));
    else operator=(source);
}
CString::CString(const unsigned char* source):CString(source,OpenMFC_GetStringMgr()) {}
CString::CString(const unsigned char* source,IAtlStringMgr* manager):CSimpleStringW(manager) {operator=(reinterpret_cast<const char*>(source));}
CString::CString(const char* source,int length):CString(source,length,OpenMFC_GetStringMgr()) {}
CString::CString(const char* source,int length,IAtlStringMgr* manager):CSimpleStringW(manager) {Convert(source,length);}
void CString::Convert(const char* source,int length) {
    if(length<=0)return;
    if(!source)InvalidLength();
    int characters=::MultiByteToWideChar(CP_THREAD_ACP,0,source,length,nullptr,0);
    wchar_t* out=GetBuffer(characters);
    ::MultiByteToWideChar(CP_THREAD_ACP,0,source,length,out,characters);
    SetLength(characters);
}
CString& CString::operator=(const char* source) {
    int characters=source?int32_t(uint32_t(::MultiByteToWideChar(CP_THREAD_ACP,0,source,-1,nullptr,0))-1):0;
    if(characters>0) {
        wchar_t* out=GetBuffer(characters);
        ::MultiByteToWideChar(CP_THREAD_ACP,0,source,-1,out,characters);
        SetLength(characters);
    } else Empty();
    return *this;
}
CString::CString(wchar_t value,int count):CSimpleStringW(OpenMFC_GetStringMgr()) {
    if(count<=0)return;
    wchar_t text[]={value,0};
    size_t width=value?wcslen(text):1;
    size_t capacity=size_t(int64_t(int32_t(uint32_t(width)+1)));
    if(capacity>SIZE_MAX/sizeof(wchar_t))AfxThrowOleException(static_cast<long>(0x80070216u));
    wchar_t local[5]={};wchar_t* converted=local;
    if(capacity*sizeof(wchar_t)>sizeof(local)) {
        converted=static_cast<wchar_t*>(malloc(capacity*sizeof(wchar_t)));
        if(!converted)AfxThrowMemoryException();
    }
    try {
        int result=wmemcpy_s(converted,capacity,text,1);
        if(result && result!=STRUNCATE) {
            if(result==ENOMEM)AfxThrowMemoryException();else AfxThrowInvalidArgException();
        }
        int length=int32_t(uint32_t(count)*uint32_t(width));
        wchar_t* out=GetBuffer(length);
        if(width==1)wmemset(out,converted[0],size_t(count));
        else for(int index=0;index<count;++index)for(int at=0;at<int(width);++at)*out++=converted[at];
        SetLength(length);
    } catch(...){if(converted!=local)free(converted);throw;}
    if(converted!=local)free(converted);
}
CString::CString(char value,int count):CSimpleStringW(OpenMFC_GetStringMgr()) {
    if(count>0){wmemset(GetBuffer(count),wchar_t(static_cast<signed char>(value)),size_t(count));SetLength(count);}
}

// Symbol: ??4?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@PEBD@Z
extern "C" CString* impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_PEBD_Z(CString* self,const char* text) {*self=text;return self;}
// Symbol: ??1?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@XZ
extern "C" void impl___1__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(CString* self) {self->~CString();}
// Symbol: ??1?$CSimpleStringT@_W$00@ATL@@QEAA@XZ
extern "C" void impl___1__CSimpleStringT__W_00_ATL__QEAA_XZ(CSimpleStringW* self) {self->~CSimpleStringW();}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@_WH@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA__WH_Z(CString* self,wchar_t value,int count) {return ::new(self) CString(value,count);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@XZ
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(CString* self) {return ::new(self) CString();}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEB_WPEAUIAtlStringMgr@1@@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEB_WPEAUIAtlStringMgr_1__Z(CString* self,const wchar_t* text,IAtlStringMgr* manager) {return ::new(self) CString(text,manager);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEB_WHPEAUIAtlStringMgr@1@@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEB_WHPEAUIAtlStringMgr_1__Z(CString* self,const wchar_t* text,int count,IAtlStringMgr* manager) {return ::new(self) CString(text,count,manager);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEB_WH@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEB_WH_Z(CString* self,const wchar_t* text,int count) {return ::new(self) CString(text,count);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEB_W@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEB_W_Z(CString* self,const wchar_t* text) {return ::new(self) CString(text);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEBEPEAUIAtlStringMgr@1@@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBEPEAUIAtlStringMgr_1__Z(CString* self,const unsigned char* text,IAtlStringMgr* manager) {return ::new(self) CString(text,manager);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEBE@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBE_Z(CString* self,const unsigned char* text) {return ::new(self) CString(text);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEBDPEAUIAtlStringMgr@1@@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBDPEAUIAtlStringMgr_1__Z(CString* self,const char* text,IAtlStringMgr* manager) {return ::new(self) CString(text,manager);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEBDHPEAUIAtlStringMgr@1@@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBDHPEAUIAtlStringMgr_1__Z(CString* self,const char* text,int count,IAtlStringMgr* manager) {return ::new(self) CString(text,count,manager);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEBDH@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBDH_Z(CString* self,const char* text,int count) {return ::new(self) CString(text,count);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEBD@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBD_Z(CString* self,const char* text) {return ::new(self) CString(text);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEA_W@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEA_W_Z(CString* self,wchar_t* text) {return ::new(self) CString(text);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEAUIAtlStringMgr@1@@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEAUIAtlStringMgr_1__Z(CString* self,IAtlStringMgr* manager) {return ::new(self) CString(manager);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEAE@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEAE_Z(CString* self,unsigned char* text) {return ::new(self) CString(text);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEAD@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEAD_Z(CString* self,char* text) {return ::new(self) CString(text);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@DH@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_DH_Z(CString* self,char value,int count) {return ::new(self) CString(value,count);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@AEBV01@@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_AEBV01__Z(CString* self,const CString* source) {return ::new(self) CString(*source);}
// Symbol: ??0?$CSimpleStringT@_W$00@ATL@@QEAA@PEB_WPEAUIAtlStringMgr@1@@Z
extern "C" CSimpleStringW* impl___0__CSimpleStringT__W_00_ATL__QEAA_PEB_WPEAUIAtlStringMgr_1__Z(CSimpleStringW* self,const wchar_t* text,IAtlStringMgr* manager) {return ::new(self) CSimpleStringW(text,manager);}
// Symbol: ??0?$CSimpleStringT@_W$00@ATL@@QEAA@PEB_WHPEAUIAtlStringMgr@1@@Z
extern "C" CSimpleStringW* impl___0__CSimpleStringT__W_00_ATL__QEAA_PEB_WHPEAUIAtlStringMgr_1__Z(CSimpleStringW* self,const wchar_t* text,int count,IAtlStringMgr* manager) {return ::new(self) CSimpleStringW(text,count,manager);}
// Symbol: ??0?$CSimpleStringT@_W$00@ATL@@QEAA@PEAUIAtlStringMgr@1@@Z
extern "C" CSimpleStringW* impl___0__CSimpleStringT__W_00_ATL__QEAA_PEAUIAtlStringMgr_1__Z(CSimpleStringW* self,IAtlStringMgr* manager) {return ::new(self) CSimpleStringW(manager);}
// Symbol: ??0?$CSimpleStringT@_W$00@ATL@@QEAA@AEBV?$CSimpleStringT@_W$0A@@1@@Z
extern "C" CSimpleStringW* impl___0__CSimpleStringT__W_00_ATL__QEAA_AEBV__CSimpleStringT__W_0A__1__Z(CSimpleStringW* self,const CSimpleStringW* source) {return ::new(self) CSimpleStringW(*source);}
// Symbol: ??0?$CSimpleStringT@_W$00@ATL@@QEAA@AEBV01@@Z
extern "C" CSimpleStringW* impl___0__CSimpleStringT__W_00_ATL__QEAA_AEBV01__Z(CSimpleStringW* self,const CSimpleStringW* source) {return ::new(self) CSimpleStringW(*source);}
// Symbol: ??4?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@_W@Z
extern "C" CString* impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01__W_Z(CString* self,wchar_t source) {*self=source;return self;}
// Symbol: ??4?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@PEB_W@Z
extern "C" CString* impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_PEB_W_Z(CString* self,const wchar_t* source) {*self=source;return self;}
// Symbol: ??4?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@PEBE@Z
extern "C" CString* impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_PEBE_Z(CString* self,const unsigned char* source) {*self=source;return self;}
// Symbol: ??4?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@D@Z
extern "C" CString* impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_D_Z(CString* self,char source) {*self=source;return self;}
// Symbol: ??4?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@AEBV01@@Z
extern "C" CString* impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_AEBV01__Z(CString* self,const CString* source) {*self=*source;return self;}
// Symbol: ??4?$CSimpleStringT@_W$00@ATL@@QEAAAEAV01@PEB_W@Z
extern "C" CSimpleStringW* impl___4__CSimpleStringT__W_00_ATL__QEAAAEAV01_PEB_W_Z(CSimpleStringW* self,const wchar_t* source) {*self=source;return self;}
// Symbol: ??4?$CSimpleStringT@_W$00@ATL@@QEAAAEAV01@AEBV?$CSimpleStringT@_W$0A@@1@@Z
extern "C" CSimpleStringW* impl___4__CSimpleStringT__W_00_ATL__QEAAAEAV01_AEBV__CSimpleStringT__W_0A__1__Z(CSimpleStringW* self,const CSimpleStringW* source) {*self=*source;return self;}
// Symbol: ??4?$CSimpleStringT@_W$00@ATL@@QEAAAEAV01@AEBV01@@Z
extern "C" CSimpleStringW* impl___4__CSimpleStringT__W_00_ATL__QEAAAEAV01_AEBV01__Z(CSimpleStringW* self,const CSimpleStringW* source) {*self=*source;return self;}
