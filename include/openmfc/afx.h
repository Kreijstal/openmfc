#pragma once
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cwchar>
#include "openmfc/afxstr.h"

// MFC CObject and CRuntimeClass - ABI compatible implementation
//
// CObject is the root of most MFC classes and provides:
// - Runtime type identification (RTTI)
// - Dynamic object creation
// - Serialization support
//
// sizeof(CObject) = 8 (just vptr on x64)

// OpenMFC is Windows-only - panic if not building for Windows
#if !defined(_WIN32) && !defined(__MINGW32__)
    #error "OpenMFC is Windows-only. Must build with MinGW or MSVC."
#endif

// OpenMFC is a Windows MFC compatibility layer
// It requires Windows or MinGW cross-compilation
#if !defined(_WIN32) && !defined(__MINGW32__)
    #error "OpenMFC requires Windows or MinGW cross-compilation. Cannot build on this platform."
#endif

#include <windows.h>

// Undefine Windows macros that conflict with MFC method names
// Windows defines LoadString as LoadStringW/LoadStringA which conflicts with CString::LoadString
#ifdef LoadString
#undef LoadString
#endif

// Calling convention macros
// Note: Most MFC global functions use __cdecl, not __stdcall
#define AFXAPI __cdecl

// DLL import/export macros
#ifdef OPENMFC_EXPORTS
    #define AFX_IMPORT __declspec(dllexport)
#else
    #define AFX_IMPORT __declspec(dllimport)
#endif

// For functions that are always imported from MFC DLL
#define AFX_IMPORT_FUNC AFX_IMPORT AFXAPI

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef NULL
#define NULL 0
#endif

// Windows type definitions
#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef WORD
typedef unsigned short WORD;
#endif

#ifndef DWORD
typedef unsigned long DWORD;
#endif

#ifndef LONG
typedef long LONG;
#endif

#ifndef ULONGLONG
typedef unsigned long long ULONGLONG;
#endif

#ifndef LONGLONG
typedef long long LONGLONG;
#endif

#ifndef UINT
typedef unsigned int UINT;
#endif

#ifndef BOOL
typedef int BOOL;
#endif

#ifndef _WINDOWS_
    #ifndef HANDLE
    typedef void* HANDLE;
    #endif
    #ifndef LPVOID
    typedef void* LPVOID;
    #endif

    #ifndef HWND
    typedef void* HWND;
    #endif

    #ifndef WPARAM
    typedef unsigned long long WPARAM;
    #endif

    #ifndef LPARAM
    typedef long long LPARAM;
    #endif

    // POINT is defined in afxwin.h

    #ifndef LPSECURITY_ATTRIBUTES
    typedef void* LPSECURITY_ATTRIBUTES;
    #endif

    // MSG structure is defined in afxwin.h

    #ifndef _MAX_PATH
    #define _MAX_PATH 260
    #endif

    // Windows wait constants
    #ifndef WAIT_FAILED
    #define WAIT_FAILED 0xFFFFFFFF
    #endif

    #ifndef WAIT_OBJECT_0
    #define WAIT_OBJECT_0 0
    #endif

    #ifndef WAIT_TIMEOUT
    #define WAIT_TIMEOUT 0x00000102
    #endif

    #ifndef INFINITE
    #define INFINITE 0xFFFFFFFF
    #endif
#endif

// File attribute constants
#ifndef FILE_ATTRIBUTE_READONLY
#define FILE_ATTRIBUTE_READONLY 0x00000001
#endif

#ifndef FILE_ATTRIBUTE_DIRECTORY
#define FILE_ATTRIBUTE_DIRECTORY 0x00000010
#endif

class CObject;
class CArchive;
class CFile;
class CException;
class CFileException;

// CFileStatus - file status structure
struct CFileStatus {
    ULONGLONG m_ctime;      // Creation time (simplified)
    ULONGLONG m_mtime;      // Last modification time (simplified)
    ULONGLONG m_atime;      // Last access time (simplified)
    ULONGLONG m_size;       // File size
    BYTE m_attribute;       // File attribute
    BYTE m_padding;         // Padding
    wchar_t m_szFullName[_MAX_PATH];  // Absolute path name
};

