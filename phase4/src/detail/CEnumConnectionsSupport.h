#pragma once
#include "detail/CCmdTargetConnectionsSupport.h"
extern "C" void* MS_ABI impl___0CEnumConnections__QEAA_PEBXI_Z(void*,const void*,unsigned int);
extern "C" void MS_ABI impl___1CEnumConnections__UEAA_XZ(void*);
extern "C" void MS_ABI impl__AddConnection_CEnumConnections__QEAAXPEAUtagCONNECTDATA___Z(void*,void*);
extern "C" void* MS_ABI impl__OnClone_CEnumConnections__MEAAPEAVCEnumArray__XZ(void*);
extern "C" int MS_ABI impl__OnNext_CEnumConnections__MEAAHPEAX_Z(void*,void*);
void* OpenMfcCreateConnectionEnumerator();
IEnumConnections* OpenMfcConnectionEnumeratorInterface(void*);
