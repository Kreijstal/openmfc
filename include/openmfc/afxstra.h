#pragma once
#include "afxstr.h"
#include <climits>

struct tagVARIANT;
namespace ATL {
template<> class CStringT<char,::StrTraitMFC_DLL<char>> : public CSimpleStringT<char,true> {
public:
    CStringT() noexcept:CSimpleStringA(OpenMFC_GetStringMgr()) {}
    explicit CStringT(IAtlStringMgr* manager) noexcept:CSimpleStringA(manager) {}
    CStringT(const CStringA& source):CSimpleStringA(source) {}
    OPENMFC_STRING_API CStringT(const char* source);
    OPENMFC_STRING_API CStringT(const char* source,IAtlStringMgr* manager);
    CStringT(const char* source,int length):CSimpleStringA(source,length,OpenMFC_GetStringMgr()) {}
    CStringT(const char* source,int length,IAtlStringMgr* manager):CSimpleStringA(source,length,manager) {}
    CStringT(unsigned char* source):CStringT(reinterpret_cast<const char*>(source)) {}
    CStringT(const unsigned char* source):CSimpleStringA(OpenMFC_GetStringMgr()) {SetString(reinterpret_cast<const char*>(source));}
    CStringT(const unsigned char* source,IAtlStringMgr* manager):CSimpleStringA(manager) {SetString(reinterpret_cast<const char*>(source));}
    OPENMFC_STRING_API CStringT(const wchar_t* source);
    OPENMFC_STRING_API CStringT(const wchar_t* source,IAtlStringMgr* manager);
    OPENMFC_STRING_API CStringT(const wchar_t* source,int length);
    OPENMFC_STRING_API CStringT(const wchar_t* source,int length,IAtlStringMgr* manager);
    OPENMFC_STRING_API CStringT(const tagVARIANT&);
    OPENMFC_STRING_API CStringT(const tagVARIANT&,IAtlStringMgr*);
    OPENMFC_STRING_API CStringA& operator=(const tagVARIANT&);
    using CSimpleStringA::operator+=;
    OPENMFC_STRING_API CStringA& operator+=(const wchar_t*);
    OPENMFC_STRING_API CStringA& operator+=(const tagVARIANT&);
    OPENMFC_STRING_API CStringT(char value,int count);
    OPENMFC_STRING_API CStringT(wchar_t value,int count);
    CStringA& operator=(const CStringA& source) {CSimpleStringA::operator=(source);return *this;}
    CStringA& operator=(const char* source) {SetString(source);return *this;}
    CStringA& operator=(const unsigned char* source) {SetString(reinterpret_cast<const char*>(source));return *this;}
    OPENMFC_STRING_API CStringA& operator=(const wchar_t* source);
    CStringA& operator=(char value) {char text[]={value,0};SetString(text);return *this;}
    OPENMFC_STRING_API CStringA& operator=(wchar_t value);
    OPENMFC_STRING_API int LoadStringW(UINT id);
    int LoadString(UINT id) {return LoadStringW(id);}
    OPENMFC_STRING_API int LoadStringW(HINSTANCE module,UINT id);
    int LoadString(HINSTANCE module,UINT id) {return LoadStringW(module,id);}
    OPENMFC_STRING_API int LoadStringW(HINSTANCE module,UINT id,unsigned short language);
    int LoadString(HINSTANCE module,UINT id,unsigned short language) {return LoadStringW(module,id,language);}
private:
    void Convert(const wchar_t*,int);
};
} // namespace ATL
static_assert(sizeof(CStringA)==8,"native ANSI CString");