// CRuntimeClass - runtime type information structure
// This structure is used for MFC's own RTTI system (separate from C++ RTTI)
// IMPORTANT: Layout must match real MFC exactly for ABI compatibility!
struct CRuntimeClass {
    // IMPORTANT: Field order must match real MFC exactly for ABI compatibility!
    const char* m_lpszClassName;      // Class name string (offset 0)
    int m_nObjectSize;                // sizeof(class) (offset 8)
    unsigned int m_wSchema;           // Schema number for serialization (offset 12)
    CObject* (AFXAPI *m_pfnCreateObject)();  // Factory function (offset 16)
    CRuntimeClass* (AFXAPI *m_pfnGetBaseClass)(); // Get base class for DLL (offset 24)
    CRuntimeClass* m_pBaseClass;      // Pointer to base class (offset 32)
    CRuntimeClass* m_pNextClass;      // Linked list of registered classes (offset 40)

    // Helper methods
    CObject* CreateObject() const {
        if (m_pfnCreateObject != nullptr) {
            return m_pfnCreateObject();
        }
        return nullptr;
    }

    bool IsDerivedFrom(const CRuntimeClass* pBaseClass) const {
        const CRuntimeClass* pClassThis = this;
        while (pClassThis != nullptr) {
            if (pClassThis == pBaseClass) {
                return true;
            }
            // Use m_pfnGetBaseClass if available (DLL linking), otherwise m_pBaseClass
            if (pClassThis->m_pfnGetBaseClass) {
                pClassThis = pClassThis->m_pfnGetBaseClass();
            } else {
                pClassThis = pClassThis->m_pBaseClass;
            }
        }
        return false;
    }
};

// DECLARE_DYNAMIC - adds runtime class support to a class
// Use in class declaration (public section)
#define DECLARE_DYNAMIC(class_name) \
public: \
    static CRuntimeClass class##class_name; \
    static CRuntimeClass* GetThisClass() { return &class##class_name; } \
    virtual CRuntimeClass* GetRuntimeClass() const override { return GetThisClass(); }

// DECLARE_DYNCREATE - adds runtime class + factory support
#define DECLARE_DYNCREATE(class_name) \
    DECLARE_DYNAMIC(class_name) \
    static CObject* AFXAPI CreateObject() { return new class_name; }

// IMPLEMENT_DYNAMIC - implements runtime class (put in .cpp file)
// Order must match CRuntimeClass layout: lpszClassName, nObjectSize, wSchema, pfnCreateObject, pfnGetBaseClass, pBaseClass, pNextClass
#define IMPLEMENT_DYNAMIC(class_name, base_class_name) \
    CRuntimeClass class_name::class##class_name = { \
        #class_name, \
        sizeof(class_name), \
        0xFFFF, \
        nullptr, \
        nullptr, \
        &base_class_name::class##base_class_name, \
        nullptr \
    };

// IMPLEMENT_DYNCREATE - implements runtime class with factory
#define IMPLEMENT_DYNCREATE(class_name, base_class_name) \
    CRuntimeClass class_name::class##class_name = { \
        #class_name, \
        sizeof(class_name), \
        0xFFFF, \
        &class_name::CreateObject, \
        nullptr, \
        &base_class_name::class##base_class_name, \
        nullptr \
    };

// DECLARE_SERIAL - adds serialization support to a class
// Extends DECLARE_DYNCREATE with archive extraction operator
#define DECLARE_SERIAL(class_name) \
    DECLARE_DYNCREATE(class_name) \
    friend CArchive& operator>>(CArchive& ar, class_name*& pOb);

// IMPLEMENT_SERIAL - implements runtime class with serialization
// Sets a specific schema version for versioned serialization
#define IMPLEMENT_SERIAL(class_name, base_class_name, wSchema) \
    CObject* AFXAPI _AfxGetObject##class_name(CArchive& ar) { \
        (void)ar; \
        return class_name::CreateObject(); \
    } \
    CArchive& operator>>(CArchive& ar, class_name*& pOb) { \
        pOb = static_cast<class_name*>(_AfxGetObject##class_name(ar)); \
        if (pOb != nullptr) pOb->Serialize(ar); \
        return ar; \
    } \
    CRuntimeClass class_name::class##class_name = { \
        #class_name, \
        sizeof(class_name), \
        wSchema, \
        &class_name::CreateObject, \
        nullptr, \
        &base_class_name::class##base_class_name, \
        nullptr \
    };

