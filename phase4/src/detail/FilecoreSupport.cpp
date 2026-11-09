#include "FilecoreSupport.h"
#include "openmfc/afxwin.h"

// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__FreeAssoc_CMapPtrToWord__IEAAXPEAUCAssoc_1___Z(CMapPtrToWord* /*pThis*/, void* pAssoc);
extern "C" void MS_ABI impl__FreeAssoc_CMapStringToOb__IEAAXPEAUCAssoc_1___Z(CMapStringToOb* /*pThis*/, void* pAssoc);
extern "C" void MS_ABI impl__FreeAssoc_CMapStringToPtr__IEAAXPEAUCAssoc_1___Z(CMapStringToPtr* /*pThis*/, void* pAssoc);
extern "C" void MS_ABI impl__FreeAssoc_CMapStringToString__IEAAXPEAVCAssoc_1___Z(CMapStringToString* /*pThis*/, void* pAssoc);
extern "C" void MS_ABI impl__FreeAssoc_CMapWordToPtr__IEAAXPEAUCAssoc_1___Z(CMapWordToPtr* /*pThis*/, void* pAssoc);
extern "C" void MS_ABI impl__FreeNode_CObList__IEAAXPEAUCNode_1___Z(CObList* /*pThis*/, void* pNode);
extern "C" void MS_ABI impl__FreeNode_CStringList__IEAAXPEAUCNode_1___Z(CStringList* /*pThis*/, void* pNode);
extern "C" void* MS_ABI impl__NewNode_CObList__IEAAPEAUCNode_1__PEAU21_0_Z(CObList* /*pThis*/, void* pPrev, void* pNext);
extern "C" void* MS_ABI impl__NewNode_CStringList__IEAAPEAUCNode_1__PEAU21_0_Z(CStringList* /*pThis*/, void* pPrev, void* pNext);

