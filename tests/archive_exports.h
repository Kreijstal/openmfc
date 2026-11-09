#pragma once
#include "openmfc/afx.h"
extern "C" void impl__FillBuffer_CArchive__QEAAXI_Z(CArchive* self,UINT count);
extern "C" UINT impl__GetObjectSchema_CArchive__QEAAIXZ(CArchive* self);
extern "C" void impl__WriteCount_CArchive__QEAAX_K_Z(CArchive* self,ULONGLONG count);
extern "C" ULONGLONG impl__ReadCount_CArchive__QEAA_KXZ(CArchive* self);
extern "C" void impl__CheckCount_CArchive__QEAAXXZ(CArchive* self);
extern "C" CArchive* impl___0CArchive__QEAA_PEAVCFile__IHPEAX_Z(void* storage,CFile* file,UINT mode,int size,void* buffer);
extern "C" void impl___1CArchive__QEAA_XZ(CArchive* self);
extern "C" void impl__Abort_CArchive__QEAAXXZ(CArchive* self);
extern "C" void impl__Close_CArchive__QEAAXXZ(CArchive* self);
extern "C" void impl__Flush_CArchive__QEAAXXZ(CArchive* self);
extern "C" UINT impl__Read_CArchive__QEAAIPEAXI_Z(CArchive* self,void* output,UINT count);
extern "C" void impl__Write_CArchive__QEAAXPEBXI_Z(CArchive* self,const void* input,UINT count);
#define ARCHIVE_API(X) \
 X(FillBuffer,impl__FillBuffer_CArchive__QEAAXI_Z,"?FillBuffer@CArchive@@QEAAXI@Z",0x1d1cc0) \
 X(GetObjectSchema,impl__GetObjectSchema_CArchive__QEAAIXZ,"?GetObjectSchema@CArchive@@QEAAIXZ",0x1d2950) \
 X(WriteCount,impl__WriteCount_CArchive__QEAAX_K_Z,"?WriteCount@CArchive@@QEAAX_K@Z",0x1d1e60) \
 X(ReadCount,impl__ReadCount_CArchive__QEAA_KXZ,"?ReadCount@CArchive@@QEAA_KXZ",0x1d1ff0) \
 X(CheckCount,impl__CheckCount_CArchive__QEAAXXZ,"?CheckCount@CArchive@@QEAAXXZ",0x1d23f0) \
 X(ctor,impl___0CArchive__QEAA_PEAVCFile__IHPEAX_Z,"??0CArchive@@QEAA@PEAVCFile@@IHPEAX@Z",0x1d1550) \
 X(dtor,impl___1CArchive__QEAA_XZ,"??1CArchive@@QEAA@XZ",0x1d1700) \
 X(Abort,impl__Abort_CArchive__QEAAXXZ,"?Abort@CArchive@@QEAAXXZ",0x1d1760) \
 X(Close,impl__Close_CArchive__QEAAXXZ,"?Close@CArchive@@QEAAXXZ",0x1d1820) \
 X(Flush,impl__Flush_CArchive__QEAAXXZ,"?Flush@CArchive@@QEAAXXZ",0x1d1be0) \
 X(Read,impl__Read_CArchive__QEAAIPEAXI_Z,"?Read@CArchive@@QEAAIPEAXI@Z",0x1d1840) \
 X(Write,impl__Write_CArchive__QEAAXPEBXI_Z,"?Write@CArchive@@QEAAXPEBXI@Z",0x1d1a70)