// RUNTIME_CLASS macro - get CRuntimeClass pointer for a class
#define RUNTIME_CLASS(class_name) (&class_name::class##class_name)

// CObject - base class for most MFC classes
// IMPORTANT: Virtual method declaration order matches real MFC for vtable ABI compatibility
// Real MFC declares GetRuntimeClass() before destructor, so vtable[0] = GetRuntimeClass
class CObject {
public:
    // Runtime class information - MUST be first virtual for vtable[0] compatibility
    virtual CRuntimeClass* GetRuntimeClass() const {
        return GetThisClass();
    }

    // Virtual destructor - second in vtable
    virtual ~CObject() = default;
    
    // Static runtime class access
    static CRuntimeClass* GetThisClass() {
        return &classCObject;
    }
    
    // Type checking
    bool IsKindOf(const CRuntimeClass* pClass) const {
        CRuntimeClass* pThisClass = GetRuntimeClass();
        return pThisClass->IsDerivedFrom(pClass);
    }
    
    // Serialization (stub - needs CArchive implementation)
    virtual void Serialize(CArchive& ar);
    
    // Diagnostic support (simplified)
    virtual void AssertValid() const {}
    virtual void Dump() const {}
    
    // Static CRuntimeClass for CObject
    static CRuntimeClass classCObject;
    
protected:
    // Protected constructors (CObject shouldn't be directly instantiated in MFC)
    CObject() = default;
    CObject(const CObject&) = default;
    CObject& operator=(const CObject&) = default;
};

// Static assertions to verify ABI compatibility
static_assert(sizeof(CObject) == 8, "CObject must be 8 bytes (vptr only)");

// CArchive - serialization archive class
class CArchive {
public:
    enum Mode { store = 0, load = 1, bNoFlushOnDelete = 2, bNoByteSwap = 4 };
    
    CArchive(CFile* pFile, UINT nMode, int nBufSize = 4096, void* lpBuf = nullptr);
    virtual ~CArchive();
    
    // Basic operations
    bool IsLoading() const { return (m_nMode & load) != 0; }
    bool IsStoring() const { return !IsLoading(); }
    CFile* GetFile() const { return m_pFile; }
    UINT GetObjectSchema() const { return m_nObjectSchema; }
    void SetObjectSchema(UINT nSchema) { m_nObjectSchema = nSchema; }
    
    // Reading operations
    CArchive& operator>>(bool& b);
    CArchive& operator>>(char& ch);
    CArchive& operator>>(unsigned char& ch);
    CArchive& operator>>(short& w);
    CArchive& operator>>(unsigned short& w);
    CArchive& operator>>(int& i);
    CArchive& operator>>(unsigned int& i);
    CArchive& operator>>(long& l);
    CArchive& operator>>(unsigned long& l);
    CArchive& operator>>(float& f);
    CArchive& operator>>(double& d);
    CArchive& operator>>(CObject*& pOb);
    CArchive& operator>>(CString& str);
    
    // Writing operations
    CArchive& operator<<(bool b);
    CArchive& operator<<(char ch);
    CArchive& operator<<(unsigned char ch);
    CArchive& operator<<(short w);
    CArchive& operator<<(unsigned short w);
    CArchive& operator<<(int i);
    CArchive& operator<<(unsigned int i);
    CArchive& operator<<(long l);
    CArchive& operator<<(unsigned long l);
    CArchive& operator<<(float f);
    CArchive& operator<<(double d);
    CArchive& operator<<(const CObject* pOb);
    CArchive& operator<<(const CString& str);
    
    // String operations
    int ReadString(wchar_t* lpsz, UINT nMax);
    int ReadString(CString& rString);
    void WriteString(const wchar_t* lpsz);
    
