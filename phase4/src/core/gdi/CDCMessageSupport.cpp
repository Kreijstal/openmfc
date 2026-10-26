// Native CDC storage and virtual callbacks used by window message handlers.
#define OPENMFC_APPCORE_IMPL
#include "detail/GdicoreSupport.h"
#include <new>
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
namespace {
template<class Result,class Fn> Result Both(CDC* dc,Result initial,Fn call) {
    Result result=initial;
    if(dc->m_hDC!=dc->m_hAttribDC)result=call(dc->m_hDC);
    if(dc->m_hAttribDC)result=call(dc->m_hAttribDC);
    return result;
}
template<class Fn> CSize Extent(CDC* dc,Fn call) {
    SIZE previous{};
    if(dc->m_hDC!=dc->m_hAttribDC)call(dc->m_hDC,&previous);
    if(dc->m_hAttribDC)call(dc->m_hAttribDC,&previous);
    return CSize(previous.cx,previous.cy);
}
}
CDC::CDC() : m_hDC(nullptr),m_hAttribDC(nullptr),m_bPrinting(FALSE) {}
CDC::~CDC() {
    if(m_hDC)::DeleteDC(Detach());
}
void CDC::SetAttribDC(HDC dc) { m_hAttribDC=dc; }
void CDC::SetOutputDC(HDC dc) { m_hDC=dc; }
void CDC::ReleaseAttribDC() { m_hAttribDC=nullptr; }
void CDC::ReleaseOutputDC() { m_hDC=nullptr; }
int CDC::SaveDC() {
    int saved=m_hAttribDC ? ::SaveDC(m_hAttribDC) : 0;
    if(m_hDC!=m_hAttribDC && ::SaveDC(m_hDC))saved=-1;
    return saved;
}
BOOL CDC::RestoreDC(int saved) {
    BOOL result=m_hDC!=m_hAttribDC ? ::RestoreDC(m_hDC,saved) : TRUE;
    if(m_hAttribDC)result=result && ::RestoreDC(m_hAttribDC,saved);
    return result;
}
CGdiObject* CDC::SelectStockObject(int index) {
    HGDIOBJ old=Both<HGDIOBJ>(this,nullptr,[&](HDC dc) { return ::SelectObject(dc,::GetStockObject(index)); });
    return GetTempGdiObject(old);
}
CFont* CDC::SelectObject(CFont* font) {
    HGDIOBJ old=Both<HGDIOBJ>(this,nullptr,[&](HDC dc) { return ::SelectObject(dc,font ? font->GetSafeHandle() : nullptr); });
    return reinterpret_cast<CFont*>(GetTempGdiObject(old));
}
COLORREF CDC::SetBkColor(COLORREF color) { return Both<COLORREF>(this,CLR_INVALID,[&](HDC dc) { return ::SetBkColor(dc,color); }); }
COLORREF CDC::SetTextColor(COLORREF color) { return Both<COLORREF>(this,CLR_INVALID,[&](HDC dc) { return ::SetTextColor(dc,color); }); }
int CDC::SetMapMode(int mode) { return Both<int>(this,0,[&](HDC dc) { return ::SetMapMode(dc,mode); }); }
CSize CDC::SetViewportExt(int x,int y) { return Extent(this,[&](HDC dc,SIZE* previous) { ::SetViewportExtEx(dc,x,y,previous); }); }
CSize CDC::ScaleViewportExt(int xn,int xd,int yn,int yd) { return Extent(this,[&](HDC dc,SIZE* previous) { ::ScaleViewportExtEx(dc,xn,xd,yn,yd,previous); }); }
CSize CDC::SetWindowExt(int x,int y) { return Extent(this,[&](HDC dc,SIZE* previous) { ::SetWindowExtEx(dc,x,y,previous); }); }
CSize CDC::ScaleWindowExt(int xn,int xd,int yn,int yd) { return Extent(this,[&](HDC dc,SIZE* previous) { ::ScaleWindowExtEx(dc,xn,xd,yn,yd,previous); }); }
int CDC::GetClipBox(LPRECT rect) const { return ::GetClipBox(m_hDC,rect); }
BOOL CDC::PtVisible(int x,int y) const { return ::PtVisible(m_hDC,x,y); }
BOOL CDC::RectVisible(LPCRECT rect) const { return ::RectVisible(m_hDC,rect); }
BOOL CDC::TextOut(int x,int y,const wchar_t* text,int count) { return ::TextOutW(m_hDC,x,y,text,count); }
BOOL CDC::ExtTextOut(int x,int y,UINT options,LPCRECT rect,const wchar_t* text,UINT count,const int* widths) { return ::ExtTextOutW(m_hDC,x,y,options,rect,text,count,widths); }
CSize CDC::TabbedTextOut(int x,int y,const wchar_t* text,int count,int tabs,int* stops,int origin) {
    DWORD size=::TabbedTextOutW(m_hDC,x,y,text,count,tabs,stops,origin);return CSize(LOWORD(size),HIWORD(size));
}
int CDC::DrawText(const wchar_t* text,int count,LPRECT rect,UINT format) { return ::DrawTextW(m_hDC,text,count,rect,format); }
int CDC::DrawTextEx(wchar_t* text,int count,LPRECT rect,UINT format,LPDRAWTEXTPARAMS params) { return ::DrawTextExW(m_hDC,text,count,rect,format,params); }
BOOL CDC::GrayString(CBrush* brush,GRAYSTRINGPROC callback,LPARAM data,int count,int x,int y,int width,int height) { return ::GrayStringW(m_hDC,brush ? reinterpret_cast<HBRUSH>(brush->GetSafeHandle()) : nullptr,callback,data,count,x,y,width,height); }
int CDC::Escape(int escape,int count,const char* input,void* output) { return ::Escape(m_hDC,escape,count,input,output); }

