// Native Unicode simple-string storage export owners.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxstr.h"
// Symbol: ?IsEmpty@?$CSimpleStringT@_W$00@ATL@@QEBA_NXZ
extern "C" bool impl__IsEmpty___CSimpleStringT__W_00_ATL__QEBA_NXZ(const CSimpleStringW* self) {return self->IsEmpty();}
// Symbol: ?GetString@?$CSimpleStringT@_W$00@ATL@@QEBAPEB_WXZ
extern "C" const wchar_t* impl__GetString___CSimpleStringT__W_00_ATL__QEBAPEB_WXZ(const CSimpleStringW* self) {return self->GetString();}
// Symbol: ??A?$CSimpleStringT@_W$00@ATL@@QEBA_WH@Z
extern "C" wchar_t impl___A__CSimpleStringT__W_00_ATL__QEBA_WH_Z(const CSimpleStringW* self,int index) {return self->GetAt(index);}
// Symbol: ?GetLength@?$CSimpleStringT@_W$00@ATL@@QEBAHXZ
extern "C" int impl__GetLength___CSimpleStringT__W_00_ATL__QEBAHXZ(const CSimpleStringW* self) {return self->GetLength();}
// Symbol: ?Empty@?$CSimpleStringT@_W$00@ATL@@QEAAXXZ
extern "C" void impl__Empty___CSimpleStringT__W_00_ATL__QEAAXXZ(CSimpleStringW* self) {self->Empty();}
// Symbol: ?GetAt@?$CSimpleStringT@_W$00@ATL@@QEBA_WH@Z
extern "C" wchar_t impl__GetAt___CSimpleStringT__W_00_ATL__QEBA_WH_Z(const CSimpleStringW* self,int index) {return self->GetAt(index);}
// Symbol: ?GetAllocLength@?$CSimpleStringT@_W$00@ATL@@QEBAHXZ
extern "C" int impl__GetAllocLength___CSimpleStringT__W_00_ATL__QEBAHXZ(const CSimpleStringW* self) {return self->GetAllocLength();}
// Symbol: ??Y?$CSimpleStringT@_W$00@ATL@@QEAAAEAV01@AEBV01@@Z
extern "C" CSimpleStringW* impl___Y__CSimpleStringT__W_00_ATL__QEAAAEAV01_AEBV01__Z(CSimpleStringW* self,const CSimpleStringW* source) {self->Append(*source);return self;}
// Symbol: ??Y?$CSimpleStringT@_W$00@ATL@@QEAAAEAV01@D@Z
extern "C" CSimpleStringW* impl___Y__CSimpleStringT__W_00_ATL__QEAAAEAV01_D_Z(CSimpleStringW* self,char value) {self->AppendChar(static_cast<wchar_t>(value));return self;}
// Symbol: ??Y?$CSimpleStringT@_W$00@ATL@@QEAAAEAV01@E@Z
extern "C" CSimpleStringW* impl___Y__CSimpleStringT__W_00_ATL__QEAAAEAV01_E_Z(CSimpleStringW* self,unsigned char value) {self->AppendChar(static_cast<wchar_t>(value));return self;}
// Symbol: ??Y?$CSimpleStringT@_W$00@ATL@@QEAAAEAV01@PEB_W@Z
extern "C" CSimpleStringW* impl___Y__CSimpleStringT__W_00_ATL__QEAAAEAV01_PEB_W_Z(CSimpleStringW* self,const wchar_t* source) {self->Append(source);return self;}
// Symbol: ??Y?$CSimpleStringT@_W$00@ATL@@QEAAAEAV01@_W@Z
extern "C" CSimpleStringW* impl___Y__CSimpleStringT__W_00_ATL__QEAAAEAV01__W_Z(CSimpleStringW* self,wchar_t value) {self->AppendChar(static_cast<wchar_t>(value));return self;}
// Symbol: ?AppendChar@?$CSimpleStringT@_W$00@ATL@@QEAAX_W@Z
extern "C" void impl__AppendChar___CSimpleStringT__W_00_ATL__QEAAX_W_Z(CSimpleStringW* self,wchar_t value) {self->AppendChar(value);}
// Symbol: ?Attach@?$CSimpleStringT@_W$00@ATL@@AEAAXPEAUCStringData@2@@Z
extern "C" void impl__Attach___CSimpleStringT__W_00_ATL__AEAAXPEAUCStringData_2__Z(CSimpleStringW* self,CStringData* data) {self->Attach(data);}
// Symbol: ?CloneData@?$CSimpleStringT@_W$00@ATL@@CAPEAUCStringData@2@PEAU32@@Z
extern "C" CStringData* impl__CloneData___CSimpleStringT__W_00_ATL__CAPEAUCStringData_2_PEAU32__Z(CStringData* data) {return CSimpleStringW::CloneData(data);}
// Symbol: ?Concatenate@?$CSimpleStringT@_W$00@ATL@@KAXAEAV12@PEB_WH1H@Z
extern "C" void impl__Concatenate___CSimpleStringT__W_00_ATL__KAXAEAV12_PEB_WH1H_Z(CSimpleStringW* self,const wchar_t* first,int firstLength,const wchar_t* second,int secondLength) {CSimpleStringW::Concatenate(*self,first,firstLength,second,secondLength);}
// Symbol: ?CopyChars@?$CSimpleStringT@_W$00@ATL@@SAXPEA_WPEB_WH@Z
extern "C" void impl__CopyChars___CSimpleStringT__W_00_ATL__SAXPEA_WPEB_WH_Z(wchar_t* output,const wchar_t* source,int length) {if(source)memcpy(output,source,size_t(int64_t(length))*sizeof(wchar_t));}
// Symbol: ?CopyCharsOverlapped@?$CSimpleStringT@_W$00@ATL@@SAXPEA_WPEB_WH@Z
extern "C" void impl__CopyCharsOverlapped___CSimpleStringT__W_00_ATL__SAXPEA_WPEB_WH_Z(wchar_t* output,const wchar_t* source,int length) {memmove(output,source,size_t(int64_t(length))*sizeof(wchar_t));}
// Symbol: ?Fork@?$CSimpleStringT@_W$00@ATL@@AEAAXH@Z
extern "C" void impl__Fork___CSimpleStringT__W_00_ATL__AEAAXH_Z(CSimpleStringW* self,int length) {self->Fork(length);}
// Symbol: ?FreeExtra@?$CSimpleStringT@_W$00@ATL@@QEAAXXZ
extern "C" void impl__FreeExtra___CSimpleStringT__W_00_ATL__QEAAXXZ(CSimpleStringW* self) {self->FreeExtra();}
// Symbol: ?GetData@?$CSimpleStringT@_W$00@ATL@@AEBAPEAUCStringData@2@XZ
extern "C" CStringData* impl__GetData___CSimpleStringT__W_00_ATL__AEBAPEAUCStringData_2_XZ(const CSimpleStringW* self) {return self->GetData();}
// Symbol: ?GetManager@?$CSimpleStringT@_W$00@ATL@@QEBAPEAUIAtlStringMgr@2@XZ
extern "C" IAtlStringMgr* impl__GetManager___CSimpleStringT__W_00_ATL__QEBAPEAUIAtlStringMgr_2_XZ(const CSimpleStringW* self) {return self->GetManager();}
// Symbol: ?Preallocate@?$CSimpleStringT@_W$00@ATL@@QEAAXH@Z
extern "C" void impl__Preallocate___CSimpleStringT__W_00_ATL__QEAAXH_Z(CSimpleStringW* self,int length) {self->Preallocate(length);}
// Symbol: ?PrepareWrite2@?$CSimpleStringT@_W$00@ATL@@AEAAXH@Z
extern "C" void impl__PrepareWrite2___CSimpleStringT__W_00_ATL__AEAAXH_Z(CSimpleStringW* self,int length) {self->PrepareWrite2(length);}
// Symbol: ?PrepareWrite@?$CSimpleStringT@_W$00@ATL@@AEAAPEA_WH@Z
extern "C" wchar_t* impl__PrepareWrite___CSimpleStringT__W_00_ATL__AEAAPEA_WH_Z(CSimpleStringW* self,int length) {return self->GetBuffer(length);}
// Symbol: ?Reallocate@?$CSimpleStringT@_W$00@ATL@@AEAAXH@Z
extern "C" void impl__Reallocate___CSimpleStringT__W_00_ATL__AEAAXH_Z(CSimpleStringW* self,int length) {self->Reallocate(length);}
// Symbol: ?SetAt@?$CSimpleStringT@_W$00@ATL@@QEAAXH_W@Z
extern "C" void impl__SetAt___CSimpleStringT__W_00_ATL__QEAAXH_W_Z(CSimpleStringW* self,int index,wchar_t value) {self->SetAt(index,value);}
// Symbol: ?SetLength@?$CSimpleStringT@_W$00@ATL@@AEAAXH@Z
extern "C" void impl__SetLength___CSimpleStringT__W_00_ATL__AEAAXH_Z(CSimpleStringW* self,int length) {self->SetLength(length);}
// Symbol: ?SetManager@?$CSimpleStringT@_W$00@ATL@@QEAAXPEAUIAtlStringMgr@2@@Z
extern "C" void impl__SetManager___CSimpleStringT__W_00_ATL__QEAAXPEAUIAtlStringMgr_2__Z(CSimpleStringW* self,IAtlStringMgr* manager) {self->SetManager(manager);}
// Symbol: ?SetString@?$CSimpleStringT@_W$00@ATL@@QEAAXPEB_W@Z
extern "C" void impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_W_Z(CSimpleStringW* self,const wchar_t* source) {self->SetString(source);}
// Symbol: ?SetString@?$CSimpleStringT@_W$00@ATL@@QEAAXPEB_WH@Z
extern "C" void impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_WH_Z(CSimpleStringW* self,const wchar_t* source,int length) {self->SetString(source,length);}
// Symbol: ?StringLength@?$CSimpleStringT@_W$00@ATL@@SAHPEBD@Z
extern "C" int impl__StringLength___CSimpleStringT__W_00_ATL__SAHPEBD_Z(const char* text) {return CSimpleStringW::StringLength(text);}
// Symbol: ?StringLength@?$CSimpleStringT@_W$00@ATL@@SAHPEB_W@Z
extern "C" int impl__StringLength___CSimpleStringT__W_00_ATL__SAHPEB_W_Z(const wchar_t* text) {return CSimpleStringW::StringLength(text);}
// Symbol: ?ThrowMemoryException@?$CSimpleStringT@_W$00@ATL@@KAXXZ
extern "C" void impl__ThrowMemoryException___CSimpleStringT__W_00_ATL__KAXXZ() {CSimpleStringW::ThrowMemoryException();}
// Symbol: ?Truncate@?$CSimpleStringT@_W$00@ATL@@QEAAXH@Z
extern "C" void impl__Truncate___CSimpleStringT__W_00_ATL__QEAAXH_Z(CSimpleStringW* self,int length) {self->Truncate(length);}