    // Raw read/write
    UINT Read(void* lpBuf, UINT nMax);
    void Write(const void* lpBuf, UINT nMax);
    
    // Flush buffer
    void Flush();
    
    // Close archive
    void Close();
    
    // Exception handling
    void Abort();

protected:
    CFile* m_pFile;
    UINT m_nMode;
    int m_nBufSize;
    unsigned char* m_lpBufStart;
    unsigned char* m_lpBufCur;
    unsigned char* m_lpBufMax;
    UINT m_nObjectSchema;
    bool m_bForceFlat;
    bool m_bUserBuf;
    
    void FillBuffer(UINT nBytesNeeded);
    void WriteBuffer();
};

// CFile - base file class
class CFile {
public:
    enum OpenFlags {
        modeRead = 0x0000,
        modeWrite = 0x0001,
        modeReadWrite = 0x0002,
        shareCompat = 0x0000,
        shareExclusive = 0x0010,
        shareDenyWrite = 0x0020,
        shareDenyRead = 0x0030,
        shareDenyNone = 0x0040,
        modeNoInherit = 0x0080,
        modeCreate = 0x1000,
        modeNoTruncate = 0x2000,
        typeText = 0x4000,
        typeBinary = 0x8000
    };
    
    enum SeekPosition { begin = 0, current = 1, end = 2 };
    
    enum { hFileNull = -1 };
    
    CFile();
    CFile(const wchar_t* lpszFileName, UINT nOpenFlags);
    virtual ~CFile();
    
    // File operations
    virtual UINT Read(void* lpBuf, UINT nCount);
    virtual void Write(const void* lpBuf, UINT nCount);
    virtual ULONGLONG Seek(LONGLONG lOff, UINT nFrom);
    virtual void SetLength(ULONGLONG dwNewLen);
    virtual ULONGLONG GetLength() const;
    virtual void Flush();
    virtual void Close();
    
    // Status
    virtual void SetFilePath(const wchar_t* lpszNewName);
    virtual CString GetFileName() const;
    virtual CString GetFileTitle() const;
    virtual CString GetFilePath() const;
    
    // Static operations
    static void Rename(const wchar_t* lpszOldName, const wchar_t* lpszNewName);
    static void Remove(const wchar_t* lpszFileName);
    static bool GetStatus(const wchar_t* lpszFileName, CFileStatus& rStatus);
    static void SetStatus(const wchar_t* lpszFileName, const CFileStatus& status);
    
protected:
    void* m_hFile;  // Platform-specific file handle
    CString m_strFileName;
    
    void CommonInit(const wchar_t* lpszFileName, UINT nOpenFlags, void* pTM = nullptr);
};

// CMemFile - memory file
class CMemFile : public CFile {
public:
    CMemFile(UINT nGrowBytes = 1024);
    CMemFile(BYTE* lpBuffer, UINT nBufferSize, UINT nGrowBytes = 0);
    virtual ~CMemFile();
    
    virtual UINT Read(void* lpBuf, UINT nCount) override;
    virtual void Write(const void* lpBuf, UINT nCount) override;
    virtual ULONGLONG Seek(LONGLONG lOff, UINT nFrom) override;
    virtual void SetLength(ULONGLONG dwNewLen) override;
    virtual ULONGLONG GetLength() const override;
    virtual void Flush() override;
    
    BYTE* Detach();
    void Attach(BYTE* lpBuffer, UINT nBufferSize, UINT nGrowBytes = 0);
    
protected:
    BYTE* m_lpBuffer;
    UINT m_nBufferSize;
    UINT m_nFileSize;
    UINT m_nGrowBytes;
    UINT m_nPosition;
    bool m_bAutoDelete;
};

// CStdioFile - stdio file
class CStdioFile : public CFile {
public:
    CStdioFile();
    CStdioFile(const wchar_t* lpszFileName, UINT nOpenFlags);
    virtual ~CStdioFile();
    
    virtual UINT Read(void* lpBuf, UINT nCount) override;
    virtual void Write(const void* lpBuf, UINT nCount) override;
    virtual ULONGLONG Seek(LONGLONG lOff, UINT nFrom) override;
    virtual void Flush() override;
    virtual void Close() override;
    
