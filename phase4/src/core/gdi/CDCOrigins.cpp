// Native aggregate-return ABI and output/attribute routing for DC origins.
#define OPENMFC_APPCORE_IMPL
#include "detail/GdicoreSupport.h"
// Bind member calls and vtable slots to the explicit hidden-return entries.
// A C++ return-by-value wrapper can defer writing the caller's result until
// after the GDI calls when NRVO is disabled, breaking native output aliasing.
#pragma comment(linker,"/alternatename:?SetViewportOrg@CDC@@UEAA?AVCPoint@@HH@Z=impl__SetViewportOrg_CDC__UEAA_AVCPoint__HH_Z")
#pragma comment(linker,"/alternatename:?OffsetViewportOrg@CDC@@UEAA?AVCPoint@@HH@Z=impl__OffsetViewportOrg_CDC__UEAA_AVCPoint__HH_Z")
#pragma comment(linker,"/alternatename:?SetWindowOrg@CDC@@QEAA?AVCPoint@@HH@Z=impl__SetWindowOrg_CDC__QEAA_AVCPoint__HH_Z")
#pragma comment(linker,"/alternatename:?OffsetWindowOrg@CDC@@QEAA?AVCPoint@@HH@Z=impl__OffsetWindowOrg_CDC__QEAA_AVCPoint__HH_Z")
namespace {
using Origin=BOOL (WINAPI*)(HDC,int,int,POINT*);
CPoint* SetOrigin(CDC* dc,CPoint* result,int x,int y,Origin operation) {
    new(result) CPoint(0,0);
    if(dc->m_hDC!=dc->m_hAttribDC)operation(dc->m_hDC,x,y,reinterpret_cast<POINT*>(result));
    if(dc->m_hAttribDC)operation(dc->m_hAttribDC,x,y,reinterpret_cast<POINT*>(result));
    return result;
}
}
CPoint CDC::SetViewportOrg(const CPoint& point) {return SetViewportOrg(point.x,point.y);}
CPoint CDC::SetWindowOrg(const CPoint& point) {return SetWindowOrg(point.x,point.y);}
// Symbol: ?SetViewportOrg@CDC@@UEAA?AVCPoint@@HH@Z
extern "C" CPoint* MS_ABI impl__SetViewportOrg_CDC__UEAA_AVCPoint__HH_Z(CDC* self,CPoint* result,int x,int y) {return SetOrigin(self,result,x,y,&::SetViewportOrgEx);}
// Symbol: ?OffsetViewportOrg@CDC@@UEAA?AVCPoint@@HH@Z
extern "C" CPoint* MS_ABI impl__OffsetViewportOrg_CDC__UEAA_AVCPoint__HH_Z(CDC* self,CPoint* result,int x,int y) {return SetOrigin(self,result,x,y,&::OffsetViewportOrgEx);}
// Symbol: ?SetWindowOrg@CDC@@QEAA?AVCPoint@@HH@Z
extern "C" CPoint* MS_ABI impl__SetWindowOrg_CDC__QEAA_AVCPoint__HH_Z(CDC* self,CPoint* result,int x,int y) {return SetOrigin(self,result,x,y,&::SetWindowOrgEx);}
// Symbol: ?OffsetWindowOrg@CDC@@QEAA?AVCPoint@@HH@Z
extern "C" CPoint* MS_ABI impl__OffsetWindowOrg_CDC__QEAA_AVCPoint__HH_Z(CDC* self,CPoint* result,int x,int y) {return SetOrigin(self,result,x,y,&::OffsetWindowOrgEx);}
