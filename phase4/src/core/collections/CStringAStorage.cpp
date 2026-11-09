// Native ANSI string storage and lifetime export owners.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxstr.h"
#include <new>
// Symbol: ?UnlockBuffer@?$CSimpleStringT@D$00@ATL@@QEAAXXZ
extern "C" void impl__UnlockBuffer___CSimpleStringT_D_00_ATL__QEAAXXZ(CSimpleStringA* self) {self->UnlockBuffer();}
// Symbol: ?Truncate@?$CSimpleStringT@D$00@ATL@@QEAAXH@Z
extern "C" void impl__Truncate___CSimpleStringT_D_00_ATL__QEAAXH_Z(CSimpleStringA* self,int length) {self->Truncate(length);}
// Symbol: ?ThrowMemoryException@?$CSimpleStringT@D$00@ATL@@KAXXZ
extern "C" void impl__ThrowMemoryException___CSimpleStringT_D_00_ATL__KAXXZ() {CSimpleStringA::ThrowMemoryException();}
// Symbol: ?StringLength@?$CSimpleStringT@D$00@ATL@@SAHPEB_W@Z
extern "C" int impl__StringLength___CSimpleStringT_D_00_ATL__SAHPEB_W_Z(const wchar_t* text) {return text?int(wcslen(text)):0;}
// Symbol: ?StringLength@?$CSimpleStringT@D$00@ATL@@SAHPEBD@Z
extern "C" int impl__StringLength___CSimpleStringT_D_00_ATL__SAHPEBD_Z(const char* text) {return CSimpleStringA::StringLength(text);}
// Symbol: ?SetString@?$CSimpleStringT@D$00@ATL@@QEAAXPEBDH@Z
extern "C" void impl__SetString___CSimpleStringT_D_00_ATL__QEAAXPEBDH_Z(CSimpleStringA* self,const char* source,int length) {self->SetString(source,length);}
// Symbol: ?SetString@?$CSimpleStringT@D$00@ATL@@QEAAXPEBD@Z
extern "C" void impl__SetString___CSimpleStringT_D_00_ATL__QEAAXPEBD_Z(CSimpleStringA* self,const char* source) {self->SetString(source);}
// Symbol: ?SetManager@?$CSimpleStringT@D$00@ATL@@QEAAXPEAUIAtlStringMgr@2@@Z
extern "C" void impl__SetManager___CSimpleStringT_D_00_ATL__QEAAXPEAUIAtlStringMgr_2__Z(CSimpleStringA* self,IAtlStringMgr* manager) {self->SetManager(manager);}
// Symbol: ?SetLength@?$CSimpleStringT@D$00@ATL@@AEAAXH@Z
extern "C" void impl__SetLength___CSimpleStringT_D_00_ATL__AEAAXH_Z(CSimpleStringA* self,int length) {self->SetLength(length);}
// Symbol: ?SetAt@?$CSimpleStringT@D$00@ATL@@QEAAXHD@Z
extern "C" void impl__SetAt___CSimpleStringT_D_00_ATL__QEAAXHD_Z(CSimpleStringA* self,int index,char value) {self->SetAt(index,value);}
// Symbol: ?ReleaseBufferSetLength@?$CSimpleStringT@D$00@ATL@@QEAAXH@Z
extern "C" void impl__ReleaseBufferSetLength___CSimpleStringT_D_00_ATL__QEAAXH_Z(CSimpleStringA* self,int length) {self->ReleaseBufferSetLength(length);}
// Symbol: ?ReleaseBuffer@?$CSimpleStringT@D$00@ATL@@QEAAXH@Z
extern "C" void impl__ReleaseBuffer___CSimpleStringT_D_00_ATL__QEAAXH_Z(CSimpleStringA* self,int length) {self->ReleaseBuffer(length);}
// Symbol: ?Reallocate@?$CSimpleStringT@D$00@ATL@@AEAAXH@Z
extern "C" void impl__Reallocate___CSimpleStringT_D_00_ATL__AEAAXH_Z(CSimpleStringA* self,int length) {self->Reallocate(length);}
// Symbol: ?PrepareWrite@?$CSimpleStringT@D$00@ATL@@AEAAPEADH@Z
extern "C" char* impl__PrepareWrite___CSimpleStringT_D_00_ATL__AEAAPEADH_Z(CSimpleStringA* self,int length) {return self->GetBuffer(length);}
// Symbol: ?PrepareWrite2@?$CSimpleStringT@D$00@ATL@@AEAAXH@Z
extern "C" void impl__PrepareWrite2___CSimpleStringT_D_00_ATL__AEAAXH_Z(CSimpleStringA* self,int length) {self->PrepareWrite2(length);}
// Symbol: ?Preallocate@?$CSimpleStringT@D$00@ATL@@QEAAXH@Z
extern "C" void impl__Preallocate___CSimpleStringT_D_00_ATL__QEAAXH_Z(CSimpleStringA* self,int length) {self->Preallocate(length);}
// Symbol: ?LockBuffer@?$CSimpleStringT@D$00@ATL@@QEAAPEADXZ
extern "C" char* impl__LockBuffer___CSimpleStringT_D_00_ATL__QEAAPEADXZ(CSimpleStringA* self) {return self->LockBuffer();}
// Symbol: ?IsEmpty@?$CSimpleStringT@D$00@ATL@@QEBA_NXZ
extern "C" bool impl__IsEmpty___CSimpleStringT_D_00_ATL__QEBA_NXZ(const CSimpleStringA* self) {return self->IsEmpty();}
// Symbol: ?GetString@?$CSimpleStringT@D$00@ATL@@QEBAPEBDXZ
extern "C" const char* impl__GetString___CSimpleStringT_D_00_ATL__QEBAPEBDXZ(const CSimpleStringA* self) {return self->GetString();}
// Symbol: ?GetManager@?$CSimpleStringT@D$00@ATL@@QEBAPEAUIAtlStringMgr@2@XZ
extern "C" IAtlStringMgr* impl__GetManager___CSimpleStringT_D_00_ATL__QEBAPEAUIAtlStringMgr_2_XZ(const CSimpleStringA* self) {return self->GetManager();}
// Symbol: ?GetLength@?$CSimpleStringT@D$00@ATL@@QEBAHXZ
extern "C" int impl__GetLength___CSimpleStringT_D_00_ATL__QEBAHXZ(const CSimpleStringA* self) {return self->GetLength();}
// Symbol: ?GetData@?$CSimpleStringT@D$00@ATL@@AEBAPEAUCStringData@2@XZ
extern "C" CStringData* impl__GetData___CSimpleStringT_D_00_ATL__AEBAPEAUCStringData_2_XZ(const CSimpleStringA* self) {return self->GetData();}
// Symbol: ?GetBufferSetLength@?$CSimpleStringT@D$00@ATL@@QEAAPEADH@Z
extern "C" char* impl__GetBufferSetLength___CSimpleStringT_D_00_ATL__QEAAPEADH_Z(CSimpleStringA* self,int length) {return self->GetBufferSetLength(length);}
// Symbol: ?GetBuffer@?$CSimpleStringT@D$00@ATL@@QEAAPEADXZ
extern "C" char* impl__GetBuffer___CSimpleStringT_D_00_ATL__QEAAPEADXZ(CSimpleStringA* self) {return self->GetBuffer();}
// Symbol: ?GetBuffer@?$CSimpleStringT@D$00@ATL@@QEAAPEADH@Z
extern "C" char* impl__GetBuffer___CSimpleStringT_D_00_ATL__QEAAPEADH_Z(CSimpleStringA* self,int length) {return self->GetBuffer(length);}
// Symbol: ?GetAt@?$CSimpleStringT@D$00@ATL@@QEBADH@Z
extern "C" char impl__GetAt___CSimpleStringT_D_00_ATL__QEBADH_Z(const CSimpleStringA* self,int index) {return self->GetAt(index);}
// Symbol: ?GetAllocLength@?$CSimpleStringT@D$00@ATL@@QEBAHXZ
extern "C" int impl__GetAllocLength___CSimpleStringT_D_00_ATL__QEBAHXZ(const CSimpleStringA* self) {return self->GetAllocLength();}
// Symbol: ?FreeExtra@?$CSimpleStringT@D$00@ATL@@QEAAXXZ
extern "C" void impl__FreeExtra___CSimpleStringT_D_00_ATL__QEAAXXZ(CSimpleStringA* self) {self->FreeExtra();}
// Symbol: ?Fork@?$CSimpleStringT@D$00@ATL@@AEAAXH@Z
extern "C" void impl__Fork___CSimpleStringT_D_00_ATL__AEAAXH_Z(CSimpleStringA* self,int length) {self->Fork(length);}
// Symbol: ?Empty@?$CSimpleStringT@D$00@ATL@@QEAAXXZ
extern "C" void impl__Empty___CSimpleStringT_D_00_ATL__QEAAXXZ(CSimpleStringA* self) {self->Empty();}
// Symbol: ?CopyCharsOverlapped@?$CSimpleStringT@D$00@ATL@@SAXPEADPEBDH@Z
extern "C" void impl__CopyCharsOverlapped___CSimpleStringT_D_00_ATL__SAXPEADPEBDH_Z(char* output,const char* source,int length) {if(length)memmove_s(output,size_t(length),source,size_t(length));}
// Symbol: ?CopyChars@?$CSimpleStringT@D$00@ATL@@SAXPEADPEBDH@Z
extern "C" void impl__CopyChars___CSimpleStringT_D_00_ATL__SAXPEADPEBDH_Z(char* output,const char* source,int length) {if(length)memcpy_s(output,size_t(length),source,size_t(length));}
// Symbol: ?Concatenate@?$CSimpleStringT@D$00@ATL@@KAXAEAV12@PEBDH1H@Z
extern "C" void impl__Concatenate___CSimpleStringT_D_00_ATL__KAXAEAV12_PEBDH1H_Z(CSimpleStringA* self,const char* first,int firstLength,const char* second,int secondLength) {int length=int32_t(uint32_t(firstLength)+uint32_t(secondLength));char* out=self->GetBuffer(length);if(firstLength)memcpy_s(out,size_t(firstLength),first,size_t(firstLength));if(secondLength)memcpy_s(out+firstLength,size_t(secondLength),second,size_t(secondLength));self->SetLength(length);}
// Symbol: ?CloneData@?$CSimpleStringT@D$00@ATL@@CAPEAUCStringData@2@PEAU32@@Z
extern "C" CStringData* impl__CloneData___CSimpleStringT_D_00_ATL__CAPEAUCStringData_2_PEAU32__Z(CStringData* data) {return CSimpleStringA::CloneData(data);}
// Symbol: ?Attach@?$CSimpleStringT@D$00@ATL@@AEAAXPEAUCStringData@2@@Z
extern "C" void impl__Attach___CSimpleStringT_D_00_ATL__AEAAXPEAUCStringData_2__Z(CSimpleStringA* self,CStringData* data) {self->Attach(data);}
// Symbol: ?AppendChar@?$CSimpleStringT@D$00@ATL@@QEAAXD@Z
extern "C" void impl__AppendChar___CSimpleStringT_D_00_ATL__QEAAXD_Z(CSimpleStringA* self,char value) {self->AppendChar(value);}
// Symbol: ?Append@?$CSimpleStringT@D$00@ATL@@QEAAXPEBDH@Z
extern "C" void impl__Append___CSimpleStringT_D_00_ATL__QEAAXPEBDH_Z(CSimpleStringA* self,const char* source,int length) {self->Append(source,length);}
// Symbol: ?Append@?$CSimpleStringT@D$00@ATL@@QEAAXPEBD@Z
extern "C" void impl__Append___CSimpleStringT_D_00_ATL__QEAAXPEBD_Z(CSimpleStringA* self,const char* source) {self->Append(source);}
// Symbol: ?Append@?$CSimpleStringT@D$00@ATL@@QEAAXAEBV12@@Z
extern "C" void impl__Append___CSimpleStringT_D_00_ATL__QEAAXAEBV12__Z(CSimpleStringA* self,const CSimpleStringA* source) {self->Append(*source);}
// Symbol: ??Y?$CSimpleStringT@D$00@ATL@@QEAAAEAV01@_W@Z
extern "C" CSimpleStringA* impl___Y__CSimpleStringT_D_00_ATL__QEAAAEAV01__W_Z(CSimpleStringA* self,wchar_t value) {self->AppendChar(static_cast<char>(value));return self;}
// Symbol: ??Y?$CSimpleStringT@D$00@ATL@@QEAAAEAV01@PEBD@Z
extern "C" CSimpleStringA* impl___Y__CSimpleStringT_D_00_ATL__QEAAAEAV01_PEBD_Z(CSimpleStringA* self,const char* source) {self->Append(source);return self;}
// Symbol: ??Y?$CSimpleStringT@D$00@ATL@@QEAAAEAV01@E@Z
extern "C" CSimpleStringA* impl___Y__CSimpleStringT_D_00_ATL__QEAAAEAV01_E_Z(CSimpleStringA* self,unsigned char value) {self->AppendChar(static_cast<char>(value));return self;}
// Symbol: ??Y?$CSimpleStringT@D$00@ATL@@QEAAAEAV01@D@Z
extern "C" CSimpleStringA* impl___Y__CSimpleStringT_D_00_ATL__QEAAAEAV01_D_Z(CSimpleStringA* self,char value) {self->AppendChar(static_cast<char>(value));return self;}
// Symbol: ??Y?$CSimpleStringT@D$00@ATL@@QEAAAEAV01@AEBV01@@Z
extern "C" CSimpleStringA* impl___Y__CSimpleStringT_D_00_ATL__QEAAAEAV01_AEBV01__Z(CSimpleStringA* self,const CSimpleStringA* source) {self->Append(*source);return self;}
// Symbol: ??B?$CSimpleStringT@D$00@ATL@@QEBAPEBDXZ
extern "C" const char* impl___B__CSimpleStringT_D_00_ATL__QEBAPEBDXZ(const CSimpleStringA* self) {return self->GetString();}
// Symbol: ??B?$CSimpleStringT@D$00@ATL@@QEAAAEAV?$CSimpleStringT@D$0A@@1@XZ
extern "C" CSimpleStringA* impl___B__CSimpleStringT_D_00_ATL__QEAAAEAV__CSimpleStringT_D_0A__1_XZ(CSimpleStringA* self) {return self;}
// Symbol: ??A?$CSimpleStringT@D$00@ATL@@QEBADH@Z
extern "C" char impl___A__CSimpleStringT_D_00_ATL__QEBADH_Z(const CSimpleStringA* self,int index) {return self->GetAt(index);}
// Symbol: ??4?$CSimpleStringT@D$00@ATL@@QEAAAEAV01@PEBD@Z
extern "C" CSimpleStringA* impl___4__CSimpleStringT_D_00_ATL__QEAAAEAV01_PEBD_Z(CSimpleStringA* self,const char* source) {*self=source;return self;}
// Symbol: ??4?$CSimpleStringT@D$00@ATL@@QEAAAEAV01@AEBV?$CSimpleStringT@D$0A@@1@@Z
extern "C" CSimpleStringA* impl___4__CSimpleStringT_D_00_ATL__QEAAAEAV01_AEBV__CSimpleStringT_D_0A__1__Z(CSimpleStringA* self,const CSimpleStringA* source) {*self=*source;return self;}
// Symbol: ??4?$CSimpleStringT@D$00@ATL@@QEAAAEAV01@AEBV01@@Z
extern "C" CSimpleStringA* impl___4__CSimpleStringT_D_00_ATL__QEAAAEAV01_AEBV01__Z(CSimpleStringA* self,const CSimpleStringA* source) {*self=*source;return self;}
// Symbol: ??1?$CSimpleStringT@D$00@ATL@@QEAA@XZ
extern "C" void impl___1__CSimpleStringT_D_00_ATL__QEAA_XZ(CSimpleStringA* self) {self->~CSimpleStringA();}
// Symbol: ??0?$CSimpleStringT@D$00@ATL@@QEAA@PEBDPEAUIAtlStringMgr@1@@Z
extern "C" CSimpleStringA* impl___0__CSimpleStringT_D_00_ATL__QEAA_PEBDPEAUIAtlStringMgr_1__Z(CSimpleStringA* self,const char* source,IAtlStringMgr* manager) {return ::new(self) CSimpleStringA(source,manager);}
// Symbol: ??0?$CSimpleStringT@D$00@ATL@@QEAA@PEBDHPEAUIAtlStringMgr@1@@Z
extern "C" CSimpleStringA* impl___0__CSimpleStringT_D_00_ATL__QEAA_PEBDHPEAUIAtlStringMgr_1__Z(CSimpleStringA* self,const char* source,int length,IAtlStringMgr* manager) {return ::new(self) CSimpleStringA(source,length,manager);}
// Symbol: ??0?$CSimpleStringT@D$00@ATL@@QEAA@PEAUIAtlStringMgr@1@@Z
extern "C" CSimpleStringA* impl___0__CSimpleStringT_D_00_ATL__QEAA_PEAUIAtlStringMgr_1__Z(CSimpleStringA* self,IAtlStringMgr* manager) {return ::new(self) CSimpleStringA(manager);}
// Symbol: ??0?$CSimpleStringT@D$00@ATL@@QEAA@AEBV?$CSimpleStringT@D$0A@@1@@Z
extern "C" CSimpleStringA* impl___0__CSimpleStringT_D_00_ATL__QEAA_AEBV__CSimpleStringT_D_0A__1__Z(CSimpleStringA* self,const CSimpleStringA* source) {return ::new(self) CSimpleStringA(*source);}
// Symbol: ??0?$CSimpleStringT@D$00@ATL@@QEAA@AEBV01@@Z
extern "C" CSimpleStringA* impl___0__CSimpleStringT_D_00_ATL__QEAA_AEBV01__Z(CSimpleStringA* self,const CSimpleStringA* source) {return ::new(self) CSimpleStringA(*source);}
// Symbol: ?LoadStringW@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAHPEAUHINSTANCE__@@IG@Z
extern "C" int impl__LoadStringW___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAHPEAUHINSTANCE____IG_Z(CStringA* self,HINSTANCE module,UINT id,WORD language) {return self->LoadString(module,id,language);}
// Symbol: ?LoadStringW@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAHPEAUHINSTANCE__@@I@Z
extern "C" int impl__LoadStringW___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(CStringA* self,HINSTANCE module,UINT id) {return self->LoadString(module,id);}
// Symbol: ?LoadStringW@?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAHI@Z
extern "C" int impl__LoadStringW___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAHI_Z(CStringA* self,UINT id) {return self->LoadString(id);}
// Symbol: ??4?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV01@_W@Z
extern "C" CStringA* impl___4__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01__W_Z(CStringA* self,wchar_t value) {*self=value;return self;}
// Symbol: ??4?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV01@PEB_W@Z
extern "C" CStringA* impl___4__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_PEB_W_Z(CStringA* self,const wchar_t* source) {*self=source;return self;}
// Symbol: ??4?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV01@PEBE@Z
extern "C" CStringA* impl___4__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_PEBE_Z(CStringA* self,const unsigned char* source) {*self=source;return self;}
// Symbol: ??4?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV01@PEBD@Z
extern "C" CStringA* impl___4__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_PEBD_Z(CStringA* self,const char* source) {*self=source;return self;}
// Symbol: ??4?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV01@D@Z
extern "C" CStringA* impl___4__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_D_Z(CStringA* self,char value) {*self=value;return self;}
// Symbol: ??4?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV01@AEBV01@@Z
extern "C" CStringA* impl___4__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_AEBV01__Z(CStringA* self,const CStringA* source) {*self=*source;return self;}
// Symbol: ??1?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAA@XZ
extern "C" void impl___1__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_XZ(CStringA* self) {self->~CStringA();}
// Symbol: ??0?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAA@_WH@Z
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA__WH_Z(CStringA* self,wchar_t value,int count) {return ::new(self) CStringA(value,count);}
// Symbol: ??0?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAA@XZ
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_XZ(CStringA* self) {return ::new(self) CStringA;}
// Symbol: ??0?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAA@PEB_WPEAUIAtlStringMgr@1@@Z
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEB_WPEAUIAtlStringMgr_1__Z(CStringA* self,const wchar_t* source,IAtlStringMgr* manager) {return ::new(self) CStringA(source,manager);}
// Symbol: ??0?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAA@PEB_WHPEAUIAtlStringMgr@1@@Z
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEB_WHPEAUIAtlStringMgr_1__Z(CStringA* self,const wchar_t* source,int length,IAtlStringMgr* manager) {return ::new(self) CStringA(source,length,manager);}
// Symbol: ??0?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAA@PEB_WH@Z
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEB_WH_Z(CStringA* self,const wchar_t* source,int length) {return ::new(self) CStringA(source,length);}
// Symbol: ??0?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAA@PEB_W@Z
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEB_W_Z(CStringA* self,const wchar_t* source) {return ::new(self) CStringA(source);}
// Symbol: ??0?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAA@PEBEPEAUIAtlStringMgr@1@@Z
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEBEPEAUIAtlStringMgr_1__Z(CStringA* self,const unsigned char* source,IAtlStringMgr* manager) {return ::new(self) CStringA(source,manager);}
// Symbol: ??0?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAA@PEBE@Z
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEBE_Z(CStringA* self,const unsigned char* source) {return ::new(self) CStringA(source);}
// Symbol: ??0?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAA@PEBDPEAUIAtlStringMgr@1@@Z
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEBDPEAUIAtlStringMgr_1__Z(CStringA* self,const char* source,IAtlStringMgr* manager) {return ::new(self) CStringA(source,manager);}
// Symbol: ??0?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAA@PEBDHPEAUIAtlStringMgr@1@@Z
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEBDHPEAUIAtlStringMgr_1__Z(CStringA* self,const char* source,int length,IAtlStringMgr* manager) {return ::new(self) CStringA(source,length,manager);}
// Symbol: ??0?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAA@PEBDH@Z
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEBDH_Z(CStringA* self,const char* source,int length) {return ::new(self) CStringA(source,length);}
// Symbol: ??0?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAA@PEBD@Z
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEBD_Z(CStringA* self,const char* source) {return ::new(self) CStringA(source);}
// Symbol: ??0?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAA@PEA_W@Z
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEA_W_Z(CStringA* self,const wchar_t* source) {return ::new(self) CStringA(source);}
// Symbol: ??0?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAA@PEAUIAtlStringMgr@1@@Z
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEAUIAtlStringMgr_1__Z(CStringA* self,IAtlStringMgr* manager) {return ::new(self) CStringA(manager);}
// Symbol: ??0?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAA@PEAE@Z
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEAE_Z(CStringA* self,unsigned char* source) {return ::new(self) CStringA(source);}
// Symbol: ??0?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAA@PEAD@Z
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEAD_Z(CStringA* self,const char* source) {return ::new(self) CStringA(source);}
// Symbol: ??0?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAA@DH@Z
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_DH_Z(CStringA* self,char value,int count) {return ::new(self) CStringA(value,count);}
// Symbol: ??0?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAA@AEBV01@@Z
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_AEBV01__Z(CStringA* self,const CStringA* source) {return ::new(self) CStringA(*source);}
