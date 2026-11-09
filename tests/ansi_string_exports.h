#pragma once
#include "openmfc/afxstr.h"
#include <utility>
// Actual ANSI export providers, paired with the pinned native DLL entry points.
extern "C" void impl__UnlockBuffer___CSimpleStringT_D_00_ATL__QEAAXXZ(CSimpleStringA* self);
extern "C" void impl__Truncate___CSimpleStringT_D_00_ATL__QEAAXH_Z(CSimpleStringA* self,int length);
extern "C" void impl__ThrowMemoryException___CSimpleStringT_D_00_ATL__KAXXZ();
extern "C" int impl__StringLength___CSimpleStringT_D_00_ATL__SAHPEB_W_Z(const wchar_t* text);
extern "C" int impl__StringLength___CSimpleStringT_D_00_ATL__SAHPEBD_Z(const char* text);
extern "C" void impl__SetString___CSimpleStringT_D_00_ATL__QEAAXPEBDH_Z(CSimpleStringA* self,const char* source,int length);
extern "C" void impl__SetString___CSimpleStringT_D_00_ATL__QEAAXPEBD_Z(CSimpleStringA* self,const char* source);
extern "C" void impl__SetManager___CSimpleStringT_D_00_ATL__QEAAXPEAUIAtlStringMgr_2__Z(CSimpleStringA* self,IAtlStringMgr* manager);
extern "C" void impl__SetLength___CSimpleStringT_D_00_ATL__AEAAXH_Z(CSimpleStringA* self,int length);
extern "C" void impl__SetAt___CSimpleStringT_D_00_ATL__QEAAXHD_Z(CSimpleStringA* self,int index,char value);
extern "C" void impl__ReleaseBufferSetLength___CSimpleStringT_D_00_ATL__QEAAXH_Z(CSimpleStringA* self,int length);
extern "C" void impl__ReleaseBuffer___CSimpleStringT_D_00_ATL__QEAAXH_Z(CSimpleStringA* self,int length);
extern "C" void impl__Reallocate___CSimpleStringT_D_00_ATL__AEAAXH_Z(CSimpleStringA* self,int length);
extern "C" char* impl__PrepareWrite___CSimpleStringT_D_00_ATL__AEAAPEADH_Z(CSimpleStringA* self,int length);
extern "C" void impl__PrepareWrite2___CSimpleStringT_D_00_ATL__AEAAXH_Z(CSimpleStringA* self,int length);
extern "C" void impl__Preallocate___CSimpleStringT_D_00_ATL__QEAAXH_Z(CSimpleStringA* self,int length);
extern "C" char* impl__LockBuffer___CSimpleStringT_D_00_ATL__QEAAPEADXZ(CSimpleStringA* self);
extern "C" bool impl__IsEmpty___CSimpleStringT_D_00_ATL__QEBA_NXZ(const CSimpleStringA* self);
extern "C" const char* impl__GetString___CSimpleStringT_D_00_ATL__QEBAPEBDXZ(const CSimpleStringA* self);
extern "C" IAtlStringMgr* impl__GetManager___CSimpleStringT_D_00_ATL__QEBAPEAUIAtlStringMgr_2_XZ(const CSimpleStringA* self);
extern "C" int impl__GetLength___CSimpleStringT_D_00_ATL__QEBAHXZ(const CSimpleStringA* self);
extern "C" CStringData* impl__GetData___CSimpleStringT_D_00_ATL__AEBAPEAUCStringData_2_XZ(const CSimpleStringA* self);
extern "C" char* impl__GetBufferSetLength___CSimpleStringT_D_00_ATL__QEAAPEADH_Z(CSimpleStringA* self,int length);
extern "C" char* impl__GetBuffer___CSimpleStringT_D_00_ATL__QEAAPEADXZ(CSimpleStringA* self);
extern "C" char* impl__GetBuffer___CSimpleStringT_D_00_ATL__QEAAPEADH_Z(CSimpleStringA* self,int length);
extern "C" char impl__GetAt___CSimpleStringT_D_00_ATL__QEBADH_Z(const CSimpleStringA* self,int index);
extern "C" int impl__GetAllocLength___CSimpleStringT_D_00_ATL__QEBAHXZ(const CSimpleStringA* self);
extern "C" void impl__FreeExtra___CSimpleStringT_D_00_ATL__QEAAXXZ(CSimpleStringA* self);
extern "C" void impl__Fork___CSimpleStringT_D_00_ATL__AEAAXH_Z(CSimpleStringA* self,int length);
extern "C" void impl__Empty___CSimpleStringT_D_00_ATL__QEAAXXZ(CSimpleStringA* self);
extern "C" void impl__CopyCharsOverlapped___CSimpleStringT_D_00_ATL__SAXPEADPEBDH_Z(char* output,const char* source,int length);
extern "C" void impl__CopyChars___CSimpleStringT_D_00_ATL__SAXPEADPEBDH_Z(char* output,const char* source,int length);
extern "C" void impl__Concatenate___CSimpleStringT_D_00_ATL__KAXAEAV12_PEBDH1H_Z(CSimpleStringA* self,const char* first,int firstLength,const char* second,int secondLength);
extern "C" CStringData* impl__CloneData___CSimpleStringT_D_00_ATL__CAPEAUCStringData_2_PEAU32__Z(CStringData* data);
extern "C" void impl__Attach___CSimpleStringT_D_00_ATL__AEAAXPEAUCStringData_2__Z(CSimpleStringA* self,CStringData* data);
extern "C" void impl__AppendChar___CSimpleStringT_D_00_ATL__QEAAXD_Z(CSimpleStringA* self,char value);
extern "C" void impl__Append___CSimpleStringT_D_00_ATL__QEAAXPEBDH_Z(CSimpleStringA* self,const char* source,int length);
extern "C" void impl__Append___CSimpleStringT_D_00_ATL__QEAAXPEBD_Z(CSimpleStringA* self,const char* source);
extern "C" void impl__Append___CSimpleStringT_D_00_ATL__QEAAXAEBV12__Z(CSimpleStringA* self,const CSimpleStringA* source);
extern "C" CSimpleStringA* impl___Y__CSimpleStringT_D_00_ATL__QEAAAEAV01__W_Z(CSimpleStringA* self,wchar_t value);
extern "C" CSimpleStringA* impl___Y__CSimpleStringT_D_00_ATL__QEAAAEAV01_PEBD_Z(CSimpleStringA* self,const char* source);
extern "C" CSimpleStringA* impl___Y__CSimpleStringT_D_00_ATL__QEAAAEAV01_E_Z(CSimpleStringA* self,unsigned char value);
extern "C" CSimpleStringA* impl___Y__CSimpleStringT_D_00_ATL__QEAAAEAV01_D_Z(CSimpleStringA* self,char value);
extern "C" CSimpleStringA* impl___Y__CSimpleStringT_D_00_ATL__QEAAAEAV01_AEBV01__Z(CSimpleStringA* self,const CSimpleStringA* source);
extern "C" const char* impl___B__CSimpleStringT_D_00_ATL__QEBAPEBDXZ(const CSimpleStringA* self);
extern "C" CSimpleStringA* impl___B__CSimpleStringT_D_00_ATL__QEAAAEAV__CSimpleStringT_D_0A__1_XZ(CSimpleStringA* self);
extern "C" char impl___A__CSimpleStringT_D_00_ATL__QEBADH_Z(const CSimpleStringA* self,int index);
extern "C" CSimpleStringA* impl___4__CSimpleStringT_D_00_ATL__QEAAAEAV01_PEBD_Z(CSimpleStringA* self,const char* source);
extern "C" CSimpleStringA* impl___4__CSimpleStringT_D_00_ATL__QEAAAEAV01_AEBV__CSimpleStringT_D_0A__1__Z(CSimpleStringA* self,const CSimpleStringA* source);
extern "C" CSimpleStringA* impl___4__CSimpleStringT_D_00_ATL__QEAAAEAV01_AEBV01__Z(CSimpleStringA* self,const CSimpleStringA* source);
extern "C" void impl___1__CSimpleStringT_D_00_ATL__QEAA_XZ(CSimpleStringA* self);
extern "C" CSimpleStringA* impl___0__CSimpleStringT_D_00_ATL__QEAA_PEBDPEAUIAtlStringMgr_1__Z(CSimpleStringA* self,const char* source,IAtlStringMgr* manager);
extern "C" CSimpleStringA* impl___0__CSimpleStringT_D_00_ATL__QEAA_PEBDHPEAUIAtlStringMgr_1__Z(CSimpleStringA* self,const char* source,int length,IAtlStringMgr* manager);
extern "C" CSimpleStringA* impl___0__CSimpleStringT_D_00_ATL__QEAA_PEAUIAtlStringMgr_1__Z(CSimpleStringA* self,IAtlStringMgr* manager);
extern "C" CSimpleStringA* impl___0__CSimpleStringT_D_00_ATL__QEAA_AEBV__CSimpleStringT_D_0A__1__Z(CSimpleStringA* self,const CSimpleStringA* source);
extern "C" CSimpleStringA* impl___0__CSimpleStringT_D_00_ATL__QEAA_AEBV01__Z(CSimpleStringA* self,const CSimpleStringA* source);
extern "C" int impl__LoadStringW___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAHPEAUHINSTANCE____IG_Z(CStringA* self,HINSTANCE module,UINT id,WORD language);
extern "C" int impl__LoadStringW___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(CStringA* self,HINSTANCE module,UINT id);
extern "C" int impl__LoadStringW___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAHI_Z(CStringA* self,UINT id);
extern "C" CStringA* impl___4__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01__W_Z(CStringA* self,wchar_t value);
extern "C" CStringA* impl___4__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_PEB_W_Z(CStringA* self,const wchar_t* source);
extern "C" CStringA* impl___4__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_PEBE_Z(CStringA* self,const unsigned char* source);
extern "C" CStringA* impl___4__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_PEBD_Z(CStringA* self,const char* source);
extern "C" CStringA* impl___4__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_D_Z(CStringA* self,char value);
extern "C" CStringA* impl___4__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_AEBV01__Z(CStringA* self,const CStringA* source);
extern "C" void impl___1__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_XZ(CStringA* self);
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA__WH_Z(CStringA* self,wchar_t value,int count);
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_XZ(CStringA* self);
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEB_WPEAUIAtlStringMgr_1__Z(CStringA* self,const wchar_t* source,IAtlStringMgr* manager);
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEB_WHPEAUIAtlStringMgr_1__Z(CStringA* self,const wchar_t* source,int length,IAtlStringMgr* manager);
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEB_WH_Z(CStringA* self,const wchar_t* source,int length);
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEB_W_Z(CStringA* self,const wchar_t* source);
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEBEPEAUIAtlStringMgr_1__Z(CStringA* self,const unsigned char* source,IAtlStringMgr* manager);
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEBE_Z(CStringA* self,const unsigned char* source);
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEBDPEAUIAtlStringMgr_1__Z(CStringA* self,const char* source,IAtlStringMgr* manager);
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEBDHPEAUIAtlStringMgr_1__Z(CStringA* self,const char* source,int length,IAtlStringMgr* manager);
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEBDH_Z(CStringA* self,const char* source,int length);
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEBD_Z(CStringA* self,const char* source);
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEA_W_Z(CStringA* self,const wchar_t* source);
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEAUIAtlStringMgr_1__Z(CStringA* self,IAtlStringMgr* manager);
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEAE_Z(CStringA* self,unsigned char* source);
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEAD_Z(CStringA* self,const char* source);
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_DH_Z(CStringA* self,char value,int count);
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_AEBV01__Z(CStringA* self,const CStringA* source);
extern "C" CStringData* impl__Allocate_CAfxStringMgr__UEAAPEAUCStringData_ATL__HH_Z(void* pThis, int nAllocLength, int nCharSize);
extern "C" IAtlStringMgr* impl__Clone_CAfxStringMgr__UEAAPEAUIAtlStringMgr_ATL__XZ(void* pThis);
extern "C" void impl__Free_CAfxStringMgr__UEAAXPEAUCStringData_ATL___Z(void* pThis, CStringData* pData);
extern "C" CStringData* impl__GetNilString_CAfxStringMgr__UEAAPEAUCStringData_ATL__XZ(void* pThis);
extern "C" CStringData* impl__Reallocate_CAfxStringMgr__UEAAPEAUCStringData_ATL__PEAU23_HH_Z(void* pThis, CStringData* pData, int nAllocLength, int nCharSize);
extern "C" CString* impl___Y__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_AEBUtagVARIANT___Z(CString* self,const VARIANT* source);
extern "C" CStringA* impl___Y__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_PEB_W_Z(CStringA* self,const wchar_t* text);
extern "C" CStringA* impl___Y__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_AEBUtagVARIANT___Z(CStringA* self,const VARIANT* source);
extern "C" CString* impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_AEBUtagVARIANT___Z(CString* self,const VARIANT* source);
extern "C" CStringA* impl___4__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_AEBUtagVARIANT___Z(CStringA* self,const VARIANT* source);
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_AEBUtagVARIANT__PEAUIAtlStringMgr_1__Z(CString* self,const VARIANT* source,IAtlStringMgr* manager);
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_AEBUtagVARIANT___Z(CString* self,const VARIANT* source);
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_AEBUtagVARIANT__PEAUIAtlStringMgr_1__Z(CStringA* self,const VARIANT* source,IAtlStringMgr* manager);
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_AEBUtagVARIANT___Z(CStringA* self,const VARIANT* source);
extern "C" void impl__Append___CSimpleStringT__W_00_ATL__QEAAXPEB_WH_Z(CString* self,const wchar_t* text,int count);
extern "C" void impl__Append___CSimpleStringT__W_00_ATL__QEAAXPEB_W_Z(CString* self,const wchar_t* text);
extern "C" void impl__Append___CSimpleStringT__W_00_ATL__QEAAXAEBV12__Z(CString* self,const CString* text);
extern "C" CString* impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_PEBD_Z(CString* self,const char* text);
extern "C" void impl___1__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(CString* self);
extern "C" void impl___1__CSimpleStringT__W_00_ATL__QEAA_XZ(CSimpleStringW* self);
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA__WH_Z(CString* self,wchar_t value,int count);
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(CString* self);
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEB_WPEAUIAtlStringMgr_1__Z(CString* self,const wchar_t* text,IAtlStringMgr* manager);
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEB_WHPEAUIAtlStringMgr_1__Z(CString* self,const wchar_t* text,int count,IAtlStringMgr* manager);
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEB_WH_Z(CString* self,const wchar_t* text,int count);
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEB_W_Z(CString* self,const wchar_t* text);
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBEPEAUIAtlStringMgr_1__Z(CString* self,const unsigned char* text,IAtlStringMgr* manager);
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBE_Z(CString* self,const unsigned char* text);
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBDPEAUIAtlStringMgr_1__Z(CString* self,const char* text,IAtlStringMgr* manager);
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBDHPEAUIAtlStringMgr_1__Z(CString* self,const char* text,int count,IAtlStringMgr* manager);
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBDH_Z(CString* self,const char* text,int count);
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBD_Z(CString* self,const char* text);
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEA_W_Z(CString* self,wchar_t* text);
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEAUIAtlStringMgr_1__Z(CString* self,IAtlStringMgr* manager);
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEAE_Z(CString* self,unsigned char* text);
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEAD_Z(CString* self,char* text);
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_DH_Z(CString* self,char value,int count);
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_AEBV01__Z(CString* self,const CString* source);
extern "C" CSimpleStringW* impl___0__CSimpleStringT__W_00_ATL__QEAA_PEB_WPEAUIAtlStringMgr_1__Z(CSimpleStringW* self,const wchar_t* text,IAtlStringMgr* manager);
extern "C" CSimpleStringW* impl___0__CSimpleStringT__W_00_ATL__QEAA_PEB_WHPEAUIAtlStringMgr_1__Z(CSimpleStringW* self,const wchar_t* text,int count,IAtlStringMgr* manager);
extern "C" CSimpleStringW* impl___0__CSimpleStringT__W_00_ATL__QEAA_PEAUIAtlStringMgr_1__Z(CSimpleStringW* self,IAtlStringMgr* manager);
extern "C" CSimpleStringW* impl___0__CSimpleStringT__W_00_ATL__QEAA_AEBV__CSimpleStringT__W_0A__1__Z(CSimpleStringW* self,const CSimpleStringW* source);
extern "C" CSimpleStringW* impl___0__CSimpleStringT__W_00_ATL__QEAA_AEBV01__Z(CSimpleStringW* self,const CSimpleStringW* source);
extern "C" CString* impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01__W_Z(CString* self,wchar_t source);
extern "C" CString* impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_PEB_W_Z(CString* self,const wchar_t* source);
extern "C" CString* impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_PEBE_Z(CString* self,const unsigned char* source);
extern "C" CString* impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_D_Z(CString* self,char source);
extern "C" CString* impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_AEBV01__Z(CString* self,const CString* source);
extern "C" CSimpleStringW* impl___4__CSimpleStringT__W_00_ATL__QEAAAEAV01_PEB_W_Z(CSimpleStringW* self,const wchar_t* source);
extern "C" CSimpleStringW* impl___4__CSimpleStringT__W_00_ATL__QEAAAEAV01_AEBV__CSimpleStringT__W_0A__1__Z(CSimpleStringW* self,const CSimpleStringW* source);
extern "C" CSimpleStringW* impl___4__CSimpleStringT__W_00_ATL__QEAAAEAV01_AEBV01__Z(CSimpleStringW* self,const CSimpleStringW* source);
static const std::pair<UINT_PTR,void*> ansiStringProviders[]={
    {0x2b4680,reinterpret_cast<void*>(&impl__UnlockBuffer___CSimpleStringT_D_00_ATL__QEAAXXZ)},
    {0x2b6e00,reinterpret_cast<void*>(&impl__Truncate___CSimpleStringT_D_00_ATL__QEAAXH_Z)},
    {0x3160,reinterpret_cast<void*>(&impl__ThrowMemoryException___CSimpleStringT_D_00_ATL__KAXXZ)},
    {0x2f40,reinterpret_cast<void*>(&impl__StringLength___CSimpleStringT_D_00_ATL__SAHPEB_W_Z)},
    {0x2b45e0,reinterpret_cast<void*>(&impl__StringLength___CSimpleStringT_D_00_ATL__SAHPEBD_Z)},
    {0x203fa0,reinterpret_cast<void*>(&impl__SetString___CSimpleStringT_D_00_ATL__QEAAXPEBDH_Z)},
    {0x2b6d60,reinterpret_cast<void*>(&impl__SetString___CSimpleStringT_D_00_ATL__QEAAXPEBD_Z)},
    {0x2b4620,reinterpret_cast<void*>(&impl__SetManager___CSimpleStringT_D_00_ATL__QEAAXPEAUIAtlStringMgr_2__Z)},
    {0x1cd920,reinterpret_cast<void*>(&impl__SetLength___CSimpleStringT_D_00_ATL__AEAAXH_Z)},
    {0x2b6da0,reinterpret_cast<void*>(&impl__SetAt___CSimpleStringT_D_00_ATL__QEAAXHD_Z)},
    {0x1cd920,reinterpret_cast<void*>(&impl__ReleaseBufferSetLength___CSimpleStringT_D_00_ATL__QEAAXH_Z)},
    {0x1fdf40,reinterpret_cast<void*>(&impl__ReleaseBuffer___CSimpleStringT_D_00_ATL__QEAAXH_Z)},
    {0x1cd810,reinterpret_cast<void*>(&impl__Reallocate___CSimpleStringT_D_00_ATL__AEAAXH_Z)},
    {0x1cd8d0,reinterpret_cast<void*>(&impl__PrepareWrite___CSimpleStringT_D_00_ATL__AEAAPEADH_Z)},
    {0x1cd860,reinterpret_cast<void*>(&impl__PrepareWrite2___CSimpleStringT_D_00_ATL__AEAAXH_Z)},
    {0x2b6e60,reinterpret_cast<void*>(&impl__Preallocate___CSimpleStringT_D_00_ATL__QEAAXH_Z)},
    {0x203f20,reinterpret_cast<void*>(&impl__LockBuffer___CSimpleStringT_D_00_ATL__QEAAPEADXZ)},
    {0x9720,reinterpret_cast<void*>(&impl__IsEmpty___CSimpleStringT_D_00_ATL__QEBA_NXZ)},
    {0x2e10,reinterpret_cast<void*>(&impl__GetString___CSimpleStringT_D_00_ATL__QEBAPEBDXZ)},
    {0xd830,reinterpret_cast<void*>(&impl__GetManager___CSimpleStringT_D_00_ATL__QEBAPEAUIAtlStringMgr_2_XZ)},
    {0x3080,reinterpret_cast<void*>(&impl__GetLength___CSimpleStringT_D_00_ATL__QEBAHXZ)},
    {0x3360,reinterpret_cast<void*>(&impl__GetData___CSimpleStringT_D_00_ATL__AEBAPEAUCStringData_2_XZ)},
    {0x1fdf90,reinterpret_cast<void*>(&impl__GetBufferSetLength___CSimpleStringT_D_00_ATL__QEAAPEADH_Z)},
    {0x27ea40,reinterpret_cast<void*>(&impl__GetBuffer___CSimpleStringT_D_00_ATL__QEAAPEADXZ)},
    {0x1cd8d0,reinterpret_cast<void*>(&impl__GetBuffer___CSimpleStringT_D_00_ATL__QEAAPEADH_Z)},
    {0x2b6ea0,reinterpret_cast<void*>(&impl__GetAt___CSimpleStringT_D_00_ATL__QEBADH_Z)},
    {0x2e20,reinterpret_cast<void*>(&impl__GetAllocLength___CSimpleStringT_D_00_ATL__QEBAHXZ)},
    {0x2b6ed0,reinterpret_cast<void*>(&impl__FreeExtra___CSimpleStringT_D_00_ATL__QEAAXXZ)},
    {0x1cd740,reinterpret_cast<void*>(&impl__Fork___CSimpleStringT_D_00_ATL__AEAAXH_Z)},
    {0x1cd950,reinterpret_cast<void*>(&impl__Empty___CSimpleStringT_D_00_ATL__QEAAXXZ)},
    {0x2b6d30,reinterpret_cast<void*>(&impl__CopyCharsOverlapped___CSimpleStringT_D_00_ATL__SAXPEADPEBDH_Z)},
    {0x2b6d40,reinterpret_cast<void*>(&impl__CopyChars___CSimpleStringT_D_00_ATL__SAXPEADPEBDH_Z)},
    {0x2b6c30,reinterpret_cast<void*>(&impl__Concatenate___CSimpleStringT_D_00_ATL__KAXAEAV12_PEBDH1H_Z)},
    {0x2040a0,reinterpret_cast<void*>(&impl__CloneData___CSimpleStringT_D_00_ATL__CAPEAUCStringData_2_PEAU32__Z)},
    {0x33a0,reinterpret_cast<void*>(&impl__Attach___CSimpleStringT_D_00_ATL__AEAAXPEAUCStringData_2__Z)},
    {0x2b6fe0,reinterpret_cast<void*>(&impl__AppendChar___CSimpleStringT_D_00_ATL__QEAAXD_Z)},
    {0x2b7050,reinterpret_cast<void*>(&impl__Append___CSimpleStringT_D_00_ATL__QEAAXPEBDH_Z)},
    {0x2b7150,reinterpret_cast<void*>(&impl__Append___CSimpleStringT_D_00_ATL__QEAAXPEBD_Z)},
    {0x2b6fd0,reinterpret_cast<void*>(&impl__Append___CSimpleStringT_D_00_ATL__QEAAXAEBV12__Z)},
    {0x2b7190,reinterpret_cast<void*>(&impl___Y__CSimpleStringT_D_00_ATL__QEAAAEAV01__W_Z)},
    {0x2b64c0,reinterpret_cast<void*>(&impl___Y__CSimpleStringT_D_00_ATL__QEAAAEAV01_PEBD_Z)},
    {0x2b7200,reinterpret_cast<void*>(&impl___Y__CSimpleStringT_D_00_ATL__QEAAAEAV01_E_Z)},
    {0x2b7200,reinterpret_cast<void*>(&impl___Y__CSimpleStringT_D_00_ATL__QEAAAEAV01_D_Z)},
    {0x2b6500,reinterpret_cast<void*>(&impl___Y__CSimpleStringT_D_00_ATL__QEAAAEAV01_AEBV01__Z)},
    {0x2e10,reinterpret_cast<void*>(&impl___B__CSimpleStringT_D_00_ATL__QEBAPEBDXZ)},
    {0x10130,reinterpret_cast<void*>(&impl___B__CSimpleStringT_D_00_ATL__QEAAAEAV__CSimpleStringT_D_0A__1_XZ)},
    {0x2b6ea0,reinterpret_cast<void*>(&impl___A__CSimpleStringT_D_00_ATL__QEBADH_Z)},
    {0x2b6580,reinterpret_cast<void*>(&impl___4__CSimpleStringT_D_00_ATL__QEAAAEAV01_PEBD_Z)},
    {0x204180,reinterpret_cast<void*>(&impl___4__CSimpleStringT_D_00_ATL__QEAAAEAV01_AEBV__CSimpleStringT_D_0A__1__Z)},
    {0x204180,reinterpret_cast<void*>(&impl___4__CSimpleStringT_D_00_ATL__QEAAAEAV01_AEBV01__Z)},
    {0x2fd0,reinterpret_cast<void*>(&impl___1__CSimpleStringT_D_00_ATL__QEAA_XZ)},
    {0x2b7270,reinterpret_cast<void*>(&impl___0__CSimpleStringT_D_00_ATL__QEAA_PEBDPEAUIAtlStringMgr_1__Z)},
    {0x273840,reinterpret_cast<void*>(&impl___0__CSimpleStringT_D_00_ATL__QEAA_PEBDHPEAUIAtlStringMgr_1__Z)},
    {0x3000,reinterpret_cast<void*>(&impl___0__CSimpleStringT_D_00_ATL__QEAA_PEAUIAtlStringMgr_1__Z)},
    {0x2b6c00,reinterpret_cast<void*>(&impl___0__CSimpleStringT_D_00_ATL__QEAA_AEBV__CSimpleStringT_D_0A__1__Z)},
    {0x2b6c00,reinterpret_cast<void*>(&impl___0__CSimpleStringT_D_00_ATL__QEAA_AEBV01__Z)},
    {0x2b48a0,reinterpret_cast<void*>(&impl__LoadStringW___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAHPEAUHINSTANCE____IG_Z)},
    {0x1cd5d0,reinterpret_cast<void*>(&impl__LoadStringW___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z)},
    {0x1cd6b0,reinterpret_cast<void*>(&impl__LoadStringW___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAHI_Z)},
    {0x2b6520,reinterpret_cast<void*>(&impl___4__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01__W_Z)},
    {0x1cd9e0,reinterpret_cast<void*>(&impl___4__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_PEB_W_Z)},
    {0x2b6580,reinterpret_cast<void*>(&impl___4__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_PEBE_Z)},
    {0x2b6580,reinterpret_cast<void*>(&impl___4__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_PEBD_Z)},
    {0x2b6540,reinterpret_cast<void*>(&impl___4__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_D_Z)},
    {0x204210,reinterpret_cast<void*>(&impl___4__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_AEBV01__Z)},
    {0x2fd0,reinterpret_cast<void*>(&impl___1__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_XZ)},
    {0x2b66f0,reinterpret_cast<void*>(&impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA__WH_Z)},
    {0x3040,reinterpret_cast<void*>(&impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_XZ)},
    {0x2b6ad0,reinterpret_cast<void*>(&impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEB_WPEAUIAtlStringMgr_1__Z)},
    {0x2b65c0,reinterpret_cast<void*>(&impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEB_WHPEAUIAtlStringMgr_1__Z)},
    {0x273740,reinterpret_cast<void*>(&impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEB_WH_Z)},
    {0x1cdad0,reinterpret_cast<void*>(&impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEB_W_Z)},
    {0x2b6960,reinterpret_cast<void*>(&impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEBEPEAUIAtlStringMgr_1__Z)},
    {0x2b6a60,reinterpret_cast<void*>(&impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEBE_Z)},
    {0x2b6b60,reinterpret_cast<void*>(&impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEBDPEAUIAtlStringMgr_1__Z)},
    {0x2b66d0,reinterpret_cast<void*>(&impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEBDHPEAUIAtlStringMgr_1__Z)},
    {0x273910,reinterpret_cast<void*>(&impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEBDH_Z)},
    {0x2b69d0,reinterpret_cast<void*>(&impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEBD_Z)},
    {0x1cdad0,reinterpret_cast<void*>(&impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEA_W_Z)},
    {0x12bc0,reinterpret_cast<void*>(&impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEAUIAtlStringMgr_1__Z)},
    {0x2b69d0,reinterpret_cast<void*>(&impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEAE_Z)},
    {0x2b69d0,reinterpret_cast<void*>(&impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEAD_Z)},
    {0x2b68d0,reinterpret_cast<void*>(&impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_DH_Z)},
    {0x2b6c00,reinterpret_cast<void*>(&impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_AEBV01__Z)},
    {0x2736a0,reinterpret_cast<void*>(&impl__Allocate_CAfxStringMgr__UEAAPEAUCStringData_ATL__HH_Z)},
    {0x10130,reinterpret_cast<void*>(&impl__Clone_CAfxStringMgr__UEAAPEAUIAtlStringMgr_ATL__XZ)},
    {0x22b070,reinterpret_cast<void*>(&impl__Free_CAfxStringMgr__UEAAXPEAUCStringData_ATL___Z)},
    {0x273730,reinterpret_cast<void*>(&impl__GetNilString_CAfxStringMgr__UEAAPEAUCStringData_ATL__XZ)},
    {0x2736f0,reinterpret_cast<void*>(&impl__Reallocate_CAfxStringMgr__UEAAPEAUCStringData_ATL__PEAU23_HH_Z)},
    {0x2b33d0,reinterpret_cast<void*>(&impl___Y__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_AEBUtagVARIANT___Z)},
    {0x2b6420,reinterpret_cast<void*>(&impl___Y__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_PEB_W_Z)},
    {0x2b2db0,reinterpret_cast<void*>(&impl___Y__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_AEBUtagVARIANT___Z)},
    {0x2b3340,reinterpret_cast<void*>(&impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_AEBUtagVARIANT___Z)},
    {0x2b2d30,reinterpret_cast<void*>(&impl___4__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_AEBUtagVARIANT___Z)},
    {0x2b31c0,reinterpret_cast<void*>(&impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_AEBUtagVARIANT__PEAUIAtlStringMgr_1__Z)},
    {0x2b3280,reinterpret_cast<void*>(&impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_AEBUtagVARIANT___Z)},
    {0x2b2be0,reinterpret_cast<void*>(&impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_AEBUtagVARIANT__PEAUIAtlStringMgr_1__Z)},
    {0x2b2c90,reinterpret_cast<void*>(&impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_AEBUtagVARIANT___Z)},
    {0x2ba0,reinterpret_cast<void*>(&impl__Append___CSimpleStringT__W_00_ATL__QEAAXPEB_WH_Z)},
    {0x122f0,reinterpret_cast<void*>(&impl__Append___CSimpleStringT__W_00_ATL__QEAAXPEB_W_Z)},
    {0x2ca0,reinterpret_cast<void*>(&impl__Append___CSimpleStringT__W_00_ATL__QEAAXAEBV12__Z)},
    {0x1cd480,reinterpret_cast<void*>(&impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_PEBD_Z)},
    {0x2fd0,reinterpret_cast<void*>(&impl___1__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ)},
    {0x2fd0,reinterpret_cast<void*>(&impl___1__CSimpleStringT__W_00_ATL__QEAA_XZ)},
    {0x7c260,reinterpret_cast<void*>(&impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA__WH_Z)},
    {0x3040,reinterpret_cast<void*>(&impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ)},
    {0xd790,reinterpret_cast<void*>(&impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEB_WPEAUIAtlStringMgr_1__Z)},
    {0x12a60,reinterpret_cast<void*>(&impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEB_WHPEAUIAtlStringMgr_1__Z)},
    {0x1c0c0,reinterpret_cast<void*>(&impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEB_WH_Z)},
    {0xdcb0,reinterpret_cast<void*>(&impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEB_W_Z)},
    {0x2b4500,reinterpret_cast<void*>(&impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBEPEAUIAtlStringMgr_1__Z)},
    {0x2b4560,reinterpret_cast<void*>(&impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBE_Z)},
    {0x264be0,reinterpret_cast<void*>(&impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBDPEAUIAtlStringMgr_1__Z)},
    {0x2b4380,reinterpret_cast<void*>(&impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBDHPEAUIAtlStringMgr_1__Z)},
    {0x1c0e0,reinterpret_cast<void*>(&impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBDH_Z)},
    {0x1cd550,reinterpret_cast<void*>(&impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEBD_Z)},
    {0xdcb0,reinterpret_cast<void*>(&impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEA_W_Z)},
    {0x12bc0,reinterpret_cast<void*>(&impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEAUIAtlStringMgr_1__Z)},
    {0x1cd550,reinterpret_cast<void*>(&impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEAE_Z)},
    {0x1cd550,reinterpret_cast<void*>(&impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEAD_Z)},
    {0x2b4470,reinterpret_cast<void*>(&impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_DH_Z)},
    {0xd740,reinterpret_cast<void*>(&impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_AEBV01__Z)},
    {0x2b47c0,reinterpret_cast<void*>(&impl___0__CSimpleStringT__W_00_ATL__QEAA_PEB_WPEAUIAtlStringMgr_1__Z)},
    {0x12980,reinterpret_cast<void*>(&impl___0__CSimpleStringT__W_00_ATL__QEAA_PEB_WHPEAUIAtlStringMgr_1__Z)},
    {0x3000,reinterpret_cast<void*>(&impl___0__CSimpleStringT__W_00_ATL__QEAA_PEAUIAtlStringMgr_1__Z)},
    {0xd740,reinterpret_cast<void*>(&impl___0__CSimpleStringT__W_00_ATL__QEAA_AEBV__CSimpleStringT__W_0A__1__Z)},
    {0xd740,reinterpret_cast<void*>(&impl___0__CSimpleStringT__W_00_ATL__QEAA_AEBV01__Z)},
    {0x2b4330,reinterpret_cast<void*>(&impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01__W_Z)},
    {0x2f90,reinterpret_cast<void*>(&impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_PEB_W_Z)},
    {0x2b4370,reinterpret_cast<void*>(&impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_PEBE_Z)},
    {0x2b4310,reinterpret_cast<void*>(&impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_D_Z)},
    {0xdec0,reinterpret_cast<void*>(&impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_AEBV01__Z)},
    {0x2f90,reinterpret_cast<void*>(&impl___4__CSimpleStringT__W_00_ATL__QEAAAEAV01_PEB_W_Z)},
    {0xde30,reinterpret_cast<void*>(&impl___4__CSimpleStringT__W_00_ATL__QEAAAEAV01_AEBV__CSimpleStringT__W_0A__1__Z)},
    {0xde30,reinterpret_cast<void*>(&impl___4__CSimpleStringT__W_00_ATL__QEAAAEAV01_AEBV01__Z)},
};