IMPLEMENT_DYNAMIC(CUIntArray, CObject)
IMPLEMENT_SERIAL(CDWordArray, CObject, 0xFFFF)
IMPLEMENT_DYNAMIC(CMapPtrToWord, CObject)
IMPLEMENT_SERIAL(CMapStringToOb, CObject, 0xFFFF)
IMPLEMENT_DYNAMIC(CMapStringToPtr, CObject)
IMPLEMENT_SERIAL(CMapStringToString, CObject, 0xFFFF)
IMPLEMENT_DYNAMIC(CMapWordToPtr, CObject)
OPENMFC_DEFINE_ARRAY_METHODS(CUIntArray, unsigned int, unsigned int)
OPENMFC_DEFINE_ARRAY_METHODS(CDWordArray, DWORD, DWORD)
OPENMFC_DEFINE_MAP_METHODS(CMapPtrToWord, void*, void*, WORD, WORD)
OPENMFC_DEFINE_MAP_METHODS(CMapWordToPtr, WORD, WORD, void*, void*)
#undef OPENMFC_DEFINE_LIST_METHODS
#undef OPENMFC_DEFINE_MAP_METHODS
#undef OPENMFC_DEFINE_ARRAY_METHODS
namespace openmfc { namespace detail { namespace filecore {
UINT HashCStringKey(const CString& key) {
    UINT hash = 0;
    const wchar_t* p = static_cast<const wchar_t*>(key);
    while (*p) {
        hash = (hash << 5) + hash + static_cast<UINT>(*p++);
    }
    return hash;
}
CStringMapStringState& EnsureCStringMapStringStorage(const CMapStringToString* self, int nBlockSize) {
    return EnsureCollectionState<CMapStringToString, CStringMapStringState>(self, ClampCollectionBlockSize(nBlockSize));
}
const CStringMapStringState* FindCStringMapStringStorage(const CMapStringToString* self) {
    return FindCollectionState<CMapStringToString, CStringMapStringState>(self);
}
static CRuntimeClass* AFXAPI _openmfc_gb_g_classCWordArray() { return &CObject::classCObject; }
CRuntimeClass g_classCWordArray = {
    "CWordArray", sizeof(CWordArray), 0xFFFF, nullptr, &_openmfc_gb_g_classCWordArray, nullptr, nullptr
};
extern "C" void MS_ABI impl__FreeNode_CStringList__IEAAXPEAUCNode_1__Z(CStringList* pThis, void* pNode) { impl__FreeNode_CStringList__IEAAXPEAUCNode_1___Z(pThis, pNode); }
extern "C" void* MS_ABI impl__NewNode_CStringList__IEAAPEAUCNode_1_PEAU21_0_Z(CStringList* pThis, void* pPrev, void* pNext) { return impl__NewNode_CStringList__IEAAPEAUCNode_1__PEAU21_0_Z(pThis, pPrev, pNext); }
extern "C" void MS_ABI impl__FreeNode_CObList__IEAAXPEAUCNode_1__Z(CObList* pThis, void* pNode) { impl__FreeNode_CObList__IEAAXPEAUCNode_1___Z(pThis, pNode); }
extern "C" void* MS_ABI impl__NewNode_CObList__IEAAPEAUCNode_1_PEAU21_0_Z(CObList* pThis, void* pPrev, void* pNext) { return impl__NewNode_CObList__IEAAPEAUCNode_1__PEAU21_0_Z(pThis, pPrev, pNext); }
extern "C" void MS_ABI impl__FreeAssoc_CMapPtrToWord__IEAAXPEAUCAssoc_1__Z(CMapPtrToWord* pThis, void* pAssoc) { impl__FreeAssoc_CMapPtrToWord__IEAAXPEAUCAssoc_1___Z(pThis, pAssoc); }
extern "C" void MS_ABI impl__FreeAssoc_CMapStringToOb__IEAAXPEAUCAssoc_1__Z(CMapStringToOb* pThis, void* pAssoc) { impl__FreeAssoc_CMapStringToOb__IEAAXPEAUCAssoc_1___Z(pThis, pAssoc); }
extern "C" void MS_ABI impl__FreeAssoc_CMapStringToPtr__IEAAXPEAUCAssoc_1__Z(CMapStringToPtr* pThis, void* pAssoc) { impl__FreeAssoc_CMapStringToPtr__IEAAXPEAUCAssoc_1___Z(pThis, pAssoc); }
extern "C" void MS_ABI impl__FreeAssoc_CMapStringToString__IEAAXPEAVCAssoc_1__Z(CMapStringToString* pThis, void* pAssoc) { impl__FreeAssoc_CMapStringToString__IEAAXPEAVCAssoc_1___Z(pThis, pAssoc); }
extern "C" void MS_ABI impl__FreeAssoc_CMapWordToPtr__IEAAXPEAUCAssoc_1__Z(CMapWordToPtr* pThis, void* pAssoc) { impl__FreeAssoc_CMapWordToPtr__IEAAXPEAUCAssoc_1___Z(pThis, pAssoc); }
void OpenStdioFile(CStdioFile* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags) {
    if (!pThis) return;
    FILE*& stream = CStdioFileAccessor::Stream(pThis);
    if (stream) {
        fclose(stream);
        stream = nullptr;
    }
    if (!lpszFileName) return;
    wchar_t szMode[4] = {0};
    int idx = 0;
    bool bText = (nOpenFlags & CFile::typeText) != 0;
    unsigned int accessMode = nOpenFlags & 0x0003;
    if (accessMode == CFile::modeRead) szMode[idx++] = L'r';
    else if (accessMode == CFile::modeWrite) szMode[idx++] = L'w';
    else { szMode[idx++] = (nOpenFlags & CFile::modeCreate) ? L'w' : L'r'; szMode[idx++] = L'+'; }
    if (!bText) szMode[idx++] = L'b';
    stream = _wfopen(lpszFileName, szMode);
    pThis->m_hFile = (void*)INVALID_HANDLE_VALUE;
    if (stream) {
        int fd = _fileno(stream);
        if (fd >= 0) {
            pThis->m_hFile = (void*)_get_osfhandle(fd);
        }
    }
    pThis->m_strFileName = lpszFileName;
}
CRuntimeClass* AFXAPI gb_CStdioFile() { return &CFile::classCFile; }
CRuntimeClass g_classCStdioFile = {
    "CStdioFile", 48, 0xFFFF, nullptr, gb_CStdioFile, nullptr, nullptr
};
int FileExceptionFromErrno(int error) {return CFileException::ErrnoToException(error);}

} } }  // namespace openmfc::detail::filecore
