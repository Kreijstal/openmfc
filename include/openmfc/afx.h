#pragma once
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cwchar>
#include <new>  // For placement new in collection templates
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
// - Avoid dllimport for app-defined classes when not using _AFXDLL.
// - Avoid dllexport under MinGW; exports are driven by the .def file.
#if defined(OPENMFC_EXPORTS)
    #if defined(__MINGW32__)
        #define AFX_IMPORT
    #else
        #define AFX_IMPORT __declspec(dllexport)
    #endif
#elif defined(_AFXDLL)
    #define AFX_IMPORT __declspec(dllimport)
#else
    #define AFX_IMPORT
#endif

// For functions that are always imported from MFC DLL
#define AFX_IMPORT_FUNC AFX_IMPORT AFXAPI
// For data imported/exported from the DLL (runtime class statics, globals)
#define AFX_DATA AFX_IMPORT

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
    static AFX_DATA CRuntimeClass class##class_name; \
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
    static AFX_DATA CRuntimeClass classCObject;
    
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

// ============================================================================
// CArray template implementation
// ============================================================================

template<class TYPE, class ARG_TYPE>
CArray<TYPE, ARG_TYPE>::CArray() : m_pData(nullptr), m_nSize(0), m_nMaxSize(0), m_nGrowBy(0) {}

