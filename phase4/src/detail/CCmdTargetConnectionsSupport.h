#pragma once
#include "detail/CCmdTargetVirtualsSupport.h"
#include <ocidl.h>
class COleConnPtContainer;
extern "C" void MS_ABI impl__EnableConnections_CCmdTarget__QEAAXXZ(CCmdTarget*);
extern "C" HRESULT MS_ABI impl__QueryInterface_COleConnPtContainer__UEAAJAEBU_GUID__PEAPEAX_Z(COleConnPtContainer*,REFIID,void**);
extern "C" ULONG MS_ABI impl__AddRef_COleConnPtContainer__UEAAKXZ(COleConnPtContainer*);
extern "C" ULONG MS_ABI impl__Release_COleConnPtContainer__UEAAKXZ(COleConnPtContainer*);
extern "C" HRESULT MS_ABI impl__EnumConnectionPoints_COleConnPtContainer__UEAAJPEAPEAUIEnumConnectionPoints___Z(COleConnPtContainer*,IEnumConnectionPoints**);
extern "C" HRESULT MS_ABI impl__FindConnectionPoint_COleConnPtContainer__UEAAJAEBU_GUID__PEAPEAUIConnectionPoint___Z(COleConnPtContainer*,REFIID,IConnectionPoint**);
