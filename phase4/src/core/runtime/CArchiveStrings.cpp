// Native CString length records and line-oriented archive I/O.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <climits>

ULONGLONG AFXAPI AfxReadStringLength(CArchive& archive,int& charSize) {
    charSize=1;
    BYTE first;
    archive>>first;
    if(first!=0xff)return first;
    WORD medium;
    archive>>medium;
    if(medium==0xfffe) {
        charSize=2;
        archive>>first;
        if(first!=0xff)return first;
        archive>>medium;
    }
    if(medium!=0xffff)return medium;
    DWORD large;
    archive>>large;
    if(large!=0xffffffff)return large;
    ULONGLONG length;
    archive>>length;
    return length;
}

void AFXAPI AfxWriteStringLength(CArchive& archive,ULONGLONG length,BOOL unicode) {
    if(unicode)archive<<BYTE(0xff)<<WORD(0xfffe);
    if(length<0xff) {
        archive<<BYTE(length);
        return;
    }
    archive<<BYTE(0xff);
    if(length<0xfffe) {
        archive<<WORD(length);
        return;
    }
    archive<<WORD(0xffff);
    if(length<0xffffffff)archive<<DWORD(length);
    else archive<<DWORD(0xffffffff)<<length;
}

void CArchive::WriteString(const wchar_t* text) {
    Write(text,text ? UINT(wcslen(text)*sizeof(wchar_t)) : 0);
}

wchar_t* CArchive::ReadString(wchar_t* text,UINT limit) {
    if(!text)return nullptr;
    const bool keepTerminator=static_cast<int>(limit)<0;
    const int count=static_cast<int>(keepTerminator ? 0u-limit : limit);
    int used=0;
    try {
        while(used<count) {
            WORD character;
            *this>>character;
            if(character==L'\r') {
                *this>>character;
                if(keepTerminator)text[used++]=character;
                break;
            }
            if(character==L'\n') {
                if(keepTerminator)text[used++]=character;
                break;
            }
            text[used++]=character;
        }
    } catch(CArchiveException* error) {
        if(error->m_cause!=CArchiveException::endOfFile)throw;
        error->Delete();
        if(!used)return nullptr;
    }
    text[used]=0;
    return text;
}

BOOL CArchive::ReadString(CString& text) {
    constexpr int chunkSize=128;
    text.SetString(L"",0);
    wchar_t* line;
    size_t count;
    do {
        const int length=text.GetLength();
        if(length>INT_MAX-chunkSize)AfxThrowOleException(E_INVALIDARG);
        wchar_t* chunk=text.GetBuffer(length+chunkSize)+length;
        line=ReadString(chunk,UINT(-chunkSize));
        text.ReleaseBuffer();
        count=line?wcslen(line):0;
    } while(count>=chunkSize && line[count-1]!=L'\n');

    const wchar_t* result=text.GetBuffer();
    const int length=text.GetLength();
    if(length && result[length-1]==L'\n')text.Truncate(length-1);
    return line!=nullptr;
}

// Symbol: ?AfxReadStringLength@@YA_KAEAVCArchive@@AEAH@Z
extern "C" ULONGLONG impl__AfxReadStringLength__YA_KAEAVCArchive__AEAH_Z(CArchive* archive,int* charSize) {
    return AfxReadStringLength(*archive,*charSize);
}
// Symbol: ?AfxWriteStringLength@@YAXAEAVCArchive@@_KH@Z
extern "C" void impl__AfxWriteStringLength__YAXAEAVCArchive___KH_Z(CArchive* archive,ULONGLONG length,BOOL unicode) {
    AfxWriteStringLength(*archive,length,unicode);
}
// Symbol: ?WriteString@CArchive@@QEAAXPEB_W@Z
extern "C" void impl__WriteString_CArchive__QEAAXPEB_W_Z(CArchive* archive,const wchar_t* text) {
    archive->WriteString(text);
}
// Symbol: ?ReadString@CArchive@@QEAAPEA_WPEA_WI@Z
extern "C" wchar_t* impl__ReadString_CArchive__QEAAPEA_WPEA_WI_Z(CArchive* archive,wchar_t* text,UINT limit) {
    return archive->ReadString(text,limit);
}
// Symbol: ?ReadString@CArchive@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" BOOL impl__ReadString_CArchive__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CArchive* archive,CString* text) {
    return archive->ReadString(*text);
}