template<class TYPE, class ARG_TYPE>
CArray<TYPE, ARG_TYPE>::~CArray() {
    if (m_pData != nullptr) {
        for (int i = 0; i < m_nSize; i++)
            m_pData[i].~TYPE();
        free(m_pData);
    }
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::SetSize(int nNewSize, int nGrowBy) {
    if (nGrowBy >= 0)
        m_nGrowBy = nGrowBy;

    if (nNewSize == 0) {
        if (m_pData != nullptr) {
            for (int i = 0; i < m_nSize; i++)
                m_pData[i].~TYPE();
            free(m_pData);
            m_pData = nullptr;
        }
        m_nSize = m_nMaxSize = 0;
    } else if (nNewSize <= m_nMaxSize) {
        if (nNewSize > m_nSize) {
            for (int i = m_nSize; i < nNewSize; i++)
                new (&m_pData[i]) TYPE();
        } else if (nNewSize < m_nSize) {
            for (int i = nNewSize; i < m_nSize; i++)
                m_pData[i].~TYPE();
        }
        m_nSize = nNewSize;
    } else {
        int nNewMax = (m_nGrowBy > 0) ? m_nMaxSize + m_nGrowBy : m_nMaxSize + m_nMaxSize / 2;
        if (nNewMax < nNewSize)
            nNewMax = nNewSize;
        TYPE* pNewData = (TYPE*)malloc(nNewMax * sizeof(TYPE));
        if (m_pData != nullptr) {
            for (int i = 0; i < m_nSize; i++) {
                new (&pNewData[i]) TYPE(m_pData[i]);
                m_pData[i].~TYPE();
            }
            free(m_pData);
        }
        for (int i = m_nSize; i < nNewSize; i++)
            new (&pNewData[i]) TYPE();
        m_pData = pNewData;
        m_nSize = nNewSize;
        m_nMaxSize = nNewMax;
    }
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::FreeExtra() {
    if (m_nSize != m_nMaxSize) {
        TYPE* pNewData = nullptr;
        if (m_nSize > 0) {
            pNewData = (TYPE*)malloc(m_nSize * sizeof(TYPE));
            for (int i = 0; i < m_nSize; i++) {
                new (&pNewData[i]) TYPE(m_pData[i]);
                m_pData[i].~TYPE();
            }
        }
        free(m_pData);
        m_pData = pNewData;
        m_nMaxSize = m_nSize;
    }
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::RemoveAll() {
    SetSize(0);
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::SetAtGrow(int nIndex, ARG_TYPE newElement) {
    if (nIndex >= m_nSize)
        SetSize(nIndex + 1);
    m_pData[nIndex] = newElement;
}

template<class TYPE, class ARG_TYPE>
int CArray<TYPE, ARG_TYPE>::Add(ARG_TYPE newElement) {
    int nIndex = m_nSize;
    SetAtGrow(nIndex, newElement);
    return nIndex;
}

template<class TYPE, class ARG_TYPE>
int CArray<TYPE, ARG_TYPE>::Append(const CArray& src) {
    int nOldSize = m_nSize;
    SetSize(m_nSize + src.m_nSize);
    for (int i = 0; i < src.m_nSize; i++)
        m_pData[nOldSize + i] = src.m_pData[i];
    return nOldSize;
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::Copy(const CArray& src) {
    SetSize(src.m_nSize);
    for (int i = 0; i < src.m_nSize; i++)
        m_pData[i] = src.m_pData[i];
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::InsertAt(int nIndex, ARG_TYPE newElement, int nCount) {
    if (nIndex >= m_nSize) {
        SetSize(nIndex + nCount);
    } else {
        int nOldSize = m_nSize;
        SetSize(m_nSize + nCount);
        for (int i = nOldSize - 1; i >= nIndex; i--)
            m_pData[i + nCount] = m_pData[i];
    }
    for (int i = 0; i < nCount; i++)
        m_pData[nIndex + i] = newElement;
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::RemoveAt(int nIndex, int nCount) {
    if (nCount <= 0) {
        return;
    }
    int nOldSize = m_nSize;
    int nMoveCount = m_nSize - (nIndex + nCount);
    for (int i = 0; i < nMoveCount; i++)
        m_pData[nIndex + i] = m_pData[nIndex + nCount + i];
    for (int i = nOldSize - nCount; i < nOldSize; i++)
        m_pData[i].~TYPE();
    m_nSize = nOldSize - nCount;
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::InsertAt(int nStartIndex, CArray* pNewArray) {
    if (pNewArray->GetSize() > 0) {
        InsertAt(nStartIndex, pNewArray->GetAt(0), pNewArray->GetSize());
        for (int i = 1; i < pNewArray->GetSize(); i++)
            m_pData[nStartIndex + i] = pNewArray->GetAt(i);
    }
}

// ============================================================================
// CList template implementation
// ============================================================================

template<class TYPE, class ARG_TYPE>
CList<TYPE, ARG_TYPE>::CList(int nBlockSize)
    : m_pNodeHead(nullptr), m_pNodeTail(nullptr), m_nCount(0),
      m_pNodeFree(nullptr), m_nBlockSize(nBlockSize), m_pBlocks(nullptr) {}

template<class TYPE, class ARG_TYPE>
CList<TYPE, ARG_TYPE>::~CList() {
    RemoveAll();
    // Free block memory
    while (m_pBlocks != nullptr) {
        CBlock* pNext = m_pBlocks->pNext;
        free(m_pBlocks);
        m_pBlocks = pNext;
    }
}

template<class TYPE, class ARG_TYPE>
typename CList<TYPE, ARG_TYPE>::CNode* CList<TYPE, ARG_TYPE>::NewNode(CNode* pPrev, CNode* pNext) {
    if (m_pNodeFree == nullptr) {
        // Allocate a new block of nodes
        size_t blockSize = sizeof(CBlock) + m_nBlockSize * sizeof(CNode);
        CBlock* pNewBlock = (CBlock*)malloc(blockSize);
        pNewBlock->pNext = m_pBlocks;
        m_pBlocks = pNewBlock;

        // Chain the new nodes onto the free list
        CNode* pNode = (CNode*)(pNewBlock + 1);
        for (int i = 0; i < m_nBlockSize - 1; i++) {
            pNode[i].pNext = &pNode[i + 1];
        }
        pNode[m_nBlockSize - 1].pNext = nullptr;
        m_pNodeFree = pNode;
    }

    CNode* pNode = m_pNodeFree;
    m_pNodeFree = m_pNodeFree->pNext;

    pNode->pPrev = pPrev;
    pNode->pNext = pNext;
    new (&pNode->data) TYPE();
    m_nCount++;

    return pNode;
}

template<class TYPE, class ARG_TYPE>
TYPE& CList<TYPE, ARG_TYPE>::GetNext(POSITION& rPosition) {
    CNode* pNode = rPosition.pNode;
    rPosition.pNode = pNode->pNext;
    return pNode->data;
}

template<class TYPE, class ARG_TYPE>
const TYPE& CList<TYPE, ARG_TYPE>::GetNext(POSITION& rPosition) const {
    CNode* pNode = rPosition.pNode;
    rPosition.pNode = pNode->pNext;
    return pNode->data;
}

template<class TYPE, class ARG_TYPE>
TYPE& CList<TYPE, ARG_TYPE>::GetPrev(POSITION& rPosition) {
    CNode* pNode = rPosition.pNode;
    rPosition.pNode = pNode->pPrev;
    return pNode->data;
}

template<class TYPE, class ARG_TYPE>
const TYPE& CList<TYPE, ARG_TYPE>::GetPrev(POSITION& rPosition) const {
    CNode* pNode = rPosition.pNode;
    rPosition.pNode = pNode->pPrev;
    return pNode->data;
}

template<class TYPE, class ARG_TYPE>
TYPE CList<TYPE, ARG_TYPE>::GetAt(POSITION position) const {
    return position.pNode->data;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::SetAt(POSITION pos, ARG_TYPE newElement) {
    pos.pNode->data = newElement;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::RemoveAt(POSITION position) {
    CNode* pNode = position.pNode;

    if (pNode->pPrev != nullptr)
        pNode->pPrev->pNext = pNode->pNext;
    else
        m_pNodeHead = pNode->pNext;

    if (pNode->pNext != nullptr)
        pNode->pNext->pPrev = pNode->pPrev;
    else
        m_pNodeTail = pNode->pPrev;

    pNode->data.~TYPE();
    pNode->pNext = m_pNodeFree;
    m_pNodeFree = pNode;
    m_nCount--;
}

template<class TYPE, class ARG_TYPE>
typename CList<TYPE, ARG_TYPE>::POSITION CList<TYPE, ARG_TYPE>::FindIndex(int nIndex) const {
    if (nIndex >= m_nCount || nIndex < 0)
        return POSITION(nullptr);

    CNode* pNode = m_pNodeHead;
    for (int i = 0; i < nIndex; i++)
        pNode = pNode->pNext;
    return POSITION(pNode);
}

template<class TYPE, class ARG_TYPE>
typename CList<TYPE, ARG_TYPE>::POSITION CList<TYPE, ARG_TYPE>::Find(ARG_TYPE searchValue, POSITION startAfter) const {
    CNode* pNode = (startAfter.pNode != nullptr) ? startAfter.pNode->pNext : m_pNodeHead;
    while (pNode != nullptr) {
        if (pNode->data == searchValue)
            return POSITION(pNode);
        pNode = pNode->pNext;
    }
    return POSITION(nullptr);
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::AddHead(ARG_TYPE newElement) {
    CNode* pNewNode = NewNode(nullptr, m_pNodeHead);
    pNewNode->data = newElement;
    if (m_pNodeHead != nullptr)
        m_pNodeHead->pPrev = pNewNode;
    else
        m_pNodeTail = pNewNode;
    m_pNodeHead = pNewNode;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::AddTail(ARG_TYPE newElement) {
    CNode* pNewNode = NewNode(m_pNodeTail, nullptr);
    pNewNode->data = newElement;
    if (m_pNodeTail != nullptr)
        m_pNodeTail->pNext = pNewNode;
    else
        m_pNodeHead = pNewNode;
    m_pNodeTail = pNewNode;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::AddHead(CList* pNewList) {
    POSITION pos = pNewList->GetTailPosition();
    while (pos != nullptr)
        AddHead(pNewList->GetPrev(pos));
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::AddTail(CList* pNewList) {
    POSITION pos = pNewList->GetHeadPosition();
    while (pos != nullptr)
        AddTail(pNewList->GetNext(pos));
}

template<class TYPE, class ARG_TYPE>
TYPE CList<TYPE, ARG_TYPE>::RemoveHead() {
    CNode* pOldNode = m_pNodeHead;
    TYPE returnValue = pOldNode->data;

    m_pNodeHead = pOldNode->pNext;
    if (m_pNodeHead != nullptr)
        m_pNodeHead->pPrev = nullptr;
    else
        m_pNodeTail = nullptr;

    pOldNode->data.~TYPE();
    pOldNode->pNext = m_pNodeFree;
    m_pNodeFree = pOldNode;
    m_nCount--;

    return returnValue;
}

template<class TYPE, class ARG_TYPE>
TYPE CList<TYPE, ARG_TYPE>::RemoveTail() {
    CNode* pOldNode = m_pNodeTail;
    TYPE returnValue = pOldNode->data;

    m_pNodeTail = pOldNode->pPrev;
    if (m_pNodeTail != nullptr)
        m_pNodeTail->pNext = nullptr;
    else
        m_pNodeHead = nullptr;

    pOldNode->data.~TYPE();
    pOldNode->pNext = m_pNodeFree;
    m_pNodeFree = pOldNode;
    m_nCount--;

    return returnValue;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::InsertBefore(POSITION position, ARG_TYPE newElement) {
    if (position.pNode == nullptr) {
        AddTail(newElement);
        return;
    }

    CNode* pOldNode = position.pNode;
    CNode* pNewNode = NewNode(pOldNode->pPrev, pOldNode);
    pNewNode->data = newElement;

    if (pOldNode->pPrev != nullptr)
        pOldNode->pPrev->pNext = pNewNode;
    else
        m_pNodeHead = pNewNode;
    pOldNode->pPrev = pNewNode;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::InsertAfter(POSITION position, ARG_TYPE newElement) {
    if (position.pNode == nullptr) {
        AddHead(newElement);
        return;
    }

    CNode* pOldNode = position.pNode;
    CNode* pNewNode = NewNode(pOldNode, pOldNode->pNext);
    pNewNode->data = newElement;

    if (pOldNode->pNext != nullptr)
        pOldNode->pNext->pPrev = pNewNode;
    else
        m_pNodeTail = pNewNode;
    pOldNode->pNext = pNewNode;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::RemoveAll() {
    CNode* pNode = m_pNodeHead;
    while (pNode != nullptr) {
        CNode* pNext = pNode->pNext;
        pNode->data.~TYPE();
        pNode->pNext = m_pNodeFree;
        m_pNodeFree = pNode;
        pNode = pNext;
    }
    m_pNodeHead = m_pNodeTail = nullptr;
    m_nCount = 0;
}

// ============================================================================
// CMap template implementation
// ============================================================================

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::CMap(int nBlockSize)
    : m_pHashTable(nullptr), m_nHashTableSize(17), m_nCount(0),
      m_pFreeList(nullptr), m_pBlocks(nullptr), m_nBlockSize(nBlockSize) {}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::~CMap() {
    RemoveAll();
    if (m_pHashTable != nullptr)
        free(m_pHashTable);
    while (m_pBlocks != nullptr) {
        CBlock* pNext = m_pBlocks->pNext;
        free(m_pBlocks);
        m_pBlocks = pNext;
    }
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::InitHashTable(UINT hashSize, bool bAllocNow) {
    RemoveAll();
    if (m_pHashTable != nullptr) {
        free(m_pHashTable);
        m_pHashTable = nullptr;
    }
    m_nHashTableSize = hashSize;
    if (bAllocNow) {
        m_pHashTable = (CAssoc**)calloc(hashSize, sizeof(CAssoc*));
    }
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
UINT CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::HashKey(ARG_KEY key) const {
    // Default hash for integral types
    return ((UINT)(uintptr_t)key) >> 4;
}

// Specialization for CString keys
template<>
inline UINT CMap<CString, const CString&, CObject*, CObject*>::HashKey(const CString& key) const {
    UINT hash = 0;
    const wchar_t* p = (const wchar_t*)key;
    while (*p) {
        hash = (hash << 5) + hash + *p++;
    }
    return hash;
}

template<>
inline UINT CMap<CString, const CString&, void*, void*>::HashKey(const CString& key) const {
    UINT hash = 0;
    const wchar_t* p = (const wchar_t*)key;
    while (*p) {
        hash = (hash << 5) + hash + *p++;
    }
    return hash;
}

template<>
inline UINT CMap<CString, const CString&, CString, const CString&>::HashKey(const CString& key) const {
    UINT hash = 0;
    const wchar_t* p = (const wchar_t*)key;
    while (*p) {
        hash = (hash << 5) + hash + *p++;
    }
    return hash;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
typename CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::CAssoc*
CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::NewAssoc() {
    if (m_pFreeList == nullptr) {
        // Allocate a new block
        size_t blockSize = sizeof(CBlock) + m_nBlockSize * sizeof(CAssoc);
        CBlock* pNewBlock = (CBlock*)malloc(blockSize);
        pNewBlock->pNext = m_pBlocks;
        m_pBlocks = pNewBlock;

        // Chain onto free list
        CAssoc* pAssoc = (CAssoc*)(pNewBlock + 1);
        for (int i = 0; i < m_nBlockSize - 1; i++)
            pAssoc[i].pNext = &pAssoc[i + 1];
        pAssoc[m_nBlockSize - 1].pNext = nullptr;
        m_pFreeList = pAssoc;
    }

    CAssoc* pAssoc = m_pFreeList;
    m_pFreeList = m_pFreeList->pNext;
    m_nCount++;

    new (&pAssoc->key) KEY();
    new (&pAssoc->value) VALUE();

    return pAssoc;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::FreeAssoc(CAssoc* pAssoc) {
    pAssoc->key.~KEY();
    pAssoc->value.~VALUE();
    pAssoc->pNext = m_pFreeList;
    m_pFreeList = pAssoc;
    m_nCount--;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
typename CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::CAssoc*
CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetAssocAt(ARG_KEY key, UINT& nHash, UINT& nHashBucket) const {
    nHash = HashKey(key);
    nHashBucket = nHash % m_nHashTableSize;

    if (m_pHashTable == nullptr)
        return nullptr;

    for (CAssoc* pAssoc = m_pHashTable[nHashBucket]; pAssoc != nullptr; pAssoc = pAssoc->pNext) {
        if (pAssoc->key == key)
            return pAssoc;
    }
    return nullptr;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
bool CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::Lookup(ARG_KEY key, VALUE& rValue) const {
    UINT nHash, nHashBucket;
    CAssoc* pAssoc = GetAssocAt(key, nHash, nHashBucket);
    if (pAssoc == nullptr)
        return false;
    rValue = pAssoc->value;
    return true;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
bool CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::LookupKey(ARG_KEY key, KEY& rKey) const {
    UINT nHash, nHashBucket;
    CAssoc* pAssoc = GetAssocAt(key, nHash, nHashBucket);
    if (pAssoc == nullptr)
        return false;
    rKey = pAssoc->key;
    return true;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
VALUE& CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::operator[](ARG_KEY key) {
    UINT nHash, nHashBucket;
    CAssoc* pAssoc = GetAssocAt(key, nHash, nHashBucket);

    if (pAssoc == nullptr) {
        if (m_pHashTable == nullptr)
            InitHashTable(m_nHashTableSize);

        pAssoc = NewAssoc();
        pAssoc->nHashValue = nHash;
        pAssoc->key = key;
        pAssoc->pNext = m_pHashTable[nHashBucket];
        m_pHashTable[nHashBucket] = pAssoc;
    }
    return pAssoc->value;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
const VALUE& CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::operator[](ARG_KEY key) const {
    UINT nHash, nHashBucket;
    CAssoc* pAssoc = GetAssocAt(key, nHash, nHashBucket);
    // Note: This will crash if key not found - matches MFC behavior
    return pAssoc->value;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::SetAt(ARG_KEY key, ARG_VALUE newValue) {
    (*this)[key] = newValue;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
bool CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::RemoveKey(ARG_KEY key) {
    if (m_pHashTable == nullptr)
        return false;

    UINT nHashBucket = HashKey(key) % m_nHashTableSize;
    CAssoc** ppAssocPrev = &m_pHashTable[nHashBucket];

    for (CAssoc* pAssoc = *ppAssocPrev; pAssoc != nullptr; pAssoc = pAssoc->pNext) {
        if (pAssoc->key == key) {
            *ppAssocPrev = pAssoc->pNext;
            FreeAssoc(pAssoc);
            return true;
        }
        ppAssocPrev = &pAssoc->pNext;
    }
    return false;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::RemoveAll() {
    if (m_pHashTable != nullptr) {
        for (UINT i = 0; i < m_nHashTableSize; i++) {
            for (CAssoc* pAssoc = m_pHashTable[i]; pAssoc != nullptr;) {
                CAssoc* pNext = pAssoc->pNext;
                FreeAssoc(pAssoc);
                pAssoc = pNext;
            }
            m_pHashTable[i] = nullptr;
        }
    }
    m_nCount = 0;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
typename CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::POSITION
CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetStartPosition() const {
    if (m_nCount == 0 || m_pHashTable == nullptr)
        return POSITION(nullptr, 0);

    for (UINT i = 0; i < m_nHashTableSize; i++) {
        if (m_pHashTable[i] != nullptr)
            return POSITION(m_pHashTable[i], i);
    }
    return POSITION(nullptr, 0);
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetNextAssoc(POSITION& rNextPosition, KEY& rKey, VALUE& rValue) const {
    CAssoc* pAssoc = rNextPosition.pAssoc;
    rKey = pAssoc->key;
    rValue = pAssoc->value;

    // Advance position
    if (pAssoc->pNext != nullptr) {
        rNextPosition.pAssoc = pAssoc->pNext;
    } else {
        // Find next bucket
        for (UINT i = rNextPosition.nHashBucket + 1; i < m_nHashTableSize; i++) {
            if (m_pHashTable[i] != nullptr) {
                rNextPosition.pAssoc = m_pHashTable[i];
                rNextPosition.nHashBucket = i;
                return;
            }
        }
        rNextPosition.pAssoc = nullptr;
    }
}

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
    typedef const wchar_t* BASE_KEY;
    CMapStringToOb(int nBlockSize = 10) : CMap(nBlockSize) {}

    // Additional string-specific methods
    CObject*& operator[](const wchar_t* key);
    bool Lookup(const wchar_t* key, CObject*& rValue) const;
    void SetAt(const wchar_t* key, CObject* newValue);
};

class CMapStringToPtr : public CMap<CString, const CString&, void*, void*> {
public:
    typedef const wchar_t* BASE_KEY;
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

// ============================================================================
// Typed pointer collection templates
// ============================================================================

// CTypedPtrArray - type-safe wrapper for CPtrArray/CObArray
template<class BASE_CLASS, class TYPE>
class CTypedPtrArray : public BASE_CLASS {
public:
    TYPE GetAt(int nIndex) const {
        return static_cast<TYPE>(BASE_CLASS::GetAt(nIndex));
    }
    TYPE& ElementAt(int nIndex) {
        return reinterpret_cast<TYPE&>(BASE_CLASS::ElementAt(nIndex));
    }
    void SetAt(int nIndex, TYPE ptr) {
        BASE_CLASS::SetAt(nIndex, ptr);
    }
    void SetAtGrow(int nIndex, TYPE newElement) {
        BASE_CLASS::SetAtGrow(nIndex, newElement);
    }
    int Add(TYPE newElement) {
        return BASE_CLASS::Add(newElement);
    }
    void InsertAt(int nIndex, TYPE newElement, int nCount = 1) {
        BASE_CLASS::InsertAt(nIndex, newElement, nCount);
    }
    TYPE operator[](int nIndex) const {
        return GetAt(nIndex);
    }
    TYPE& operator[](int nIndex) {
        return ElementAt(nIndex);
    }
};

// CTypedPtrList - type-safe wrapper for CPtrList/CObList
template<class BASE_CLASS, class TYPE>
class CTypedPtrList : public BASE_CLASS {
public:
    TYPE& GetHead() {
        return reinterpret_cast<TYPE&>(BASE_CLASS::GetHead());
    }
    TYPE GetHead() const {
        return static_cast<TYPE>(BASE_CLASS::GetHead());
    }
    TYPE& GetTail() {
        return reinterpret_cast<TYPE&>(BASE_CLASS::GetTail());
    }
    TYPE GetTail() const {
        return static_cast<TYPE>(BASE_CLASS::GetTail());
    }
    TYPE RemoveHead() {
        return static_cast<TYPE>(BASE_CLASS::RemoveHead());
    }
    TYPE RemoveTail() {
        return static_cast<TYPE>(BASE_CLASS::RemoveTail());
    }
    TYPE& GetNext(typename BASE_CLASS::POSITION& rPosition) {
        return reinterpret_cast<TYPE&>(BASE_CLASS::GetNext(rPosition));
    }
    TYPE GetNext(typename BASE_CLASS::POSITION& rPosition) const {
        return static_cast<TYPE>(BASE_CLASS::GetNext(rPosition));
    }
    TYPE& GetPrev(typename BASE_CLASS::POSITION& rPosition) {
        return reinterpret_cast<TYPE&>(BASE_CLASS::GetPrev(rPosition));
    }
    TYPE GetPrev(typename BASE_CLASS::POSITION& rPosition) const {
        return static_cast<TYPE>(BASE_CLASS::GetPrev(rPosition));
    }
    TYPE GetAt(typename BASE_CLASS::POSITION position) const {
        return static_cast<TYPE>(BASE_CLASS::GetAt(position));
    }
    void SetAt(typename BASE_CLASS::POSITION pos, TYPE newElement) {
        BASE_CLASS::SetAt(pos, newElement);
    }
    void AddHead(TYPE newElement) {
        BASE_CLASS::AddHead(newElement);
    }
    void AddTail(TYPE newElement) {
        BASE_CLASS::AddTail(newElement);
    }
};

// CTypedPtrMap - type-safe wrapper for CMapXxxToPtr/CMapXxxToOb
template<class BASE_CLASS, class KEY, class VALUE>
class CTypedPtrMap : public BASE_CLASS {
public:
    bool Lookup(typename BASE_CLASS::BASE_KEY key, VALUE& rValue) const {
        void* pValue;
        if (!BASE_CLASS::Lookup(key, pValue))
            return false;
        rValue = static_cast<VALUE>(pValue);
        return true;
    }
    VALUE& operator[](typename BASE_CLASS::BASE_KEY key) {
        return reinterpret_cast<VALUE&>(BASE_CLASS::operator[](key));
    }
    void SetAt(KEY key, VALUE newValue) {
        BASE_CLASS::SetAt(key, newValue);
    }
};

// ============================================================================
// CMapStringTo* inline implementations
// ============================================================================

inline CObject*& CMapStringToOb::operator[](const wchar_t* key) {
    return CMap<CString, const CString&, CObject*, CObject*>::operator[](CString(key));
}

inline bool CMapStringToOb::Lookup(const wchar_t* key, CObject*& rValue) const {
    return CMap<CString, const CString&, CObject*, CObject*>::Lookup(CString(key), rValue);
}

inline void CMapStringToOb::SetAt(const wchar_t* key, CObject* newValue) {
    CMap<CString, const CString&, CObject*, CObject*>::SetAt(CString(key), newValue);
}

inline void*& CMapStringToPtr::operator[](const wchar_t* key) {
    return CMap<CString, const CString&, void*, void*>::operator[](CString(key));
}

inline bool CMapStringToPtr::Lookup(const wchar_t* key, void*& rValue) const {
    return CMap<CString, const CString&, void*, void*>::Lookup(CString(key), rValue);
}

inline void CMapStringToPtr::SetAt(const wchar_t* key, void* newValue) {
    CMap<CString, const CString&, void*, void*>::SetAt(CString(key), newValue);
}

inline CString& CMapStringToString::operator[](const wchar_t* key) {
    return CMap<CString, const CString&, CString, const CString&>::operator[](CString(key));
}

inline bool CMapStringToString::Lookup(const wchar_t* key, CString& rValue) const {
    return CMap<CString, const CString&, CString, const CString&>::Lookup(CString(key), rValue);
}

inline void CMapStringToString::SetAt(const wchar_t* key, const CString& newValue) {
    CMap<CString, const CString&, CString, const CString&>::SetAt(CString(key), newValue);
}

inline void CMapStringToString::SetAt(const wchar_t* key, const wchar_t* newValue) {
    CMap<CString, const CString&, CString, const CString&>::SetAt(CString(key), CString(newValue));
}

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
// Note: Provide an inline definition unless we're consuming the DLL (_AFXDLL).
#ifndef COBJECT_IMPL_DEFINED
#define COBJECT_IMPL_DEFINED
#if defined(OPENMFC_EXPORTS) || !defined(_AFXDLL)
#if !defined(OPENMFC_INLINE_VAR)
#if defined(__cpp_inline_variables) || (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L)
#define OPENMFC_INLINE_VAR inline
#elif defined(_MSC_VER)
#define OPENMFC_INLINE_VAR __declspec(selectany)
#else
#define OPENMFC_INLINE_VAR __attribute__((weak))
#endif
#endif
// Order: lpszClassName, nObjectSize, wSchema, pfnCreateObject, pfnGetBaseClass, pBaseClass, pNextClass
OPENMFC_INLINE_VAR CRuntimeClass CObject::classCObject = {
    "CObject",
    sizeof(CObject),
    0xFFFF,         // m_wSchema
    nullptr,        // m_pfnCreateObject
    nullptr,        // m_pfnGetBaseClass
    nullptr,        // m_pBaseClass - CObject is root
    nullptr         // m_pNextClass
};

#endif
// CObject::Serialize default implementation (does nothing for base class)
inline void CObject::Serialize(CArchive& ar) {
    (void)ar;  // Base CObject::Serialize does nothing
}
#endif