// Symbol: ??0CDC@@QEAA@XZ
extern "C" CDC* MS_ABI impl___0CDC__QEAA_XZ(CDC* self) { return new(self) CDC; }
// Symbol: ??1CDC@@UEAA@XZ
extern "C" void MS_ABI impl___1CDC__UEAA_XZ(CDC* self) { self->CDC::~CDC(); }

// Symbol: ?SetAttribDC@CDC@@UEAAXPEAUHDC__@@@Z
extern "C" void MS_ABI impl__SetAttribDC_CDC__UEAAXPEAUHDC_____Z(CDC* self,HDC dc) { self->CDC::SetAttribDC(dc); }
// Symbol: ?SetOutputDC@CDC@@UEAAXPEAUHDC__@@@Z
extern "C" void MS_ABI impl__SetOutputDC_CDC__UEAAXPEAUHDC_____Z(CDC* self,HDC dc) { self->CDC::SetOutputDC(dc); }
// Symbol: ?ReleaseAttribDC@CDC@@UEAAXXZ
extern "C" void MS_ABI impl__ReleaseAttribDC_CDC__UEAAXXZ(CDC* self) { self->CDC::ReleaseAttribDC(); }
// Symbol: ?ReleaseOutputDC@CDC@@UEAAXXZ
extern "C" void MS_ABI impl__ReleaseOutputDC_CDC__UEAAXXZ(CDC* self) { self->CDC::ReleaseOutputDC(); }
// Symbol: ?SaveDC@CDC@@UEAAHXZ
extern "C" int MS_ABI impl__SaveDC_CDC__UEAAHXZ(CDC* self) { return self->CDC::SaveDC(); }
// Symbol: ?RestoreDC@CDC@@UEAAHH@Z
extern "C" BOOL MS_ABI impl__RestoreDC_CDC__UEAAHH_Z(CDC* self,int saved) { return self->CDC::RestoreDC(saved); }
// Symbol: ?SelectStockObject@CDC@@UEAAPEAVCGdiObject@@H@Z
extern "C" CGdiObject* MS_ABI impl__SelectStockObject_CDC__UEAAPEAVCGdiObject__H_Z(CDC* self,int index) { return self->CDC::SelectStockObject(index); }
// Symbol: ?SelectObject@CDC@@UEAAPEAVCFont@@PEAV2@@Z
extern "C" CFont* MS_ABI impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(CDC* self,CFont* font) { return self->CDC::SelectObject(font); }
// Symbol: ?SetBkColor@CDC@@UEAAKK@Z
extern "C" COLORREF MS_ABI impl__SetBkColor_CDC__UEAAKK_Z(CDC* self,COLORREF color) { return self->CDC::SetBkColor(color); }
// Symbol: ?SetTextColor@CDC@@UEAAKK@Z
extern "C" COLORREF MS_ABI impl__SetTextColor_CDC__UEAAKK_Z(CDC* self,COLORREF color) { return self->CDC::SetTextColor(color); }
// Symbol: ?SetMapMode@CDC@@UEAAHH@Z
extern "C" int MS_ABI impl__SetMapMode_CDC__UEAAHH_Z(CDC* self,int mode) { return self->CDC::SetMapMode(mode); }
// Symbol: ?SetViewportExt@CDC@@UEAA?AVCSize@@HH@Z
extern "C" CSize* MS_ABI impl__SetViewportExt_CDC__UEAA_AVCSize__HH_Z(CDC* self,CSize* output,int x,int y) { *output=self->CDC::SetViewportExt(x,y);return output; }
// Symbol: ?ScaleViewportExt@CDC@@UEAA?AVCSize@@HHHH@Z
extern "C" CSize* MS_ABI impl__ScaleViewportExt_CDC__UEAA_AVCSize__HHHH_Z(CDC* self,CSize* output,int xn,int xd,int yn,int yd) { *output=self->CDC::ScaleViewportExt(xn,xd,yn,yd);return output; }
// Symbol: ?SetWindowExt@CDC@@UEAA?AVCSize@@HH@Z
extern "C" CSize* MS_ABI impl__SetWindowExt_CDC__UEAA_AVCSize__HH_Z(CDC* self,CSize* output,int x,int y) { *output=self->CDC::SetWindowExt(x,y);return output; }
// Symbol: ?ScaleWindowExt@CDC@@UEAA?AVCSize@@HHHH@Z
extern "C" CSize* MS_ABI impl__ScaleWindowExt_CDC__UEAA_AVCSize__HHHH_Z(CDC* self,CSize* output,int xn,int xd,int yn,int yd) { *output=self->CDC::ScaleWindowExt(xn,xd,yn,yd);return output; }
// Symbol: ?GetClipBox@CDC@@UEBAHPEAUtagRECT@@@Z
extern "C" int MS_ABI impl__GetClipBox_CDC__UEBAHPEAUtagRECT___Z(const CDC* self,LPRECT rect) { return self->CDC::GetClipBox(rect); }