    // String operations
    bool ReadString(wchar_t* lpsz, UINT nMax);
    bool ReadString(CString& rString);
    void WriteString(const wchar_t* lpsz);
    
protected:
    FILE* m_pStream;
};



// Template collection classes
template<class TYPE, class ARG_TYPE = const TYPE&>
class CArray {
protected:
    TYPE* m_pData;
    int m_nSize;
    int m_nMaxSize;
    int m_nGrowBy;
    
public:
    CArray();
    ~CArray();
    
    int GetSize() const { return m_nSize; }
    int GetCount() const { return m_nSize; }
    bool IsEmpty() const { return m_nSize == 0; }
    int GetUpperBound() const { return m_nSize - 1; }
    void SetSize(int nNewSize, int nGrowBy = -1);
    void FreeExtra();
    void RemoveAll();
    
    TYPE GetAt(int nIndex) const { return m_pData[nIndex]; }
    void SetAt(int nIndex, ARG_TYPE newElement) { m_pData[nIndex] = newElement; }
    TYPE& ElementAt(int nIndex) { return m_pData[nIndex]; }
    const TYPE& ElementAt(int nIndex) const { return m_pData[nIndex]; }
    
    TYPE operator[](int nIndex) const { return GetAt(nIndex); }
    TYPE& operator[](int nIndex) { return ElementAt(nIndex); }
    
    TYPE* GetData() { return m_pData; }
    const TYPE* GetData() const { return m_pData; }
    
    void SetAtGrow(int nIndex, ARG_TYPE newElement);
    int Add(ARG_TYPE newElement);
    int Append(const CArray& src);
    void Copy(const CArray& src);
    
    void InsertAt(int nIndex, ARG_TYPE newElement, int nCount = 1);
    void RemoveAt(int nIndex, int nCount = 1);
    void InsertAt(int nStartIndex, CArray* pNewArray);
};

template<class TYPE, class ARG_TYPE = const TYPE&>
class CList {
protected:
    struct CNode {
        CNode* pNext;
        CNode* pPrev;
        TYPE data;
    };
    
    CNode* m_pNodeHead;
    CNode* m_pNodeTail;
    int m_nCount;
    CNode* m_pNodeFree;
    int m_nBlockSize;
    
private:
    struct CBlock {
        CBlock* pNext;
    };
    CBlock* m_pBlocks;
    
public:
    struct POSITION {
        CNode* pNode;
        POSITION(CNode* p = nullptr) : pNode(p) {}
        bool operator==(const POSITION& pos) const { return pNode == pos.pNode; }
        bool operator!=(const POSITION& pos) const { return pNode != pos.pNode; }
    };
    
    CList(int nBlockSize = 10);
    ~CList();
    
    int GetCount() const { return m_nCount; }
    bool IsEmpty() const { return m_nCount == 0; }
    
    TYPE& GetHead() { return m_pNodeHead->data; }
    const TYPE& GetHead() const { return m_pNodeHead->data; }
    TYPE& GetTail() { return m_pNodeTail->data; }
    const TYPE& GetTail() const { return m_pNodeTail->data; }
    
    POSITION GetHeadPosition() const { return POSITION(m_pNodeHead); }
    POSITION GetTailPosition() const { return POSITION(m_pNodeTail); }
    
    TYPE& GetNext(POSITION& rPosition);
    const TYPE& GetNext(POSITION& rPosition) const;
    TYPE& GetPrev(POSITION& rPosition);
    const TYPE& GetPrev(POSITION& rPosition) const;
    
    TYPE GetAt(POSITION position) const;
    void SetAt(POSITION pos, ARG_TYPE newElement);
    void RemoveAt(POSITION position);
    
    POSITION FindIndex(int nIndex) const;
    POSITION Find(ARG_TYPE searchValue, POSITION startAfter = nullptr) const;
    
    void AddHead(ARG_TYPE newElement);
    void AddTail(ARG_TYPE newElement);
    void AddHead(CList* pNewList);
    void AddTail(CList* pNewList);
    
