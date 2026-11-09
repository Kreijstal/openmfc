#define OPENMFC_APPCORE_IMPL
#include "detail/OleDataSourceSupport.h"
#include <cstring>
#include <cwchar>
namespace {
HGLOBAL CopyGlobal(HGLOBAL dest,HGLOBAL source) {
    SIZE_T size=GlobalSize(source);
    if (!dest) {dest=GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,size);if (!dest) return nullptr;}
    else if (size>GlobalSize(dest)) return nullptr;
    const void* input=GlobalLock(source);
    void* output=GlobalLock(dest);
    SIZE_T capacity=GlobalSize(dest);
    // Native allocation/bounds use SIZE_T; its checked copy receives a DWORD.
    UINT count=static_cast<UINT>(size);
    if (count && memcpy_s(output,capacity,input,count)) AfxThrowInvalidArgException();
    GlobalUnlock(dest);GlobalUnlock(source);return dest;
}
}
namespace openmfc { namespace detail {
BOOL CopyDataMedium(CLIPFORMAT cf,STGMEDIUM* dest,STGMEDIUM* source) {
    if (dest->tymed==TYMED_NULL) {
        switch (source->tymed) {
        case TYMED_HGLOBAL:case TYMED_GDI:case TYMED_ENHMF:
            dest->tymed=source->tymed;dest->hGlobal=nullptr;break;
        case TYMED_FILE: {
            dest->tymed=TYMED_FILE;
            if (!source->lpszFileName) AfxThrowInvalidArgException();
            ULONGLONG size=ULONGLONG(UINT(wcslen(source->lpszFileName)+1))*2;
            dest->lpszFileName=size>UINT_MAX?nullptr:static_cast<wchar_t*>(CoTaskMemAlloc(ULONG(size)));
            if (!dest->lpszFileName) return FALSE;
            if (size && memcpy_s(dest->lpszFileName,size,source->lpszFileName,size)) AfxThrowInvalidArgException();
            return TRUE;
        }
        case TYMED_ISTREAM:
            dest->pstm=source->pstm;dest->pstm->AddRef();dest->tymed=TYMED_ISTREAM;return TRUE;
        case TYMED_ISTORAGE:
            dest->pstg=source->pstg;dest->pstg->AddRef();dest->tymed=TYMED_ISTORAGE;return TRUE;
        case TYMED_MFPICT: {
            HGLOBAL result=CopyGlobal(nullptr,source->hMetaFilePict);
            if (!result) return FALSE;
            auto* pict=static_cast<METAFILEPICT*>(GlobalLock(result));
            pict->hMF=CopyMetaFileW(pict->hMF,nullptr);
            if (!pict->hMF) {GlobalUnlock(result);GlobalFree(result);return FALSE;}
            GlobalUnlock(result);dest->hMetaFilePict=result;dest->tymed=TYMED_MFPICT;return TRUE;
        }
        default:return FALSE;
        }
    }
    switch (source->tymed) {
    case TYMED_HGLOBAL: {
        auto result=CopyGlobal(dest->hGlobal,source->hGlobal);
        if (!result) return FALSE;
        dest->hGlobal=result;return TRUE;
    }
    case TYMED_FILE: {
        CString input(source->lpszFileName),output(dest->lpszFileName);
        return CopyFileW(source->lpszFileName?input.GetString():nullptr,dest->lpszFileName?output.GetString():nullptr,FALSE);
    }
    case TYMED_ISTREAM: {
        STATSTG status;
        if (source->pstm->Stat(&status,STATFLAG_NONAME)!=S_OK) return FALSE;
        dest->pstm->Seek(LARGE_INTEGER{},STREAM_SEEK_SET,nullptr);
        source->pstm->Seek(LARGE_INTEGER{},STREAM_SEEK_SET,nullptr);
        if (source->pstm->CopyTo(dest->pstm,status.cbSize,nullptr,nullptr)!=S_OK) return FALSE;
        dest->pstm->Seek(LARGE_INTEGER{},STREAM_SEEK_SET,nullptr);
        source->pstm->Seek(LARGE_INTEGER{},STREAM_SEEK_SET,nullptr);
        return TRUE;
    }
    case TYMED_ISTORAGE:return source->pstg->CopyTo(0,nullptr,nullptr,dest->pstg)==S_OK;
    case TYMED_GDI:case TYMED_ENHMF:
        if (dest->hGlobal) return FALSE;
        dest->hGlobal=OleDuplicateData(source->hGlobal,cf,0);return dest->hGlobal!=nullptr;
    default:return FALSE;
    }
}
} }
