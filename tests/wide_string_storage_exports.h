#pragma once
#include "openmfc/afxstr.h"
#include <utility>
// Each wide provider is selected explicitly, including native folded aliases.
extern "C" bool impl__IsEmpty___CSimpleStringT__W_00_ATL__QEBA_NXZ(const CSimpleStringW* self);
extern "C" const wchar_t* impl__GetString___CSimpleStringT__W_00_ATL__QEBAPEB_WXZ(const CSimpleStringW* self);
extern "C" wchar_t impl___A__CSimpleStringT__W_00_ATL__QEBA_WH_Z(const CSimpleStringW* self,int index);
extern "C" int impl__GetLength___CSimpleStringT__W_00_ATL__QEBAHXZ(const CSimpleStringW* self);
extern "C" void impl__Empty___CSimpleStringT__W_00_ATL__QEAAXXZ(CSimpleStringW* self);
extern "C" wchar_t impl__GetAt___CSimpleStringT__W_00_ATL__QEBA_WH_Z(const CSimpleStringW* self,int index);
extern "C" int impl__GetAllocLength___CSimpleStringT__W_00_ATL__QEBAHXZ(const CSimpleStringW* self);
extern "C" CSimpleStringW* impl___Y__CSimpleStringT__W_00_ATL__QEAAAEAV01_AEBV01__Z(CSimpleStringW* self,const CSimpleStringW* source);
extern "C" CSimpleStringW* impl___Y__CSimpleStringT__W_00_ATL__QEAAAEAV01_D_Z(CSimpleStringW* self,char value);
extern "C" CSimpleStringW* impl___Y__CSimpleStringT__W_00_ATL__QEAAAEAV01_E_Z(CSimpleStringW* self,unsigned char value);
extern "C" CSimpleStringW* impl___Y__CSimpleStringT__W_00_ATL__QEAAAEAV01_PEB_W_Z(CSimpleStringW* self,const wchar_t* source);
extern "C" CSimpleStringW* impl___Y__CSimpleStringT__W_00_ATL__QEAAAEAV01__W_Z(CSimpleStringW* self,wchar_t value);
extern "C" void impl__AppendChar___CSimpleStringT__W_00_ATL__QEAAX_W_Z(CSimpleStringW* self,wchar_t value);
extern "C" void impl__Attach___CSimpleStringT__W_00_ATL__AEAAXPEAUCStringData_2__Z(CSimpleStringW* self,CStringData* data);
extern "C" CStringData* impl__CloneData___CSimpleStringT__W_00_ATL__CAPEAUCStringData_2_PEAU32__Z(CStringData* data);
extern "C" void impl__Concatenate___CSimpleStringT__W_00_ATL__KAXAEAV12_PEB_WH1H_Z(CSimpleStringW* self,const wchar_t* first,int firstLength,const wchar_t* second,int secondLength);
extern "C" void impl__CopyChars___CSimpleStringT__W_00_ATL__SAXPEA_WPEB_WH_Z(wchar_t* output,const wchar_t* source,int length);
extern "C" void impl__CopyCharsOverlapped___CSimpleStringT__W_00_ATL__SAXPEA_WPEB_WH_Z(wchar_t* output,const wchar_t* source,int length);
extern "C" void impl__Fork___CSimpleStringT__W_00_ATL__AEAAXH_Z(CSimpleStringW* self,int length);
extern "C" void impl__FreeExtra___CSimpleStringT__W_00_ATL__QEAAXXZ(CSimpleStringW* self);
extern "C" CStringData* impl__GetData___CSimpleStringT__W_00_ATL__AEBAPEAUCStringData_2_XZ(const CSimpleStringW* self);
extern "C" IAtlStringMgr* impl__GetManager___CSimpleStringT__W_00_ATL__QEBAPEAUIAtlStringMgr_2_XZ(const CSimpleStringW* self);
extern "C" void impl__Preallocate___CSimpleStringT__W_00_ATL__QEAAXH_Z(CSimpleStringW* self,int length);
extern "C" void impl__PrepareWrite2___CSimpleStringT__W_00_ATL__AEAAXH_Z(CSimpleStringW* self,int length);
extern "C" wchar_t* impl__PrepareWrite___CSimpleStringT__W_00_ATL__AEAAPEA_WH_Z(CSimpleStringW* self,int length);
extern "C" void impl__Reallocate___CSimpleStringT__W_00_ATL__AEAAXH_Z(CSimpleStringW* self,int length);
extern "C" void impl__SetAt___CSimpleStringT__W_00_ATL__QEAAXH_W_Z(CSimpleStringW* self,int index,wchar_t value);
extern "C" void impl__SetLength___CSimpleStringT__W_00_ATL__AEAAXH_Z(CSimpleStringW* self,int length);
extern "C" void impl__SetManager___CSimpleStringT__W_00_ATL__QEAAXPEAUIAtlStringMgr_2__Z(CSimpleStringW* self,IAtlStringMgr* manager);
extern "C" void impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_W_Z(CSimpleStringW* self,const wchar_t* source);
extern "C" void impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_WH_Z(CSimpleStringW* self,const wchar_t* source,int length);
extern "C" int impl__StringLength___CSimpleStringT__W_00_ATL__SAHPEBD_Z(const char* text);
extern "C" int impl__StringLength___CSimpleStringT__W_00_ATL__SAHPEB_W_Z(const wchar_t* text);
extern "C" void impl__ThrowMemoryException___CSimpleStringT__W_00_ATL__KAXXZ();
extern "C" void impl__Truncate___CSimpleStringT__W_00_ATL__QEAAXH_Z(CSimpleStringW* self,int length);
extern "C" wchar_t* impl__GetBuffer___CSimpleStringT__W_00_ATL__QEAAPEA_WH_Z(CString* self,int minimum);
extern "C" wchar_t* impl__GetBuffer___CSimpleStringT__W_00_ATL__QEAAPEA_WXZ(CString* self);
extern "C" wchar_t* impl__GetBufferSetLength___CSimpleStringT__W_00_ATL__QEAAPEA_WH_Z(CString* self,int length);
extern "C" void impl__ReleaseBuffer___CSimpleStringT__W_00_ATL__QEAAXH_Z(CString* self,int length);
extern "C" void impl__ReleaseBufferSetLength___CSimpleStringT__W_00_ATL__QEAAXH_Z(CString* self,int length);
extern "C" wchar_t* impl__LockBuffer___CSimpleStringT__W_00_ATL__QEAAPEA_WXZ(CString* self);
extern "C" void impl__UnlockBuffer___CSimpleStringT__W_00_ATL__QEAAXXZ(CString* self);
inline const std::pair<UINT_PTR,void*> wideStorageProviders[]={
    {0x9720,reinterpret_cast<void*>(&impl__IsEmpty___CSimpleStringT__W_00_ATL__QEBA_NXZ)},
    {0x2e10,reinterpret_cast<void*>(&impl__GetString___CSimpleStringT__W_00_ATL__QEBAPEB_WXZ)},
    {0x2d70,reinterpret_cast<void*>(&impl___A__CSimpleStringT__W_00_ATL__QEBA_WH_Z)},
    {0x3080,reinterpret_cast<void*>(&impl__GetLength___CSimpleStringT__W_00_ATL__QEBAHXZ)},
    {0x33b0,reinterpret_cast<void*>(&impl__Empty___CSimpleStringT__W_00_ATL__QEAAXXZ)},
    {0x2d70,reinterpret_cast<void*>(&impl__GetAt___CSimpleStringT__W_00_ATL__QEBA_WH_Z)},
    {0x2e20,reinterpret_cast<void*>(&impl__GetAllocLength___CSimpleStringT__W_00_ATL__QEBAHXZ)},
    {0x2cb0,reinterpret_cast<void*>(&impl___Y__CSimpleStringT__W_00_ATL__QEAAAEAV01_AEBV01__Z)},
    {0x3320,reinterpret_cast<void*>(&impl___Y__CSimpleStringT__W_00_ATL__QEAAAEAV01_D_Z)},
    {0x2b42f0,reinterpret_cast<void*>(&impl___Y__CSimpleStringT__W_00_ATL__QEAAAEAV01_E_Z)},
    {0x12330,reinterpret_cast<void*>(&impl___Y__CSimpleStringT__W_00_ATL__QEAAAEAV01_PEB_W_Z)},
    {0x122d0,reinterpret_cast<void*>(&impl___Y__CSimpleStringT__W_00_ATL__QEAAAEAV01__W_Z)},
    {0x32b0,reinterpret_cast<void*>(&impl__AppendChar___CSimpleStringT__W_00_ATL__QEAAX_W_Z)},
    {0x33a0,reinterpret_cast<void*>(&impl__Attach___CSimpleStringT__W_00_ATL__AEAAXPEAUCStringData_2__Z)},
    {0xdd40,reinterpret_cast<void*>(&impl__CloneData___CSimpleStringT__W_00_ATL__CAPEAUCStringData_2_PEAU32__Z)},
    {0x12c00,reinterpret_cast<void*>(&impl__Concatenate___CSimpleStringT__W_00_ATL__KAXAEAV12_PEB_WH1H_Z)},
    {0x2b4600,reinterpret_cast<void*>(&impl__CopyChars___CSimpleStringT__W_00_ATL__SAXPEA_WPEB_WH_Z)},
    {0x2b45f0,reinterpret_cast<void*>(&impl__CopyCharsOverlapped___CSimpleStringT__W_00_ATL__SAXPEA_WPEB_WH_Z)},
    {0x3090,reinterpret_cast<void*>(&impl__Fork___CSimpleStringT__W_00_ATL__AEAAXH_Z)},
    {0x2b46b0,reinterpret_cast<void*>(&impl__FreeExtra___CSimpleStringT__W_00_ATL__QEAAXXZ)},
    {0x3360,reinterpret_cast<void*>(&impl__GetData___CSimpleStringT__W_00_ATL__AEBAPEAUCStringData_2_XZ)},
    {0xd830,reinterpret_cast<void*>(&impl__GetManager___CSimpleStringT__W_00_ATL__QEBAPEAUIAtlStringMgr_2_XZ)},
    {0x133ba0,reinterpret_cast<void*>(&impl__Preallocate___CSimpleStringT__W_00_ATL__QEAAXH_Z)},
    {0x31c0,reinterpret_cast<void*>(&impl__PrepareWrite2___CSimpleStringT__W_00_ATL__AEAAXH_Z)},
    {0x3230,reinterpret_cast<void*>(&impl__PrepareWrite___CSimpleStringT__W_00_ATL__AEAAPEA_WH_Z)},
    {0x3170,reinterpret_cast<void*>(&impl__Reallocate___CSimpleStringT__W_00_ATL__AEAAXH_Z)},
    {0x2d00,reinterpret_cast<void*>(&impl__SetAt___CSimpleStringT__W_00_ATL__QEAAXH_W_Z)},
    {0x3280,reinterpret_cast<void*>(&impl__SetLength___CSimpleStringT__W_00_ATL__AEAAXH_Z)},
    {0x2b4620,reinterpret_cast<void*>(&impl__SetManager___CSimpleStringT__W_00_ATL__QEAAXPEAUIAtlStringMgr_2__Z)},
    {0x2f50,reinterpret_cast<void*>(&impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_W_Z)},
    {0x2e30,reinterpret_cast<void*>(&impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_WH_Z)},
    {0x2b45e0,reinterpret_cast<void*>(&impl__StringLength___CSimpleStringT__W_00_ATL__SAHPEBD_Z)},
    {0x2f40,reinterpret_cast<void*>(&impl__StringLength___CSimpleStringT__W_00_ATL__SAHPEB_W_Z)},
    {0x3160,reinterpret_cast<void*>(&impl__ThrowMemoryException___CSimpleStringT__W_00_ATL__KAXXZ)},
    {0x126f0,reinterpret_cast<void*>(&impl__Truncate___CSimpleStringT__W_00_ATL__QEAAXH_Z)},
    {0x3230,reinterpret_cast<void*>(&impl__GetBuffer___CSimpleStringT__W_00_ATL__QEAAPEA_WH_Z)},
    {0x2cd0,reinterpret_cast<void*>(&impl__GetBuffer___CSimpleStringT__W_00_ATL__QEAAPEA_WXZ)},
    {0x77850,reinterpret_cast<void*>(&impl__GetBufferSetLength___CSimpleStringT__W_00_ATL__QEAAPEA_WH_Z)},
    {0x777f0,reinterpret_cast<void*>(&impl__ReleaseBuffer___CSimpleStringT__W_00_ATL__QEAAXH_Z)},
    {0x3280,reinterpret_cast<void*>(&impl__ReleaseBufferSetLength___CSimpleStringT__W_00_ATL__QEAAXH_Z)},
    {0x203f60,reinterpret_cast<void*>(&impl__LockBuffer___CSimpleStringT__W_00_ATL__QEAAPEA_WXZ)},
    {0x2b4680,reinterpret_cast<void*>(&impl__UnlockBuffer___CSimpleStringT__W_00_ATL__QEAAXXZ)},
};
