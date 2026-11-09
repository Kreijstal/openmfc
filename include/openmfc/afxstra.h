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
    CStringA& operator+=(const CSimpleStringA& source) {Append(source);return *this;}
    CStringA& operator+=(const char* source) {Append(source);return *this;}
    CStringA& operator+=(char value) {AppendChar(value);return *this;}
    CStringA& operator+=(unsigned char value) {AppendChar(static_cast<char>(value));return *this;}
    CStringA& operator+=(wchar_t value) {AppendChar(static_cast<char>(value));return *this;}
    IAtlStringMgr* GetManager() const noexcept {
        auto* manager=CSimpleStringA::GetManager();return manager?manager:OpenMFC_GetStringMgr()->Clone();
    }
    OPENMFC_STRING_API int Compare(const char* text) const;
    OPENMFC_STRING_API int CompareNoCase(const char* text) const;
    OPENMFC_STRING_API int Collate(const char* text) const;
    OPENMFC_STRING_API int CollateNoCase(const char* text) const;
    OPENMFC_STRING_API int Find(char value,int start=0) const;
    OPENMFC_STRING_API int Find(const char* text,int start=0) const;
    OPENMFC_STRING_API int FindOneOf(const char* set) const;
    OPENMFC_STRING_API int ReverseFind(char value) const;
    OPENMFC_STRING_API CStringA& MakeUpper();
    OPENMFC_STRING_API CStringA& MakeLower();
    OPENMFC_STRING_API CStringA& MakeReverse();
    OPENMFC_STRING_API CStringA Left(int count) const;
    OPENMFC_STRING_API CStringA Right(int count) const;
    OPENMFC_STRING_API CStringA Mid(int first) const;
    OPENMFC_STRING_API CStringA Mid(int first,int count) const;
    OPENMFC_STRING_API CStringA SpanIncluding(const char* set) const;
    OPENMFC_STRING_API CStringA SpanExcluding(const char* set) const;
    OPENMFC_STRING_API CStringA Tokenize(const char* delimiters,int& start) const;
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
    OPENMFC_STRING_API CStringA& Trim();
    OPENMFC_STRING_API CStringA& Trim(char value);
    OPENMFC_STRING_API CStringA& Trim(const char* set);
    OPENMFC_STRING_API CStringA& TrimLeft();
    OPENMFC_STRING_API CStringA& TrimLeft(char value);
    OPENMFC_STRING_API CStringA& TrimLeft(const char* set);
    OPENMFC_STRING_API CStringA& TrimRight();
    OPENMFC_STRING_API CStringA& TrimRight(char value);
    OPENMFC_STRING_API CStringA& TrimRight(const char* set);
    OPENMFC_STRING_API int Delete(int index,int count=1);
    OPENMFC_STRING_API int Insert(int index,char value);
    OPENMFC_STRING_API int Insert(int index,const char* source);
    OPENMFC_STRING_API int Remove(char value);
    OPENMFC_STRING_API int Replace(char before,char after);
    OPENMFC_STRING_API int Replace(const char* before,const char* after);
    OPENMFC_STRING_API void FormatV(const char* format,va_list args);
    OPENMFC_STRING_API void Format(const char* format,...);
    OPENMFC_STRING_API void Format(UINT id,...);
    OPENMFC_STRING_API void AppendFormatV(const char* format,va_list args);
    OPENMFC_STRING_API void AppendFormat(const char* format,...);
    OPENMFC_STRING_API void AppendFormat(UINT id,...);
    OPENMFC_STRING_API void FormatMessageV(const char* format,va_list* args);
    OPENMFC_STRING_API void FormatMessageW(const char* format,...);
    OPENMFC_STRING_API void FormatMessageW(UINT id,...);
#pragma push_macro("FormatMessage")
#undef FormatMessage
    OPENMFC_STRING_API void FormatMessage(const char* format,...);
    OPENMFC_STRING_API void FormatMessage(UINT id,...);
#pragma pop_macro("FormatMessage")

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
