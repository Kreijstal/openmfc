#pragma once
#include "detail/PrintBitmapSupport.h"
extern "C" {
void MS_ABI impl__OnPrepareDC_CScrollView__UEAAXPEAVCDC__PEAUCPrintInfo___Z(CScrollView*,CDC*,void*);
LRESULT MS_ABI impl__OnPrintClient_CScrollView__QEAA_JPEAVCDC__I_Z(CScrollView*,CDC*,UINT);
CPoint* MS_ABI impl__SetWindowOrg_CDC__QEAA_AVCPoint__HH_Z(CDC*,CPoint*,int,int);
CPoint* MS_ABI impl__OffsetWindowOrg_CDC__QEAA_AVCPoint__HH_Z(CDC*,CPoint*,int,int);
CPoint* MS_ABI impl__SetViewportOrg_CDC__UEAA_AVCPoint__HH_Z(CDC*,CPoint*,int,int);
CPoint* MS_ABI impl__OffsetViewportOrg_CDC__UEAA_AVCPoint__HH_Z(CDC*,CPoint*,int,int);
}
namespace scroll_view_print_test {
using namespace scroll_view_sizing_test;
using Origin=decltype(&impl__SetWindowOrg_CDC__QEAA_AVCPoint__HH_Z);
struct API {
    scroll_view_sizing_test::API sizing;
    decltype(&impl__OnPrepareDC_CScrollView__UEAAXPEAVCDC__PEAUCPrintInfo___Z) prepare;
    decltype(&impl__OnPrintClient_CScrollView__QEAA_JPEAVCDC__I_Z) print;
    Origin origins[4];
    decltype(&impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z) lookup;
    decltype(&impl__DeleteTempMap_CDC__SAXXZ) clear;
    decltype(&impl__Attach_CDC__QEAAHPEAUHDC_____Z) attach;
    decltype(&impl__Detach_CDC__QEAAPEAUHDC____XZ) detach;
};
static const API* active;
static HDC targetHandle,bufferHandle,attributeHandle;
static HBITMAP bitmap;
static void* bitmapBits;
static HGDIOBJ selectedResult;
static CDC* targetDC;
static CDC* bufferDC;
static CDC* originDC;
static CPoint* originOutput;
static BYTE* printInfo;
static PRINTDLGW* parameters;
static bool noBuffer,noBitmap,noAllocation,renderPixels,delegatePreparation;
static int objectResult,originCalls,bltCalls,selectCalls;
static bool failSecondCall;
static size_t allocatedCount,freedCount;
struct Allocation {void* pointer;void (*release)(void*);};
static std::vector<Allocation> allocations;
static void* (*savedAllocate)(size_t);
static void (*savedFree)(void*);
static void* nativeAllocateSlot;
static void* nativeFreeSlot;
static INT64 dcID(HDC h) {return !h?0:h==targetHandle?1:h==bufferHandle?2:h==attributeHandle?3:-1;}
static INT64 objectID(HGDIOBJ h) {return !h?0:h==bitmap?4:h==selectedResult?5:h==HGDI_ERROR?-1:6;}
static int MS_ABI setMap(CDC* dc,int mode) {
    event(100,mode,dc->m_bPrinting,printInfo?*reinterpret_cast<BOOL*>(printInfo+20):-1);
    if(mutation==40){self->m_totalLog={151,157};self->m_totalDev={211,223};self->m_nMapMode=MM_TEXT;}
    if(mutation==42)self->m_bInitialRedraw=TRUE;return -7;
}
static CPoint* MS_ABI setOrigin(CDC* dc,CPoint* result,int x,int y) {
    event(101,x,y,dc->m_bPrinting,printInfo?*reinterpret_cast<BOOL*>(printInfo+20):-1);
    if(mutation==44 && printInfo){*reinterpret_cast<UINT*>(printInfo+24)=2;parameters->nMaxPage=0xffff;}
    new(result) CPoint(17,19);return result;
}
static CSize* MS_ABI windowExtent(CDC*,CSize* result,int x,int y) {
    event(102,x,y);if(mutation==41)self->m_totalDev={311,337};new(result) CSize(23,29);return result;
}
static CSize* MS_ABI viewportExtent(CDC* dc,CSize* result,int x,int y) {
    event(103,x,y);if(mutation==43)dc->m_bPrinting=69;new(result) CSize(31,37);return result;
}
static void MS_ABI prepare(CView*,CDC* dc,void* info) {
    bufferDC=dc;event(110,dcID(dc->m_hDC),dcID(dc->m_hAttribDC),dc->m_bPrinting,info==nullptr,active->lookup(dc->m_hDC)==dc);
    if(mutation==51)dc->m_hDC=dc->m_hAttribDC=attributeHandle;
    if(delegatePreparation)active->prepare(self,dc,info);
}
static void MS_ABI draw(CView*,void* p) {
    auto* dc=static_cast<CDC*>(p);event(111,dcID(dc->m_hDC),dcID(dc->m_hAttribDC),dc->m_bPrinting);
    if(mutation==52)targetDC->m_hDC=attributeHandle;
    if(mutation==54)self->m_hWnd=nullptr;
    if(renderPixels)SetPixel(dc->m_hDC,10,12,RGB(211,37,79));
}
static decltype(&::CreateCompatibleDC) realCompatible;
static decltype(&::CreateDIBSection) realDib;
static decltype(&::GetObjectW) realObject;
static decltype(&::SelectObject) realSelect;
static decltype(&::BitBlt) realBlt;
static decltype(&::DeleteObject) realDeleteObject;
static decltype(&::DeleteDC) realDeleteDC;
static decltype(&::SetMapMode) realMap;
static decltype(&::SetViewportOrgEx) realViewport;
static decltype(&::OffsetViewportOrgEx) realOffsetViewport;
static decltype(&::SetWindowOrgEx) realWindow;
static decltype(&::OffsetWindowOrgEx) realOffsetWindow;
static HDC WINAPI compatible(HDC source) {if(!capture)return realCompatible(source);event(120,dcID(source));return noBuffer?nullptr:bufferHandle;}
static HBITMAP WINAPI dib(HDC dc,const BITMAPINFO* info,UINT usage,void** bits,HANDLE section,DWORD offset) {
    if(!capture)return realDib(dc,info,usage,bits,section,offset);
    event(121,dcID(dc),info->bmiHeader.biWidth,info->bmiHeader.biHeight,info->bmiHeader.biPlanes,info->bmiHeader.biBitCount,info->bmiHeader.biSize,info->bmiHeader.biCompression);
    event(221,usage,section==nullptr,offset);*bits=bitmapBits;return noBitmap?nullptr:bitmap;
}
static int WINAPI getObject(HGDIOBJ h,int size,void* p) {
    if(!capture)return realObject(h,size,p);event(122,objectID(h),size);realObject(bitmap,size,p);return objectResult;
}
static HGDIOBJ WINAPI select(HDC dc,HGDIOBJ h) {
    if(!capture)return realSelect(dc,h);int savedFailure=failure;if(failSecondCall && ++selectCalls==1)failure=0;event(123,dcID(dc),objectID(h));failure=savedFailure;if(mutation==50)targetDC->m_hDC=attributeHandle;return dc?selectedResult:nullptr;
}
static BOOL WINAPI blt(HDC to,int x,int y,int width,int height,HDC from,int sx,int sy,DWORD rop) {
    if(!capture)return realBlt(to,x,y,width,height,from,sx,sy,rop);int savedFailure=failure;if(failSecondCall && ++bltCalls==1)failure=0;event(124,dcID(to),dcID(from),x,y,width,height,rop);failure=savedFailure;event(224,sx,sy);return FALSE;
}
static BOOL WINAPI deleteObject(HGDIOBJ h) {if(!capture)return realDeleteObject(h);event(125,objectID(h));return FALSE;}
static BOOL WINAPI deleteDC(HDC h) {if(!capture)return realDeleteDC(h);event(126,dcID(h),bufferDC?active->lookup(h)!=bufferDC:TRUE);return FALSE;}
static int WINAPI map(HDC h,int mode) {if(!capture)return realMap(h,mode);event(127,dcID(h),mode);return 0;}
static CPoint normalizedPoint(const void* p) {
    HDC value;memcpy(&value,p,sizeof(value));INT64 identity=dcID(value);
    if(identity>0)return CPoint(-100-static_cast<int>(identity),0);
    CPoint result;memcpy(&result,p,sizeof(result));return result;
}
static BOOL origin(HDC dc,int x,int y,POINT* previous,int kind) {
    CPoint before=normalizedPoint(previous),output=originOutput?normalizedPoint(originOutput):CPoint(-1,-1);
    event(128+kind,dcID(dc),x,y,before.x,before.y,output.x,output.y);
    previous->x=17+originCalls;previous->y=19+originCalls;++originCalls;
    if(mutation==55 && originDC)originDC->m_hAttribDC=targetHandle;
    return FALSE;
}
static BOOL WINAPI viewport(HDC h,int x,int y,POINT* p) {return capture?origin(h,x,y,p,0):realViewport(h,x,y,p);}
static BOOL WINAPI offsetViewport(HDC h,int x,int y,POINT* p) {return capture?origin(h,x,y,p,1):realOffsetViewport(h,x,y,p);}
static BOOL WINAPI window(HDC h,int x,int y,POINT* p) {return capture?origin(h,x,y,p,2):realWindow(h,x,y,p);}
static BOOL WINAPI offsetWindow(HDC h,int x,int y,POINT* p) {return capture?origin(h,x,y,p,3):realOffsetWindow(h,x,y,p);}
static void* allocateWith(size_t size,void* (*allocateFn)(size_t),void (*freeFn)(void*)) {
    if(size!=0x438 || !capture)return allocateFn(size);event(132,size);++allocatedCount;if(noAllocation)return nullptr;
    void* p=allocateFn(size);if(p)allocations.push_back({p,freeFn});return p;
}
static void freeWith(void* p,void (*freeFn)(void*)) {
    auto found=std::find_if(allocations.begin(),allocations.end(),[&](const Allocation& a){return a.pointer==p;});
    if(found!=allocations.end()) {freeFn=found->release;allocations.erase(found);if(capture){event(133);++freedCount;}}freeFn(p);
}
static void* allocate(size_t size) {return allocateWith(size,savedAllocate,savedFree);}
static void freeMemory(void* p) {freeWith(p,savedFree);}
static void* nativeAllocate(size_t size) {return allocateWith(size,reinterpret_cast<void* (*)(size_t)>(nativeAllocateSlot),reinterpret_cast<void (*)(void*)>(nativeFreeSlot));}
static void nativeFree(void* p) {freeWith(p,reinterpret_cast<void (*)(void*)>(nativeFreeSlot));}
static void cleanupAllocations() {for(const auto& a:allocations)a.release(a.pointer);allocations.clear();}
struct Imports {
    std::vector<std::unique_ptr<recordset_value_test::Patch>> patches;
    Imports(HMODULE module,bool originals) {
#define HOOK(name,callback,saved) {auto** slot=recordset_value_test::import(module,name);check(slot!=nullptr,"locate scroll print import " name);if(!slot)std::abort();if(originals)saved=reinterpret_cast<decltype(saved)>(*slot);patches.emplace_back(new recordset_value_test::Patch(slot,reinterpret_cast<void*>(&callback)));}
        HOOK("CreateCompatibleDC",compatible,realCompatible);HOOK("CreateDIBSection",dib,realDib);HOOK("GetObjectW",getObject,realObject);HOOK("SelectObject",select,realSelect);HOOK("BitBlt",blt,realBlt);
        HOOK("DeleteObject",deleteObject,realDeleteObject);HOOK("DeleteDC",deleteDC,realDeleteDC);HOOK("SetMapMode",map,realMap);HOOK("SetViewportOrgEx",viewport,realViewport);HOOK("OffsetViewportOrgEx",offsetViewport,realOffsetViewport);HOOK("SetWindowOrgEx",window,realWindow);HOOK("OffsetWindowOrgEx",offsetWindow,realOffsetWindow);
#undef HOOK
        if(!originals) {
            auto** a=recordset_value_test::import(module,"malloc");auto** f=recordset_value_test::import(module,"free");check(a && f,"locate native bitmap CRT allocation imports");
            if(!a || !f)std::abort();nativeAllocateSlot=*a;nativeFreeSlot=*f;
            patches.emplace_back(new recordset_value_test::Patch(a,reinterpret_cast<void*>(&nativeAllocate)));
            patches.emplace_back(new recordset_value_test::Patch(f,reinterpret_cast<void*>(&nativeFree)));
        }
    }
    ~Imports(){while(!patches.empty())patches.pop_back();}
};
struct Fixture {
    scroll_view_sizing_test::Fixture f;const API& api;
    alignas(8) BYTE dcBytes[40]{};void* table[32]{};
    alignas(8) BYTE info[64]{},dialog[320]{};PRINTDLGW pd{};
    Fixture(const API& functions):f(functions.sizing),api(functions) {
        capture=false;targetHandle=CreateCompatibleDC(nullptr);bufferHandle=CreateCompatibleDC(nullptr);attributeHandle=CreateCompatibleDC(nullptr);
        BITMAPINFO format{};format.bmiHeader.biSize=40;format.bmiHeader.biWidth=128;format.bmiHeader.biHeight=128;format.bmiHeader.biPlanes=1;format.bmiHeader.biBitCount=32;
        bitmap=CreateDIBSection(nullptr,&format,DIB_RGB_COLORS,&bitmapBits,nullptr,0);check(targetHandle && bufferHandle && attributeHandle && bitmap,"create real print DCs and a DIB for native comparisons");
        targetDC=reinterpret_cast<CDC*>(dcBytes);*reinterpret_cast<void***>(targetDC)=table;table[15]=reinterpret_cast<void*>(&setMap);table[16]=reinterpret_cast<void*>(&setOrigin);table[18]=reinterpret_cast<void*>(&viewportExtent);table[20]=reinterpret_cast<void*>(&windowExtent);
        f.f.table[100]=reinterpret_cast<void*>(&prepare);f.f.table[105]=reinterpret_cast<void*>(&draw);memset(dcBytes+32,0xa5,8);reset();capture=true;
    }
    void cleanupMaps() {CDC temporary;for(HDC h:{targetHandle,bufferHandle,attributeHandle}){api.attach(&temporary,h);api.detach(&temporary);}}
    void reset() {
        capture=false;cleanupAllocations();cleanupMaps();f.reset();active=&api;self->m_bInitialRedraw=FALSE;self->m_bCenter=FALSE;targetDC->m_hDC=targetHandle;targetDC->m_hAttribDC=attributeHandle;targetDC->m_bPrinting=FALSE;bufferDC=originDC=nullptr;originOutput=nullptr;
        printInfo=info;parameters=&pd;*reinterpret_cast<void**>(info)=dialog;*reinterpret_cast<void**>(dialog+0x130)=&pd;*reinterpret_cast<BOOL*>(info+20)=37;*reinterpret_cast<UINT*>(info+24)=1;pd.nMaxPage=0xffff;
        noBuffer=noBitmap=noAllocation=renderPixels=delegatePreparation=false;objectResult=sizeof(DIBSECTION);selectedResult=reinterpret_cast<HGDIOBJ>(UINT_PTR(77));originCalls=bltCalls=selectCalls=0;failSecondCall=false;allocatedCount=freedCount=0;capture=true;
    }
    void save(std::vector<Event>& out,INT64 result,int caught=0) {f.save(out,result,caught);out.push_back({140,*reinterpret_cast<BOOL*>(info+20),INT64(allocatedCount),INT64(freedCount),self->m_bInitialRedraw,0,0,0});}
    ~Fixture(){capture=false;failure=0;cleanupAllocations();cleanupMaps();api.clear();DeleteObject(bitmap);DeleteDC(targetHandle);DeleteDC(bufferHandle);DeleteDC(attributeHandle);check(*reinterpret_cast<UINT64*>(dcBytes+32)==0xa5a5a5a5a5a5a5a5ULL,"scroll printing preserves the native CDC object boundary");}
};
template<class F>static int invoke(F action){try{action();return 0;}catch(int n){return n;}catch(CException* e){e->Delete();return 92;}}
static std::vector<Event> preparation(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(BOOL initial:{FALSE,BOOL(69)})for(int mode:{-1,MM_TEXT,MM_LOMETRIC})for(BOOL printing:{FALSE,BOOL(69)})for(BOOL center:{FALSE,BOOL(-1)})for(int extent:{-17,0,100,INT_MIN,INT_MAX})for(bool information:{false,true}) {
        f.reset();self->m_bInitialRedraw=initial;self->m_nMapMode=mode;self->m_bCenter=center;self->m_totalDev={extent,extent};targetDC->m_bPrinting=printing;api.prepare(self,targetDC,information?f.info:nullptr);f.save(out,0);
    }
    for(UINT page:{0u,1u,2u,UINT_MAX})for(WORD maximum:{WORD(0),WORD(1),WORD(0xffff)})for(int mode:{0,40,41,42,43,44})for(int at:{0,1,12,31,100,101,102,103}) {
        f.reset();self->m_nMapMode=-1;self->m_bCenter=TRUE;*reinterpret_cast<UINT*>(f.info+24)=page;f.pd.nMaxPage=maximum;mutation=mode;failure=at;
        int caught=invoke([&]{api.prepare(self,targetDC,f.info);});f.save(out,0,caught);
    }
    f.reset();self->m_bInitialRedraw=69;api.prepare(self,nullptr,reinterpret_cast<void*>(UINT_PTR(1)));f.save(out,0);return out;
}
static std::vector<Event> origins(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(int method=0;method<4;++method)for(int handles:{0,1,2,3,4})for(int alias:{0,1,2})for(int mode:{0,55})for(int at:{0,128,129,130,131}) {
        f.reset();originDC=targetDC;targetDC->m_hDC=handles&1?targetHandle:nullptr;targetDC->m_hAttribDC=handles==4?targetDC->m_hDC:handles&2?attributeHandle:nullptr;
        CPoint result(101,103);CPoint* target=alias==1?reinterpret_cast<CPoint*>(&targetDC->m_hDC):alias==2?reinterpret_cast<CPoint*>(&targetDC->m_hAttribDC):&result;originOutput=target;
        mutation=mode;failure=at;CPoint* returned=nullptr;int caught=invoke([&]{returned=api.origins[method](targetDC,target,INT_MIN,INT_MAX);});
        CPoint observed=normalizedPoint(target);f.save(out,returned==target,caught);out.push_back({141,observed.x,observed.y,dcID(targetDC->m_hDC),dcID(targetDC->m_hAttribDC),0,0,0});
    }
    return out;
}
static std::vector<Event> printing(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(int absent:{0,1,2})for(UINT flags:{0u,UINT(PRF_CLIENT),UINT(PRF_ERASEBKGND),UINT_MAX})for(int allocation:{0,1,2,3})for(int selectResult:{0,1,2})for(int mode:{0,50,51,52,54}) {
        f.reset();noAllocation=allocation==1;noBitmap=allocation==2;noBuffer=allocation==3;mutation=mode;selectedResult=selectResult==0?nullptr:selectResult==1?reinterpret_cast<HGDIOBJ>(UINT_PTR(77)):HGDI_ERROR;
        if(absent==2)targetDC->m_hDC=nullptr;LRESULT result=api.print(self,absent==1?nullptr:targetDC,flags);f.save(out,result);
    }
    for(int object:{0,int(sizeof(BITMAP)),int(sizeof(DIBSECTION))})for(bool realPreparation:{false,true})for(int at:{0,1,12,31,110,111,120,123,124,125,127,128,130}) {
        out.push_back({150,object,realPreparation,at,0,0,0,0});
        f.reset();objectResult=object;delegatePreparation=realPreparation;failure=at;LRESULT result=17;int caught=invoke([&]{result=api.print(self,targetDC,UINT_MAX);});f.save(out,result,caught);
    }
    for(int at:{123,124}) {
        f.reset();failSecondCall=true;failure=at;LRESULT result=17;int caught=invoke([&]{result=api.print(self,targetDC,0);});f.save(out,result,caught);
    }
    return out;
}
static void expectations(const API& api) {
    Fixture f(api);self->m_nMapMode=-1;api.prepare(self,targetDC,f.info);
    auto calls=events;check(calls.size()>=4 && calls[0][0]==100 && calls[0][1]==MM_ANISOTROPIC && calls[1][0]==102 && calls[2][0]==103 && calls.back()[0]==101,"scale-to-fit preparation dispatches native mapping, window extents, viewport extents and origin virtuals");
    f.reset();targetDC->m_bPrinting=69;api.prepare(self,targetDC,f.info);check(events.size()==2 && events.back()[1]==0 && events.back()[2]==0,"printing preparation uses zero origin and skips window scroll positions");
    f.reset();check(api.print(self,targetDC,0)==0 && allocatedCount==1 && freedCount==1,"print-client renders with zero flags and returns the native zero result");
    check(std::count_if(events.begin(),events.end(),[](const Event& e){return e[0]==124;})==2 && std::any_of(events.begin(),events.end(),[](const Event& e){return e[0]==121 && e[5]==32;}),"print-client allocates a 32-bit DIB and copies the target before and after drawing");
    f.reset();failure=111;int caught=invoke([&]{api.print(self,targetDC,0);});check(caught==91 && events[events.size()-2][0]==125 && events.back()[0]==126,"draw exceptions destroy the offscreen bitmap before deleting its DC");
}
static void termination(bool native,int failureID) {
    wchar_t executable[2048];GetModuleFileNameW(nullptr,executable,2048);std::wstring command=L"\""+std::wstring(executable)+L"\"";
    std::wstring mode=(native?L"1 ":L"0 ")+std::to_wstring(failureID);SetEnvironmentVariableW(L"OPENMFC_SCROLL_PRINT_CHILD",mode.c_str());
    STARTUPINFOW startup{};startup.cb=sizeof(startup);PROCESS_INFORMATION process{};
    BOOL created=CreateProcessW(executable,&command[0],nullptr,nullptr,FALSE,0,nullptr,nullptr,&startup,&process);SetEnvironmentVariableW(L"OPENMFC_SCROLL_PRINT_CHILD",nullptr);
    check(created!=FALSE,"start print resource termination child");if(!created)return;
    DWORD waited=WaitForSingleObject(process.hProcess,15000);if(waited!=WAIT_OBJECT_0)TerminateProcess(process.hProcess,99);DWORD code=0;GetExitCodeProcess(process.hProcess,&code);CloseHandle(process.hThread);CloseHandle(process.hProcess);
    check(waited==WAIT_OBJECT_0 && code==3,"print bitmap creation and DC destruction preserve the native no-throw boundaries");
}
static void terminateInCreation(const API& api,int failureID) {
    SetErrorMode(SEM_NOGPFAULTERRORBOX|SEM_FAILCRITICALERRORS);std::set_terminate([]{ExitProcess(3);});
    Fixture f(api);failure=failureID;
    try{api.print(self,targetDC,0);}catch(...){ExitProcess(97);}ExitProcess(98);
}
static void realRendering(const API& api) {
    Fixture f(api);capture=false;splitter_command_test::capture=false;self->m_hWnd=splitter_command_test::handles[3];
    BITMAPINFO format{};format.bmiHeader.biSize=40;format.bmiHeader.biWidth=128;format.bmiHeader.biHeight=128;format.bmiHeader.biPlanes=1;format.bmiHeader.biBitCount=32;void* pixels=nullptr;
    HBITMAP targetBitmap=CreateDIBSection(nullptr,&format,DIB_RGB_COLORS,&pixels,nullptr,0);auto previous=SelectObject(targetHandle,targetBitmap);SetPixel(targetHandle,5,7,RGB(31,97,173));renderPixels=true;
    targetDC->m_hAttribDC=targetHandle;LRESULT result=api.print(self,targetDC,PRF_CLIENT);
    check(result==0 && GetPixel(targetHandle,5,7)==RGB(31,97,173) && GetPixel(targetHandle,10,12)==RGB(211,37,79),"real offscreen print-client rendering preserves existing pixels and copies drawing back to the caller");
    renderPixels=false;SelectObject(targetHandle,previous);DeleteObject(targetBitmap);
}
}
static void scrollViewPrinting() {
    using namespace scroll_view_print_test;
    scroll_view_input_test::API input{};input.cacheInitialized=&openmfc::detail::scrollViewWheelSettingsInitialized;input.cacheLines=&openmfc::detail::scrollViewWheelLines;input.state=&AfxGetThreadState;
    scroll_view_sizing_test::API sizing{input};sizing.data=&openmfc::detail::GetAuxData();sizing.updateBars=&impl__UpdateBars_CScrollView__IEAAXXZ;
    scroll_view_print_test::API local{sizing,&impl__OnPrepareDC_CScrollView__UEAAXPEAVCDC__PEAUCPrintInfo___Z,&impl__OnPrintClient_CScrollView__QEAA_JPEAVCDC__I_Z,
        {&impl__SetViewportOrg_CDC__UEAA_AVCPoint__HH_Z,&impl__OffsetViewportOrg_CDC__UEAA_AVCPoint__HH_Z,&impl__SetWindowOrg_CDC__QEAA_AVCPoint__HH_Z,&impl__OffsetWindowOrg_CDC__QEAA_AVCPoint__HH_Z},&impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z,&impl__DeleteTempMap_CDC__SAXXZ,&impl__Attach_CDC__QEAAHPEAUHDC_____Z,&impl__Detach_CDC__QEAAPEAUHDC____XZ};
    savedAllocate=openmfc::detail::printBitmapAllocate;savedFree=openmfc::detail::printBitmapFree;openmfc::detail::printBitmapAllocate=&allocate;openmfc::detail::printBitmapFree=&freeMemory;
    scroll_view_input_test::Imports inputImports(GetModuleHandleW(nullptr),true);scroll_view_sizing_test::Imports sizingImports(GetModuleHandleW(nullptr),true);scroll_view_print_test::Imports imports(GetModuleHandleW(nullptr),true);
    wchar_t child[32]{};GetEnvironmentVariableW(L"OPENMFC_SCROLL_PRINT_CHILD",child,32);
    if(child[0]==L'0')terminateInCreation(local,_wtoi(child+2));
    std::vector<Event> prepared,origin,printed;
    if(!child[0]) {expectations(local);prepared=preparation(local);origin=origins(local);
        {CDC dc;auto** table=*reinterpret_cast<void***>(&dc);auto virtualAPI=local;virtualAPI.origins[0]=reinterpret_cast<Origin>(table[16]);virtualAPI.origins[1]=reinterpret_cast<Origin>(table[17]);
            scroll_view_input_test::compare(origins(virtualAPI),origin,"DC origin vtable methods preserve the exported aggregate-return and aliasing behavior");}
        printed=printing(local);realRendering(local);for(int at:{121,122,126,132})scroll_view_print_test::termination(false,at);}
    wchar_t path[1024];if(GetEnvironmentVariableW(L"OPENMFC_NATIVE_SCROLL_PRINT_DLL",path,1024)) {
        HMODULE native=LoadLibraryW(path);check(native!=nullptr,"load native MFC for scroll printing comparisons");
        if(native) {
            auto* b=reinterpret_cast<char*>(native);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(b);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"scroll print oracle uses the verified MFC image build");
            if(pinned) {
                scroll_view_input_test::Imports nativeInput(native,false);scroll_view_sizing_test::Imports nativeSizing(native,false);scroll_view_print_test::Imports nativeImports(native,false);scroll_view_print_test::API api=local;
#define RVA(field,offset) api.field=reinterpret_cast<decltype(api.field)>(b+offset)
                RVA(prepare,0x287f20);RVA(print,0x288c30);RVA(lookup,0x2a2450);RVA(clear,0x2a6250);RVA(attach,0x2a2480);RVA(detach,0x2a24d0);
#undef RVA
                api.origins[0]=reinterpret_cast<Origin>(b+0x2a2ab0);api.origins[1]=reinterpret_cast<Origin>(b+0x2a2b10);api.origins[2]=reinterpret_cast<Origin>(b+0x2a2c60);api.origins[3]=reinterpret_cast<Origin>(b+0x2a2cc0);
                api.sizing.data=reinterpret_cast<AuxData*>(b+0x3c32c0);api.sizing.input.graph.construct=reinterpret_cast<decltype(input.graph.construct)>(b+0x28a700);api.sizing.input.graph.attach=reinterpret_cast<decltype(input.graph.attach)>(b+0x28adf0);api.sizing.input.graph.detach=reinterpret_cast<decltype(input.graph.detach)>(b+0x28ae50);api.sizing.input.graph.destroy=reinterpret_cast<decltype(input.graph.destroy)>(b+0x28b740);api.sizing.input.graph.type=reinterpret_cast<CRuntimeClass*>(b+0x332860);
                scroll_view_sizing_test::nativeLookupReturn=b+0x2a9da9;
                if(child[0]==L'1')terminateInCreation(api,_wtoi(child+2));
                expectations(api);
                scroll_view_input_test::compare(preparation(api),prepared,"scroll preparation mapping virtuals, centering, printing flags and continuation state match native MFC");
                scroll_view_input_test::compare(origins(api),origin,"DC origin aggregate outputs, aliases, attribute handles and interrupted operations match native MFC");
                auto actualPrint=printing(api);
                if(actualPrint!=printed)for(size_t i=0;i<std::min(actualPrint.size(),printed.size());++i)if(actualPrint[i]!=printed[i]) {
                    for(size_t j=i>12?i-12:0;j<std::min(i+8,printed.size());++j){printf("local print %zu:",j);for(auto v:printed[j])printf(" %lld",v);puts("");}
                    for(size_t j=i>12?i-12:0;j<std::min(i+8,actualPrint.size());++j){printf("native print %zu:",j);for(auto v:actualPrint[j])printf(" %lld",v);puts("");}break;
                }
                scroll_view_input_test::compare(actualPrint,printed,"offscreen print-client copies, allocation failures, callbacks and resource cleanup match native MFC");
                realRendering(api);for(int at:{121,122,126,132})scroll_view_print_test::termination(true,at);scroll_view_sizing_test::nativeLookupReturn=nullptr;
            }
            FreeLibrary(native);
        }
    }
    if(child[0])ExitProcess(96);
    openmfc::detail::printBitmapAllocate=savedAllocate;openmfc::detail::printBitmapFree=savedFree;
}