    TYPE RemoveHead();
    TYPE RemoveTail();
    
    void InsertBefore(POSITION position, ARG_TYPE newElement);
    void InsertAfter(POSITION position, ARG_TYPE newElement);
    
    void RemoveAll();
    
private:
    CNode* NewNode(CNode* pPrev, CNode* pNext);
};

// SerializeElements template function
template<class TYPE>
void SerializeElements(CArchive& ar, TYPE* pElements, int nCount) {
    if (ar.IsStoring()) {
        for (int i = 0; i < nCount; i++) {
            ar << pElements[i];
        }
    } else {
        for (int i = 0; i < nCount; i++) {
            ar >> pElements[i];
        }
    }
}

// Specialization for CString
template<>
void SerializeElements<CString>(CArchive& ar, CString* pElements, int nCount);

// CMap template - hash table implementation
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE = const VALUE&>
class CMap {
protected:
    struct CAssoc {
        CAssoc* pNext;
        UINT nHashValue;
        KEY key;
        VALUE value;
    };
    
    CAssoc** m_pHashTable;
    UINT m_nHashTableSize;
    int m_nCount;
    CAssoc* m_pFreeList;
    struct CBlock {
        CBlock* pNext;
    };
    CBlock* m_pBlocks;
    int m_nBlockSize;
    
public:
    struct POSITION {
        CAssoc* pAssoc;
        UINT nHashBucket;
        POSITION(CAssoc* p = nullptr, UINT n = 0) : pAssoc(p), nHashBucket(n) {}
        bool operator==(const POSITION& pos) const { return pAssoc == pos.pAssoc; }
        bool operator!=(const POSITION& pos) const { return pAssoc != pos.pAssoc; }
    };
    
    CMap(int nBlockSize = 10);
    ~CMap();
    
    int GetCount() const { return m_nCount; }
    bool IsEmpty() const { return m_nCount == 0; }
    
    // Lookup/access operations
    bool Lookup(ARG_KEY key, VALUE& rValue) const;
    bool LookupKey(ARG_KEY key, KEY& rKey) const;
    VALUE& operator[](ARG_KEY key);
    const VALUE& operator[](ARG_KEY key) const;
    
    // POSITION-based iteration
    POSITION GetStartPosition() const;
    void GetNextAssoc(POSITION& rNextPosition, KEY& rKey, VALUE& rValue) const;
    
    // Add/remove operations
    void SetAt(ARG_KEY key, ARG_VALUE newValue);
    bool RemoveKey(ARG_KEY key);
    void RemoveAll();
    
    // Hash table operations
    UINT GetHashTableSize() const { return m_nHashTableSize; }
    void InitHashTable(UINT hashSize, bool bAllocNow = true);
    
protected:
    CAssoc* NewAssoc();
    void FreeAssoc(CAssoc* pAssoc);
    UINT HashKey(ARG_KEY key) const;
    CAssoc* GetAssocAt(ARG_KEY key, UINT& nHash, UINT& nHashBucket) const;
};

// Type definitions for common collections
typedef CArray<int, int> CIntArray;
typedef CArray<unsigned int, unsigned int> CUIntArray;
typedef CArray<WORD, WORD> CWordArray;
typedef CArray<DWORD, DWORD> CDWordArray;
typedef CArray<CString, const CString&> CStringArray;
typedef CArray<CObject*, CObject*> CObArray;
typedef CArray<void*, void*> CPtrArray;

typedef CList<int, int> CIntList;
typedef CList<unsigned int, unsigned int> CUIntList;
typedef CList<WORD, WORD> CWordList;
typedef CList<DWORD, DWORD> CDWordList;
typedef CList<CString, const CString&> CStringList;
typedef CList<CObject*, CObject*> CObList;
typedef CList<void*, void*> CPtrList;

// Byte array (missing from above)
typedef CArray<BYTE, BYTE> CByteArray;

// String map classes (non-template versions for ABI compatibility)
class CMapStringToOb : public CMap<CString, const CString&, CObject*, CObject*> {
public:
    CMapStringToOb(int nBlockSize = 10) : CMap(nBlockSize) {}
    
