// Native two-pass resource formatting, including %1..%9 and %A..%Z inserts.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxdisp.h"
#include <cerrno>
extern "C" void* impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(UINT);
namespace {
int Index(wchar_t ch) {
    if(ch>=L'1' && ch<=L'9')return ch-L'1';
    if(ch>=L'A' && ch<=L'Z')return ch-L'A'+9;
    return -1;
}
}
void AFXAPI AfxFormatStrings(CString& text,const wchar_t* format,const wchar_t* const* arguments,int count) {
    if(!format || !arguments)AfxThrowInvalidArgException();
    uint32_t measured=0;
    for(auto* cursor=format;*cursor;) {
        int index=*cursor==L'%'?Index(cursor[1]):-1;
        if(index>=0) {
            cursor+=2;
            if(index>=count)++measured;
            else if(arguments[index])measured+=uint32_t(wcslen(arguments[index]));
        } else {++cursor;++measured;}
    }
    int remaining=int32_t(measured);
    if(remaining<0)AfxThrowOleException(E_INVALIDARG);
    wchar_t* output=text.GetBuffer(remaining);
    for(auto* cursor=format;*cursor;) {
        int index=*cursor==L'%'?Index(cursor[1]):-1;
        if(index>=0) {
            if(index>=count) {*output++=L'?';--remaining;}
            else if(arguments[index]) {
                int length=int(wcslen(arguments[index]));
                int result=wcscpy_s(output,size_t(int32_t(uint32_t(remaining)+1)),arguments[index]);
                if(result && result!=STRUNCATE) {
                    if(result==ENOMEM)AfxThrowMemoryException();
                    else AfxThrowInvalidArgException();
                }
                remaining=int32_t(uint32_t(remaining)-uint32_t(length));
                output+=length;
            }
            cursor+=2;
        } else {*output++=*cursor++;--remaining;}
    }
    auto bytes=intptr_t(output)-intptr_t(text.GetString());
    text.ReleaseBuffer(int(bytes>>1));
}
void AFXAPI AfxFormatStrings(CString& text,UINT id,const wchar_t* const* arguments,int count) {
    CString format;
    if(format.LoadString(id))AfxFormatStrings(text,format.GetString(),arguments,count);
}
// Symbol: ?AfxFormatStrings@@YAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@IPEBQEB_WH@Z
extern "C" void impl__AfxFormatStrings__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEBQEB_WH_Z(CString* text,UINT id,const wchar_t* const* args,int count) {AfxFormatStrings(*text,id,args,count);}
// Symbol: ?AfxFormatStrings@@YAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_WPEBQEB_WH@Z
extern "C" void impl__AfxFormatStrings__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_WPEBQEB_WH_Z(CString* text,const wchar_t* format,const wchar_t* const* args,int count) {AfxFormatStrings(*text,format,args,count);}

void AFXAPI AfxFormatString1(CString& text,UINT id,const wchar_t* first) {
    AfxFormatStrings(text,id,&first,1);
}
void AFXAPI AfxFormatString2(CString& text,UINT id,const wchar_t* first,const wchar_t* second) {
    const wchar_t* arguments[]={first,second};
    AfxFormatStrings(text,id,arguments,2);
}
// Symbol: ?AfxFormatString1@@YAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@IPEB_W@Z
extern "C" void impl__AfxFormatString1__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEB_W_Z(CString* text,UINT id,const wchar_t* first) {AfxFormatString1(*text,id,first);}
// Symbol: ?AfxFormatString2@@YAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@IPEB_W1@Z
extern "C" void impl__AfxFormatString2__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEB_W1_Z(CString* text,UINT id,const wchar_t* first,const wchar_t* second) {AfxFormatString2(*text,id,first,second);}