    // Additional string-specific methods
    CObject*& operator[](const wchar_t* key);
    bool Lookup(const wchar_t* key, CObject*& rValue) const;
    void SetAt(const wchar_t* key, CObject* newValue);
};

class CMapStringToPtr : public CMap<CString, const CString&, void*, void*> {
public:
    CMapStringToPtr(int nBlockSize = 10) : CMap(nBlockSize) {}
    
    // Additional string-specific methods
    void*& operator[](const wchar_t* key);
    bool Lookup(const wchar_t* key, void*& rValue) const;
    void SetAt(const wchar_t* key, void* newValue);
};

class CMapStringToString : public CMap<CString, const CString&, CString, const CString&> {
public:
    CMapStringToString(int nBlockSize = 10) : CMap(nBlockSize) {}
    
    // Additional string-specific methods
    CString& operator[](const wchar_t* key);
    bool Lookup(const wchar_t* key, CString& rValue) const;
    void SetAt(const wchar_t* key, const CString& newValue);
    void SetAt(const wchar_t* key, const wchar_t* newValue);
};

// CWinThread forward declaration - defined in afxwin.h
class CWinThread;

// Collection serialization operators
template<class TYPE, class ARG_TYPE>
CArchive& operator<<(CArchive& ar, const CArray<TYPE, ARG_TYPE>& array) {
    ar << (WORD)array.GetSize();
    SerializeElements(ar, array.GetData(), array.GetSize());
    return ar;
}

template<class TYPE, class ARG_TYPE>
CArchive& operator>>(CArchive& ar, CArray<TYPE, ARG_TYPE>& array) {
    WORD nSize;
    ar >> nSize;
    array.SetSize(nSize);
    SerializeElements(ar, array.GetData(), nSize);
    return ar;
}

template<class TYPE, class ARG_TYPE>
CArchive& operator<<(CArchive& ar, const CList<TYPE, ARG_TYPE>& list) {
    ar << (WORD)list.GetCount();
    typename CList<TYPE, ARG_TYPE>::POSITION pos = list.GetHeadPosition();
    while (pos != nullptr) {
        TYPE element = list.GetNext(pos);
        ar << element;
    }
    return ar;
}

template<class TYPE, class ARG_TYPE>
CArchive& operator>>(CArchive& ar, CList<TYPE, ARG_TYPE>& list) {
    WORD nSize;
    ar >> nSize;
    list.RemoveAll();
    for (WORD i = 0; i < nSize; i++) {
        TYPE element;
        ar >> element;
        list.AddTail(element);
    }
    return ar;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
CArchive& operator<<(CArchive& ar, const CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>& map) {
    ar << (WORD)map.GetCount();
    typename CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::POSITION pos = map.GetStartPosition();
    while (pos != nullptr) {
        KEY key;
        VALUE value;
        map.GetNextAssoc(pos, key, value);
        ar << key << value;
    }
    return ar;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
CArchive& operator>>(CArchive& ar, CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>& map) {
    WORD nSize;
    ar >> nSize;
    map.RemoveAll();
    for (WORD i = 0; i < nSize; i++) {
        KEY key;
        VALUE value;
        ar >> key >> value;
        map.SetAt(key, value);
    }
    return ar;
}

// Implementation of CObject's static runtime class
// Note: This should be in a .cpp file normally, but for header-only we inline it
#ifndef COBJECT_IMPL_DEFINED
#define COBJECT_IMPL_DEFINED
// Order: lpszClassName, nObjectSize, wSchema, pfnCreateObject, pfnGetBaseClass, pBaseClass, pNextClass
inline CRuntimeClass CObject::classCObject = {
    "CObject",
    sizeof(CObject),
    0xFFFF,         // m_wSchema
    nullptr,        // m_pfnCreateObject
    nullptr,        // m_pfnGetBaseClass
    nullptr,        // m_pBaseClass - CObject is root
    nullptr         // m_pNextClass
};

// CObject::Serialize default implementation (does nothing for base class)
inline void CObject::Serialize(CArchive& ar) {
    (void)ar;  // Base CObject::Serialize does nothing
}
#endif