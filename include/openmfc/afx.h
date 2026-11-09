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
#ifdef GetFileTitle
#undef GetFileTitle
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

void AFXAPI AfxThrowInvalidArgException();

class CObject;
class CArchive;
class CArchiveAccess;
class CFile;
class CException;
class CFileException;

void AFXAPI AfxTryCleanup();
void AFXAPI AfxThrowLastCleanup();
struct AFX_EXCEPTION_LINK {
    AFX_EXCEPTION_LINK* m_pLinkPrev;
    CException* m_pException;
    AFX_EXCEPTION_LINK();
    ~AFX_EXCEPTION_LINK() {AfxTryCleanup();}
};
static_assert(sizeof(AFX_EXCEPTION_LINK)==16,"native exception handler link");

// CFileStatus - file status structure
struct CFileStatus {
    LONGLONG m_ctime;       // Native CTime seconds since the Unix epoch
    LONGLONG m_mtime;       // Last modification time in seconds
    LONGLONG m_atime;       // Last access time in seconds
    ULONGLONG m_size;       // File size
    DWORD m_attribute;      // Native file attributes
    wchar_t m_szFullName[_MAX_PATH];  // Absolute path name
};
static_assert(sizeof(CFileStatus)==560 && offsetof(CFileStatus,m_szFullName)==36, "native file status model");

// CRuntimeClass - runtime type information structure
// This structure is used for MFC's own RTTI system (separate from C++ RTTI)
// IMPORTANT: Layout must match real MFC exactly for ABI compatibility!
struct CRuntimeClass {
    // IMPORTANT: Field order must match real MFC exactly for ABI compatibility!
    const char* m_lpszClassName;      // Class name string (offset 0)
    int m_nObjectSize;                // sizeof(class) (offset 8)
    unsigned int m_wSchema;           // Schema number for serialization (offset 12)
    CObject* (AFXAPI *m_pfnCreateObject)();  // Factory function (offset 16)
    // The base link. In an _AFXDLL build -- which is what mfc140u.dll is, and what this
    // project reimplements -- retail MFC stores a FUNCTION POINTER here, not a data
    // pointer; afx.h's `#else` branch names the same slot m_pBaseClass for the static-lib
    // build, and the two never coexist. Verified against the retail binary rather than the
    // headers: CRuntimeClass::IsDerivedFrom (mfc140 RVA 0x233440) is `mov 0x18(%rax),%rax;
    // test; je fail; call *%rax` with no data fallback at all.
    CRuntimeClass* (AFXAPI *m_pfnGetBaseClass)(); // Get base class (offset 24)
    CRuntimeClass* m_pNextClass;      // Module class list link (offset 32)
    const void* m_pClassInit;         // AFX_CLASSINIT for IMPLEMENT_SERIAL (offset 40)

    // Helper methods
    CObject* CreateObject();
    OPENMFC_STRING_API void Store(CArchive& archive) const;
    CObject* CreateObject() const { return const_cast<CRuntimeClass*>(this)->CreateObject(); }

    // Convenience accessor for the base link. Returns nullptr at a root descriptor.
    // Prefer this over touching the slot directly: there is no data member holding the
    // base class any more, by design.
    CRuntimeClass* BaseClass() const {
        return m_pfnGetBaseClass ? m_pfnGetBaseClass() : nullptr;
    }

    BOOL IsDerivedFrom(const CRuntimeClass* pBaseClass) const {
        if(!this || !pBaseClass)AfxThrowInvalidArgException();
        const CRuntimeClass* current=this;
        while(current!=pBaseClass) {
            auto next=current->m_pfnGetBaseClass;
            if(!next)return FALSE;
            current=next();
        }
        return TRUE;
    }
};

// Offsets pinned against the retail binary, not against MSVC's headers. Evidence:
//   CRuntimeClass::FromName  (mfc140 RVA 0x1cf1f0) walks the module list with
//                            `mov 0x20(%rbx),%rbx`            -> m_pNextClass  == 0x20
//   AfxClassInit             (mfc140 RVA 0x2333f0) uses CSimpleList::m_nNextOffset from
//                            AFX_MODULE_STATE+0x40, which equals 0x20, same conclusion
//   CRuntimeClass::IsDerivedFrom (0x233440) `mov 0x18(%rax),%rax; test; je; call *%rax`
//                                                             -> base link   == 0x18, a pfn
//   adjacent IMPLEMENT_SERIAL descriptors (CObList 0x1803aa928, CStringList 0x1803aa958)
//   are 0x30 apart, and each holds a 1-byte AFX_CLASSINIT pointer at +0x28
// Do not "simplify" this struct: an extra data member for the base class silently takes
// m_pNextClass's slot and shifts everything after it, which is what this file used to do.
static_assert(offsetof(CRuntimeClass, m_lpszClassName)   == 0x00, "CRuntimeClass layout");
static_assert(offsetof(CRuntimeClass, m_nObjectSize)     == 0x08, "CRuntimeClass layout");
static_assert(offsetof(CRuntimeClass, m_wSchema)         == 0x0c, "CRuntimeClass layout");
static_assert(offsetof(CRuntimeClass, m_pfnCreateObject) == 0x10, "CRuntimeClass layout");
static_assert(offsetof(CRuntimeClass, m_pfnGetBaseClass) == 0x18, "CRuntimeClass layout");
static_assert(offsetof(CRuntimeClass, m_pNextClass)      == 0x20, "CRuntimeClass layout");
static_assert(offsetof(CRuntimeClass, m_pClassInit)      == 0x28, "CRuntimeClass layout");
static_assert(sizeof(CRuntimeClass) == 0x30, "CRuntimeClass size");

// DECLARE_DYNAMIC - adds runtime class support to a class
// Use in class declaration (public section)
#define DECLARE_DYNAMIC(class_name) \
public: \
    static AFX_DATA CRuntimeClass class##class_name; \
    static CRuntimeClass* AFXAPI _GetBaseClass(); \
    static CRuntimeClass* GetThisClass() { return &class##class_name; } \
    virtual CRuntimeClass* GetRuntimeClass() const override { return GetThisClass(); }

// DECLARE_DYNCREATE - adds runtime class + factory support
#define DECLARE_DYNCREATE(class_name) \
    DECLARE_DYNAMIC(class_name) \
    static CObject* AFXAPI CreateObject() { return new class_name; }

// IMPLEMENT_DYNAMIC - implements runtime class (put in .cpp file)
// Order must match CRuntimeClass layout: lpszClassName, nObjectSize, wSchema,
// pfnCreateObject, pfnGetBaseClass, pNextClass, pClassInit.
// The base class is reached through a generated _GetBaseClass thunk, exactly as retail's
// IMPLEMENT_RUNTIMECLASS does in an _AFXDLL build.
#define IMPLEMENT_DYNAMIC(class_name, base_class_name) \
    CRuntimeClass* AFXAPI class_name::_GetBaseClass() \
        { return base_class_name::GetThisClass(); } \
    CRuntimeClass class_name::class##class_name = { \
        #class_name, \
        sizeof(class_name), \
        0xFFFF, \
        nullptr, \
        &class_name::_GetBaseClass, \
        nullptr, \
        nullptr \
    };

// IMPLEMENT_DYNCREATE - implements runtime class with factory
#define IMPLEMENT_DYNCREATE(class_name, base_class_name) \
    CRuntimeClass* AFXAPI class_name::_GetBaseClass() \
        { return base_class_name::GetThisClass(); } \
    CRuntimeClass class_name::class##class_name = { \
        #class_name, \
        sizeof(class_name), \
        0xFFFF, \
        &class_name::CreateObject, \
        &class_name::_GetBaseClass, \
        nullptr, \
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
    CRuntimeClass* AFXAPI class_name::_GetBaseClass() \
        { return base_class_name::GetThisClass(); } \
    CRuntimeClass class_name::class##class_name = { \
        #class_name, \
        sizeof(class_name), \
        wSchema, \
        &class_name::CreateObject, \
        &class_name::_GetBaseClass, \
        nullptr, \
        nullptr \
    };

// RUNTIME_CLASS macro - get CRuntimeClass pointer for a class
#define RUNTIME_CLASS(class_name) (class_name::GetThisClass())

#ifndef AFX_NOVTABLE
#if defined(_MSC_VER)
#define AFX_NOVTABLE __declspec(novtable)
#else
#define AFX_NOVTABLE
#endif
#endif

// CObject - base class for most MFC classes
// IMPORTANT: Virtual method declaration order matches real MFC for vtable ABI compatibility
// Real MFC declares GetRuntimeClass() before destructor, so vtable[0] = GetRuntimeClass
class AFX_NOVTABLE CObject {
public:
    // Runtime class information - MUST be first virtual for vtable[0] compatibility
#if defined(_AFXDLL) && !defined(OPENMFC_EXPORTS) && !defined(OPENMFC_APPCORE_IMPL)
    virtual OPENMFC_STRING_API CRuntimeClass* GetRuntimeClass() const;
#else
    virtual CRuntimeClass* GetRuntimeClass() const {
        return GetThisClass();
    }
#endif

    // Virtual destructor - second in vtable
    virtual ~CObject() = 0;
    
    // Static runtime class access
#if defined(_AFXDLL) && !defined(OPENMFC_EXPORTS) && !defined(OPENMFC_APPCORE_IMPL)
    static OPENMFC_STRING_API CRuntimeClass* GetThisClass();
#else
    static CRuntimeClass* GetThisClass() {
        return &classCObject;
    }
#endif
    
    // Type checking
    BOOL IsKindOf(const CRuntimeClass* pClass) const {
        if(!this)AfxThrowInvalidArgException();
        CRuntimeClass* type=GetRuntimeClass();
        if(!type)AfxThrowInvalidArgException();
        return type->IsDerivedFrom(pClass);
    }
    
    // Base objects carry no serialized payload.
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

inline CObject::~CObject() = default;

// CArchive - serialization archive class
void AFXAPI AfxThrowArchiveException(int,const wchar_t*);
class CDocument;
class CPtrArray;
class CMapPtrToPtr;
template<class TYPE,class ARG_TYPE> class CArray;
#define OPENMFC_ARCHIVE_API OPENMFC_STRING_API
class CArchive {
protected:
    enum LoadArrayObjType {typeUndefined=0,typeCRuntimeClass=1,typeCObject=2};
public:
    enum Mode { store = 0, load = 1, bNoFlushOnDelete = 2, bNoByteSwap = 4 };
    
    OPENMFC_ARCHIVE_API CArchive(CFile* pFile, UINT nMode, int nBufSize = 4096, void* lpBuf = nullptr);
    OPENMFC_ARCHIVE_API ~CArchive();
    
    // Basic operations
    BOOL IsLoading() const { return (m_nMode & load) != 0; }
    BOOL IsStoring() const { return !IsLoading(); }
    CFile* GetFile() const { return m_pFile; }
    OPENMFC_ARCHIVE_API UINT GetObjectSchema();
    void SetObjectSchema(UINT nSchema) { m_nObjectSchema = nSchema; }
    
    BOOL IsByteSwapping() const {return FALSE;}
    BOOL IsBufferEmpty() const {return m_lpBufCur==m_lpBufMax;}
    CDocument* m_pDocument;

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
    CArchive& operator>>(void*& p);
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
    CArchive& operator<<(const CObject* object) {WriteObject(object);return *this;}
    CArchive& operator<<(const void* p);
    CArchive& operator<<(const CString& str);
    
    // String operations
    wchar_t* ReadString(wchar_t* lpsz, UINT nMax);
    int ReadString(CString& rString);
    void WriteString(const wchar_t* lpsz);
    
    // Raw read/write
    OPENMFC_ARCHIVE_API UINT Read(void* lpBuf, UINT nMax);
    OPENMFC_ARCHIVE_API void Write(const void* lpBuf, UINT nMax);
    
    // Flush buffer
    OPENMFC_ARCHIVE_API void Flush();
    
    // Close archive
    OPENMFC_ARCHIVE_API void Close();
    
    // Exception handling
    OPENMFC_ARCHIVE_API void Abort();

    // Native public state used by clients and serialization callbacks.
    BOOL m_bForceFlat;
    BOOL m_bDirectBuffer;
    BOOL m_bBlocking;
    UINT m_nObjectSchema;
    CString m_strFileName;
    OPENMFC_ARCHIVE_API void FillBuffer(UINT additional);
    void EnsureRead(void* output,UINT count) {if(Read(output,count)!=count)AfxThrowArchiveException(3,m_strFileName.GetString());}
    OPENMFC_ARCHIVE_API ULONGLONG ReadCount();
    OPENMFC_ARCHIVE_API void WriteCount(ULONGLONG count);
    OPENMFC_ARCHIVE_API void CheckCount();
    OPENMFC_ARCHIVE_API void MapObject(const CObject* object);
    OPENMFC_ARCHIVE_API void WriteClass(const CRuntimeClass* type);
    OPENMFC_ARCHIVE_API void WriteObject(const CObject* object);
    OPENMFC_ARCHIVE_API void EnsureSchemaMapExists(CArray<LoadArrayObjType,const LoadArrayObjType&>** types=nullptr);
    void SetStoreParams(UINT hashSize=2053,UINT blockSize=128) {m_nHashSize=hashSize;m_nGrowSize=blockSize;}
    void SetLoadParams(UINT growBy=1024) {m_nGrowSize=growBy;}
protected:
    CArchive(const CArchive&);
    void operator=(const CArchive&);
    BOOL m_nMode;
    BOOL m_bUserBuf;
    int m_nBufSize;
    CFile* m_pFile;
    BYTE* m_lpBufCur;
    BYTE* m_lpBufMax;
    BYTE* m_lpBufStart;
    UINT m_nMapCount;
    union {CPtrArray* m_pLoadArray;CMapPtrToPtr* m_pStoreMap;};
    CMapPtrToPtr* m_pSchemaMap;
    UINT m_nGrowSize;
    UINT m_nHashSize;
    friend class CArchiveAccess;
};
static_assert(sizeof(CArchive)==112 && !std::is_polymorphic_v<CArchive>,"native archive representation");


// CFile - base file class
namespace ATL { class CAtlTransactionManager; }
struct CFileExports;
struct CMemFileExports;
class CFile : public CObject {
    friend struct CFileExports;
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
        typeBinary = 0x8000,
        typeUnicode = 0x0400,
        osNoBuffer = 0x10000,
        osWriteThrough = 0x20000,
        osRandomAccess = 0x40000,
        osSequentialScan = 0x80000
    };
    
    enum SeekPosition { begin = 0, current = 1, end = 2 };
    
    static AFX_DATA const HANDLE hFileNull;

    CFile();
    CFile(ATL::CAtlTransactionManager*);
    CFile(HANDLE);
    CFile(const wchar_t*,UINT);
    CFile(const wchar_t*,UINT,ATL::CAtlTransactionManager*);
    ~CFile() override;
    static AFX_DATA CRuntimeClass classCFile;
    static CRuntimeClass* GetThisClass(){return &classCFile;}
    CRuntimeClass* GetRuntimeClass() const override{return GetThisClass();}
    virtual ULONGLONG GetPosition() const;
    virtual CString GetFileName() const;
    virtual CString GetFileTitle() const;
    virtual CString GetFilePath() const;
    virtual void SetFilePath(const wchar_t* name) {if(!name)AfxThrowInvalidArgException();m_strFileName=name;}
    virtual BOOL Open(const wchar_t*,UINT,CFileException* = nullptr);
    virtual BOOL Open(const wchar_t*,UINT,ATL::CAtlTransactionManager*,CFileException*);
    virtual CFile* Duplicate() const;
    virtual ULONGLONG Seek(LONGLONG,UINT);
    virtual void SetLength(ULONGLONG);
    virtual ULONGLONG GetLength() const;
    virtual UINT Read(void*,UINT);
    virtual void Write(const void*,UINT);
    virtual void LockRange(ULONGLONG,ULONGLONG);
    virtual void UnlockRange(ULONGLONG,ULONGLONG);
    virtual void Abort();
    virtual void Flush();
    virtual void Close();
    enum BufferCommand {bufferRead,bufferWrite,bufferCommit,bufferCheck};
    enum BufferFlags {bufferDirect=1,bufferBlocking=2};
    virtual UINT GetBufferPtr(UINT,UINT=0,void** =nullptr,void** =nullptr);
    static void Rename(const wchar_t*,const wchar_t*,ATL::CAtlTransactionManager* = nullptr);
    static void Remove(const wchar_t*,ATL::CAtlTransactionManager* = nullptr);
    int GetStatus(CFileStatus&) const;
    static int GetStatus(const wchar_t*,CFileStatus&,ATL::CAtlTransactionManager* = nullptr);
    static void SetStatus(const wchar_t*,const CFileStatus&,ATL::CAtlTransactionManager* = nullptr);
    HANDLE m_hFile;
    BOOL m_bCloseOnDelete;
    CString m_strFileName;
    ATL::CAtlTransactionManager* m_pTM;
protected:
    void CommonBaseInit(HANDLE,ATL::CAtlTransactionManager*);
    void CommonInit(const wchar_t*,UINT,void* = nullptr);
};
static_assert(sizeof(CFile)==40 && offsetof(CFile,m_hFile)==8 && offsetof(CFile,m_bCloseOnDelete)==16 &&
              offsetof(CFile,m_strFileName)==24 && offsetof(CFile,m_pTM)==32,"native file model");

class CMemFile : public CFile {
    friend struct CMemFileExports;
public:
    CMemFile(UINT nGrowBytes=1024);
    CMemFile(BYTE*,UINT,UINT=0);
    ~CMemFile() override;
    static AFX_DATA CRuntimeClass classCMemFile;
    static CRuntimeClass* GetThisClass(){return &classCMemFile;}
    CRuntimeClass* GetRuntimeClass() const override{return GetThisClass();}
    ULONGLONG GetPosition() const override;
    CFile* Duplicate() const override;
    ULONGLONG Seek(LONGLONG,UINT) override;
    void SetLength(ULONGLONG) override;
    ULONGLONG GetLength() const override;
    UINT Read(void*,UINT) override;
    void Write(const void*,UINT) override;
    void LockRange(ULONGLONG,ULONGLONG) override;
    void UnlockRange(ULONGLONG,ULONGLONG) override;
    void Abort() override;
    void Flush() override;
    void Close() override;
    UINT GetBufferPtr(UINT,UINT=0,void** =nullptr,void** =nullptr) override;
    int GetStatus(CFileStatus&) const;
    BYTE* Detach();
    void Attach(BYTE*,UINT,UINT=0);
protected:
    virtual BYTE* Alloc(SIZE_T);
    virtual BYTE* Realloc(BYTE*,SIZE_T);
    virtual BYTE* Memcpy(BYTE*,const BYTE*,SIZE_T);
    virtual void Free(BYTE*);
    virtual void GrowFile(SIZE_T);
    SIZE_T m_nGrowBytes;
    ULONGLONG m_nPosition;
    SIZE_T m_nBufferSize;
    ULONGLONG m_nFileSize;
    BYTE* m_lpBuffer;
    BOOL m_bAutoDelete;
};
static_assert(sizeof(CMemFile)==88,"native memory file model");

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
    wchar_t* ReadString(wchar_t* lpsz, UINT nMax);
    int ReadString(CString& rString);
    void WriteString(const wchar_t* lpsz);
    
protected:
    FILE* m_pStream;
};



// Template collection classes
template<class TYPE, class ARG_TYPE = const TYPE&>
class CArray : public CObject {
    friend class CPtrArray;
protected:
    TYPE* m_pData;
    INT_PTR m_nSize;
    INT_PTR m_nMaxSize;
    INT_PTR m_nGrowBy;
public:
    CArray();
    ~CArray() override;
    INT_PTR GetSize() const {return m_nSize;}
    INT_PTR GetCount() const {return m_nSize;}
    BOOL IsEmpty() const {return m_nSize==0;}
    INT_PTR GetUpperBound() const {return m_nSize-1;}
    void SetSize(INT_PTR size,INT_PTR grow=-1);
    void FreeExtra();
    void RemoveAll() {SetSize(0,-1);}
    TYPE& GetAt(INT_PTR index) {if(index<0||index>=m_nSize)AfxThrowInvalidArgException();return m_pData[index];}
    const TYPE& GetAt(INT_PTR index) const {if(index<0||index>=m_nSize)AfxThrowInvalidArgException();return m_pData[index];}
    void SetAt(INT_PTR index,ARG_TYPE value) {if(index<0||index>=m_nSize)AfxThrowInvalidArgException();m_pData[index]=value;}
    TYPE& ElementAt(INT_PTR index) {return GetAt(index);}
    const TYPE& ElementAt(INT_PTR index) const {return GetAt(index);}
    TYPE& operator[](INT_PTR index) {return GetAt(index);}
    const TYPE& operator[](INT_PTR index) const {return GetAt(index);}
    TYPE* GetData() {return m_pData;}
    const TYPE* GetData() const {return m_pData;}
    void SetAtGrow(INT_PTR index,ARG_TYPE value);
    INT_PTR Add(ARG_TYPE value) {INT_PTR index=m_nSize;SetAtGrow(index,value);return index;}
    INT_PTR Append(const CArray& source);
    void Copy(const CArray& source);
    void InsertAt(INT_PTR index,ARG_TYPE value,INT_PTR count=1);
    void RemoveAt(INT_PTR index,INT_PTR count=1);
    void InsertAt(INT_PTR index,CArray* source);
    void Serialize(CArchive& ar) override;
private:
    CArray(const CArray&) = delete;
    CArray& operator=(const CArray&) = delete;
    static size_t Bytes(INT_PTR count) {return size_t(count)*sizeof(TYPE);}
    static INT_PTR Sum(INT_PTR a,INT_PTR b) {return INT_PTR(UINT_PTR(a)+UINT_PTR(b));}
    static TYPE* Allocate(INT_PTR count) {return reinterpret_cast<TYPE*>(new BYTE[Bytes(count)]);}
    static void Free(TYPE* data) {::operator delete[](data);}
    static void Construct(TYPE* data,INT_PTR count) {memset(data,0,Bytes(count));for(INT_PTR at=0;at<count;++at)::new(static_cast<void*>(data+at)) TYPE;}
    static void Destroy(TYPE* data,INT_PTR count) {for(INT_PTR at=0;at<count;++at)data[at].~TYPE();}
    static void Relocate(TYPE* out,INT_PTR capacity,const TYPE* from,INT_PTR count) {
        if(memcpy_s(out,Bytes(capacity),from,Bytes(count)))AfxThrowInvalidArgException();
    }
};
static_assert(sizeof(CArray<int>)==40,"native array representation");

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
void SerializeElements(CArchive& ar,TYPE* elements,INT_PTR count) {
    if(count && !elements)AfxThrowInvalidArgException();
    const UINT maximum=UINT(INT32_MAX/sizeof(TYPE));
    while(count) {
        UINT block=UINT_PTR(count)>maximum?maximum:UINT(count);UINT bytes=block*UINT(sizeof(TYPE));
        if(ar.IsStoring())ar.Write(elements,bytes);
        else if(ar.Read(elements,bytes)!=bytes)AfxThrowArchiveException(3,nullptr);
        elements+=block;count-=block;
    }
}

// Specialization for CString
template<>
inline void SerializeElements<CString>(CArchive& ar, CString* pElements, INT_PTR nCount) {
    if (ar.IsStoring()) {
        for (INT_PTR i = 0; i < nCount; i++) {
            ar << pElements[i];
        }
    } else {
        for (INT_PTR i = 0; i < nCount; i++) {
            ar >> pElements[i];
        }
    }
}

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
// CArray template implementation: native storage and byte relocation.
// ============================================================================
template<class T,class A>CArray<T,A>::CArray():m_pData(nullptr),m_nSize(0),m_nMaxSize(0),m_nGrowBy(0) {}
template<class T,class A>CArray<T,A>::~CArray() {if(m_pData){Destroy(m_pData,m_nSize);Free(m_pData);}}
template<class T,class A>void CArray<T,A>::SetSize(INT_PTR size,INT_PTR grow) {
    if(size<0)AfxThrowInvalidArgException();
    if(grow>=0)m_nGrowBy=grow;
    if(!size) {
        if(m_pData){Destroy(m_pData,m_nSize);Free(m_pData);m_pData=nullptr;}
        m_nSize=m_nMaxSize=0;return;
    }
    if(!m_pData) {
        INT_PTR capacity=size>m_nGrowBy?size:m_nGrowBy;
        m_pData=Allocate(capacity);memset(m_pData,0,Bytes(capacity));
        for(INT_PTR at=0;at<size;++at)::new(static_cast<void*>(m_pData+at)) T;
        m_nSize=size;m_nMaxSize=capacity;return;
    }
    if(size<=m_nMaxSize) {
        if(size>m_nSize)Construct(m_pData+m_nSize,size-m_nSize);
        else if(size<m_nSize)Destroy(m_pData+size,m_nSize-size);
        m_nSize=size;return;
    }
    INT_PTR extra=m_nGrowBy;
    if(!extra){extra=m_nSize/8;if(extra<4)extra=4;if(extra>1024)extra=1024;}
    INT_PTR capacity=Sum(m_nMaxSize,extra);if(capacity<size)capacity=size;
    T* data=Allocate(capacity);Relocate(data,capacity,m_pData,m_nSize);
    Construct(data+m_nSize,size-m_nSize);
    Free(m_pData);m_pData=data;m_nSize=size;m_nMaxSize=capacity;
}
template<class T,class A>void CArray<T,A>::FreeExtra() {
    if(m_nSize==m_nMaxSize)return;
    T* data=nullptr;if(m_nSize){data=Allocate(m_nSize);Relocate(data,m_nSize,m_pData,m_nSize);}
    Free(m_pData);m_pData=data;m_nMaxSize=m_nSize;
}
template<class T,class A>void CArray<T,A>::SetAtGrow(INT_PTR index,A value) {
    if(index<0)AfxThrowInvalidArgException();
    if(index>=m_nSize)SetSize(Sum(index,1));
    m_pData[index]=value;
}
template<class T,class A>INT_PTR CArray<T,A>::Append(const CArray& source) {
    if(this==&source)AfxThrowInvalidArgException();
    INT_PTR old=m_nSize;SetSize(Sum(old,source.m_nSize));
    for(INT_PTR at=0;at<source.m_nSize;++at)m_pData[old+at]=source.m_pData[at];
    return old;
}
template<class T,class A>void CArray<T,A>::Copy(const CArray& source) {
    if(this==&source)return;
    SetSize(source.m_nSize);
    for(INT_PTR at=0;at<source.m_nSize;++at)m_pData[at]=source.m_pData[at];
}
template<class T,class A>void CArray<T,A>::InsertAt(INT_PTR index,A value,INT_PTR count) {
    if(index<0||count<=0)AfxThrowInvalidArgException();
    if(index>=m_nSize)SetSize(Sum(index,count));
    else {
        INT_PTR old=m_nSize;SetSize(Sum(m_nSize,count));
        Destroy(m_pData+old,count);
        if(memmove_s(m_pData+index+count,Bytes(old-index),m_pData+index,Bytes(old-index)))AfxThrowInvalidArgException();
        Construct(m_pData+index,count);
    }
    for(INT_PTR at=0;at<count;++at)m_pData[index+at]=value;
}
template<class T,class A>void CArray<T,A>::RemoveAt(INT_PTR index,INT_PTR count) {
    INT_PTR end=Sum(index,count);
    if(index<0||count<0||end>m_nSize||end<index)AfxThrowInvalidArgException();
    Destroy(m_pData+index,count);
    INT_PTR remaining=m_nSize-end;
    if(remaining && memmove_s(m_pData+index,Bytes(remaining),m_pData+end,Bytes(remaining)))AfxThrowInvalidArgException();
    m_nSize-=count;
}
template<class T,class A>void CArray<T,A>::InsertAt(INT_PTR index,CArray* source) {
    if(!source||index<0)AfxThrowInvalidArgException();
    if(source->GetSize()>0){InsertAt(index,source->GetAt(0),source->GetSize());for(INT_PTR at=0;at<source->GetSize();++at)SetAt(index+at,source->GetAt(at));}
}
template<class T,class A>void CArray<T,A>::Serialize(CArchive& ar) {
    if(ar.IsStoring())ar.WriteCount(m_nSize);else SetSize(INT_PTR(ar.ReadCount()));
    SerializeElements(ar,m_pData,m_nSize);
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
typedef CArray<WORD, WORD> CWordArray;

typedef CList<int, int> CIntList;
typedef CList<unsigned int, unsigned int> CUIntList;
typedef CList<WORD, WORD> CWordList;
typedef CList<DWORD, DWORD> CDWordList;
typedef CList<void*, void*> CPtrList;

#define OPENMFC_DECLARE_ARRAY_WRAPPER(class_name, element_type, arg_type, class_decl) \
class class_name : public CObject { \
    class_decl(class_name) \
public: \
    class_name(); \
    virtual ~class_name(); \
    INT_PTR GetSize() const; \
    INT_PTR GetCount() const; \
    BOOL IsEmpty() const; \
    INT_PTR GetUpperBound() const; \
    void SetSize(INT_PTR nNewSize, INT_PTR nGrowBy = -1); \
    void FreeExtra(); \
    void RemoveAll(); \
    element_type GetAt(INT_PTR nIndex) const; \
    void SetAt(INT_PTR nIndex, arg_type newElement); \
    element_type& ElementAt(INT_PTR nIndex); \
    const element_type& ElementAt(INT_PTR nIndex) const; \
    element_type operator[](INT_PTR nIndex) const; \
    element_type& operator[](INT_PTR nIndex); \
    element_type* GetData(); \
    const element_type* GetData() const; \
    void SetAtGrow(INT_PTR nIndex, arg_type newElement); \
    INT_PTR Add(arg_type newElement); \
    INT_PTR Append(const class_name& src); \
    void Copy(const class_name& src); \
    void InsertAt(INT_PTR nIndex, arg_type newElement, INT_PTR nCount = 1); \
    void RemoveAt(INT_PTR nIndex, INT_PTR nCount = 1); \
    void InsertAt(INT_PTR nStartIndex, class_name* pNewArray); \
    virtual void Serialize(CArchive& ar) override; \
};

#define OPENMFC_DECLARE_NATIVE_ARRAY_WRAPPER(class_name, element_type, arg_type, class_decl) \
class class_name : public CObject { \
    class_decl(class_name) \
public: \
    using Element=element_type; \
    class_name(); \
    virtual ~class_name(); \
    INT_PTR GetSize() const; \
    INT_PTR GetCount() const; \
    BOOL IsEmpty() const; \
    INT_PTR GetUpperBound() const; \
    void SetSize(INT_PTR nNewSize, INT_PTR nGrowBy = -1); \
    void FreeExtra(); \
    void RemoveAll(); \
    element_type GetAt(INT_PTR nIndex) const; \
    void SetAt(INT_PTR nIndex, arg_type newElement); \
    element_type& ElementAt(INT_PTR nIndex); \
    const Element& ElementAt(INT_PTR nIndex) const; \
    element_type operator[](INT_PTR nIndex) const; \
    element_type& operator[](INT_PTR nIndex); \
    element_type* GetData(); \
    const element_type* GetData() const; \
    void SetAtGrow(INT_PTR nIndex, arg_type newElement); \
    INT_PTR Add(arg_type newElement); \
    INT_PTR Append(const class_name& src); \
    void Copy(const class_name& src); \
    void InsertAt(INT_PTR nIndex, arg_type newElement, INT_PTR nCount = 1); \
    void RemoveAt(INT_PTR nIndex, INT_PTR nCount = 1); \
    void InsertAt(INT_PTR nStartIndex, class_name* pNewArray); \
    virtual void Serialize(CArchive& ar) override; \
    element_type* m_pData; \
    INT_PTR m_nSize, m_nMaxSize, m_nGrowBy; \
};

#define OPENMFC_DECLARE_LIST_WRAPPER(class_name, element_type, arg_type, class_decl) \
class class_name : public CObject { \
    class_decl(class_name) \
public: \
    typedef CList<element_type, arg_type>::POSITION POSITION; \
    explicit class_name(INT_PTR nBlockSize = 10); \
    virtual ~class_name(); \
    INT_PTR GetCount() const; \
    BOOL IsEmpty() const; \
    element_type& GetHead(); \
    element_type GetHead() const; \
    element_type& GetTail(); \
    element_type GetTail() const; \
    POSITION GetHeadPosition() const; \
    POSITION GetTailPosition() const; \
    element_type& GetNext(POSITION& rPosition); \
    element_type GetNext(POSITION& rPosition) const; \
    element_type& GetPrev(POSITION& rPosition); \
    element_type GetPrev(POSITION& rPosition) const; \
    element_type GetAt(POSITION position) const; \
    void SetAt(POSITION pos, arg_type newElement); \
    void RemoveAt(POSITION position); \
    POSITION FindIndex(INT_PTR nIndex) const; \
    POSITION Find(arg_type searchValue, POSITION startAfter = POSITION(nullptr)) const; \
    POSITION AddHead(arg_type newElement); \
    POSITION AddTail(arg_type newElement); \
    void AddHead(class_name* pNewList); \
    void AddTail(class_name* pNewList); \
    element_type RemoveHead(); \
    element_type RemoveTail(); \
    POSITION InsertBefore(POSITION position, arg_type newElement); \
    POSITION InsertAfter(POSITION position, arg_type newElement); \
    void RemoveAll(); \
    virtual void Serialize(CArchive& ar) override; \
protected: \
    /* Retail member block, harvested with /d1reportSingleClassLayoutCObList.  */ \
    /* MFC's list classes all share this shape: sizeof 56, CObject at 0..7.    */ \
    /* OpenMFC keeps the live list contents in a side table keyed by `this`,   */ \
    /* so these are storage for binary compatibility rather than the working   */ \
    /* representation -- but they must exist, or anything embedding a list by  */ \
    /* value (CMFCToolBar has three) lays out 48 bytes short per list.         */ \
    void*    m_pNodeHead;   /* 0x08 */ \
    void*    m_pNodeTail;   /* 0x10 */ \
    INT_PTR  m_nCount;      /* 0x18 */ \
    void*    m_pNodeFree;   /* 0x20 */ \
    struct CPlex* m_pBlocks;/* 0x28 */ \
    INT_PTR  m_nBlockSize;  /* 0x30 */ \
    void SyncNativeList(); \
};

#define OPENMFC_DECLARE_MAP_WRAPPER(class_name, key_type, arg_key_type, value_type, arg_value_type, base_key_type, class_decl) \
class class_name : public CObject { \
    class_decl(class_name) \
public: \
    typedef base_key_type BASE_KEY; \
    typedef CMap<key_type, arg_key_type, value_type, arg_value_type>::POSITION POSITION; \
    explicit class_name(INT_PTR nBlockSize = 10); \
    virtual ~class_name(); \
    INT_PTR GetCount() const; \
    BOOL IsEmpty() const; \
    BOOL Lookup(arg_key_type key, value_type& rValue) const; \
    value_type& operator[](arg_key_type key); \
    const value_type& operator[](arg_key_type key) const; \
    void SetAt(arg_key_type key, arg_value_type newValue); \
    BOOL RemoveKey(arg_key_type key); \
    void RemoveAll(); \
    POSITION GetStartPosition() const; \
    void GetNextAssoc(POSITION& rNextPosition, key_type& rKey, value_type& rValue) const; \
    UINT GetHashTableSize() const; \
    void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE); \
    virtual void Serialize(CArchive& ar) override; \
};

OPENMFC_DECLARE_ARRAY_WRAPPER(CUIntArray, unsigned int, unsigned int, DECLARE_DYNAMIC)
OPENMFC_DECLARE_ARRAY_WRAPPER(CDWordArray, DWORD, DWORD, DECLARE_SERIAL)
OPENMFC_DECLARE_NATIVE_ARRAY_WRAPPER(CObArray, CObject*, CObject*, DECLARE_SERIAL)
class CPtrArray : public CObject {
#if defined(_AFXDLL) && !defined(OPENMFC_EXPORTS) && !defined(OPENMFC_APPCORE_IMPL)
public:
    static OPENMFC_ARCHIVE_API CRuntimeClass* GetThisClass();
    virtual OPENMFC_ARCHIVE_API CRuntimeClass* GetRuntimeClass() const override;
#else
    DECLARE_DYNAMIC(CPtrArray)
#endif
public:
    OPENMFC_ARCHIVE_API CPtrArray();
    OPENMFC_ARCHIVE_API virtual ~CPtrArray();
    INT_PTR GetSize() const {return m_nSize;}
    INT_PTR GetCount() const {return m_nSize;}
    BOOL IsEmpty() const {return m_nSize==0;}
    INT_PTR GetUpperBound() const {return m_nSize-1;}
    OPENMFC_ARCHIVE_API void SetSize(INT_PTR nNewSize, INT_PTR nGrowBy = -1);
    OPENMFC_ARCHIVE_API void FreeExtra();
    void RemoveAll() {SetSize(0,-1);}
    void* GetAt(INT_PTR index) const {if(index<0||index>=m_nSize)AfxThrowInvalidArgException();return m_pData[index];}
    void SetAt(INT_PTR index,void* value) {if(index<0||index>=m_nSize)AfxThrowInvalidArgException();m_pData[index]=value;}
    void*& ElementAt(INT_PTR index) {if(index<0||index>=m_nSize)AfxThrowInvalidArgException();return m_pData[index];}
    void* const& ElementAt(INT_PTR index) const {if(index<0||index>=m_nSize)AfxThrowInvalidArgException();return m_pData[index];}
    void* operator[](INT_PTR index) const {return GetAt(index);}
    void*& operator[](INT_PTR index) {return ElementAt(index);}
    void** GetData() {return m_pData;}
    const void** GetData() const {return const_cast<const void**>(m_pData);}
    OPENMFC_ARCHIVE_API void SetAtGrow(INT_PTR nIndex, void* newElement);
    INT_PTR Add(void* value) {INT_PTR index=m_nSize;SetAtGrow(index,value);return index;}
    OPENMFC_ARCHIVE_API INT_PTR Append(const CPtrArray& src);
    OPENMFC_ARCHIVE_API void Copy(const CPtrArray& src);
    OPENMFC_ARCHIVE_API void InsertAt(INT_PTR nIndex, void* newElement, INT_PTR nCount = 1);
    OPENMFC_ARCHIVE_API void RemoveAt(INT_PTR nIndex, INT_PTR nCount = 1);
    OPENMFC_ARCHIVE_API void InsertAt(INT_PTR nStartIndex, CPtrArray* pNewArray);
protected:
    void** m_pData;
    INT_PTR m_nSize;
    INT_PTR m_nMaxSize;
    INT_PTR m_nGrowBy;
private:
    CPtrArray(const CPtrArray&) = delete;
    CPtrArray& operator=(const CPtrArray&) = delete;
};
OPENMFC_DECLARE_NATIVE_ARRAY_WRAPPER(CByteArray, BYTE, BYTE, DECLARE_SERIAL)

class CStringArray : public CObject {
    DECLARE_SERIAL(CStringArray)
public:
    CStringArray();
    virtual ~CStringArray();
    INT_PTR GetSize() const;
    INT_PTR GetCount() const;
    BOOL IsEmpty() const;
    INT_PTR GetUpperBound() const;
    void SetSize(INT_PTR nNewSize, INT_PTR nGrowBy = -1);
    void FreeExtra();
    void RemoveAll();
    CString GetAt(INT_PTR nIndex) const;
    void SetAt(INT_PTR nIndex, const CString& newElement);
    CString& ElementAt(INT_PTR nIndex);
    const CString& ElementAt(INT_PTR nIndex) const;
    CString operator[](INT_PTR nIndex) const;
    CString& operator[](INT_PTR nIndex);
    CString* GetData();
    const CString* GetData() const;
    void SetAtGrow(INT_PTR nIndex, const CString& newElement);
    void SetAtGrow(INT_PTR nIndex, const wchar_t* newElement);
    INT_PTR Add(const CString& newElement);
    INT_PTR Append(const CStringArray& src);
    void Copy(const CStringArray& src);
    void InsertAt(INT_PTR nIndex, const CString& newElement, INT_PTR nCount = 1);
    void InsertAt(INT_PTR nIndex, const wchar_t* newElement, INT_PTR nCount = 1);
    void RemoveAt(INT_PTR nIndex, INT_PTR nCount = 1);
    void InsertAt(INT_PTR nStartIndex, CStringArray* pNewArray);
    void InsertEmpty(INT_PTR nIndex, INT_PTR nCount);
    virtual void Serialize(CArchive& ar) override;
};

OPENMFC_DECLARE_LIST_WRAPPER(CObList, CObject*, CObject*, DECLARE_SERIAL)

class CStringList : public CObject {
    DECLARE_SERIAL(CStringList)
public:
    typedef CList<CString, const CString&>::POSITION POSITION;
    explicit CStringList(INT_PTR nBlockSize = 10);
    virtual ~CStringList();
    INT_PTR GetCount() const;
    BOOL IsEmpty() const;
    CString& GetHead();
    CString GetHead() const;
    CString& GetTail();
    CString GetTail() const;
    POSITION GetHeadPosition() const;
    POSITION GetTailPosition() const;
    CString& GetNext(POSITION& rPosition);
    CString GetNext(POSITION& rPosition) const;
    CString& GetPrev(POSITION& rPosition);
    CString GetPrev(POSITION& rPosition) const;
    CString GetAt(POSITION position) const;
    void SetAt(POSITION pos, const CString& newElement);
    void RemoveAt(POSITION position);
    POSITION FindIndex(INT_PTR nIndex) const;
    POSITION Find(const CString& searchValue, POSITION startAfter = POSITION(nullptr)) const;
    POSITION Find(const wchar_t* searchValue, POSITION startAfter = POSITION(nullptr)) const;
    POSITION AddHead(const CString& newElement);
    POSITION AddHead(const wchar_t* newElement);
    void AddHead(CStringList* pNewList);
    POSITION AddTail(const CString& newElement);
    POSITION AddTail(const wchar_t* newElement);
    void AddTail(CStringList* pNewList);
    CString RemoveHead();
    CString RemoveTail();
    POSITION InsertBefore(POSITION position, const CString& newElement);
    POSITION InsertBefore(POSITION position, const wchar_t* newElement);
    POSITION InsertAfter(POSITION position, const CString& newElement);
    POSITION InsertAfter(POSITION position, const wchar_t* newElement);
    void RemoveAll();
    virtual void Serialize(CArchive& ar) override;
};

struct __POSITION;
class CMapPtrToPtr : public CObject {
    DECLARE_DYNAMIC(CMapPtrToPtr)
public:
    using BASE_KEY=void*;
    using POSITION=__POSITION*;
    struct CAssoc {CAssoc* pNext;void* key;void* value;};
    explicit CMapPtrToPtr(INT_PTR blockSize=10);
    ~CMapPtrToPtr() override;
    INT_PTR GetCount() const {return m_nCount;}
    INT_PTR GetSize() const {return m_nCount;}
    BOOL IsEmpty() const {return !m_nCount;}
    BOOL Lookup(void*,void*&) const;
    void*& operator[](void*);
    void* const& operator[](void* key) const {return const_cast<CMapPtrToPtr*>(this)->operator[](key);}
    void SetAt(void* key,void* value) {(*this)[key]=value;}
    BOOL RemoveKey(void*);
    void RemoveAll();
    POSITION GetStartPosition() const {return m_nCount ? reinterpret_cast<POSITION>(INT_PTR(-1)) : nullptr;}
    void GetNextAssoc(POSITION&,void*&,void*&) const;
    UINT GetHashTableSize() const {return m_nHashTableSize;}
    void InitHashTable(UINT,BOOL=TRUE);
    UINT HashKey(void*) const;
    void* GetValueAt(void* key) const {void* value=nullptr;return Lookup(key,value)?value:nullptr;}
    CAssoc* NewAssoc();
    void FreeAssoc(CAssoc*);
    CAssoc* GetAssocAt(void*,UINT& bucket,UINT& hash) const;
    CAssoc** m_pHashTable;
    UINT m_nHashTableSize;
    INT_PTR m_nCount;
    CAssoc* m_pFreeList;
    struct CPlex* m_pBlocks;
    INT_PTR m_nBlockSize;
};
static_assert(sizeof(CMapPtrToPtr)==56,"native pointer map");
OPENMFC_DECLARE_MAP_WRAPPER(CMapPtrToWord, void*, void*, WORD, WORD, void*, DECLARE_DYNAMIC)
struct __POSITION;
class CMapWordToOb : public CObject {
    DECLARE_SERIAL(CMapWordToOb)
public:
    using BASE_KEY=WORD;
    using POSITION=__POSITION*;
    struct CAssoc { CAssoc* pNext;CObject* value;WORD key; };
    explicit CMapWordToOb(INT_PTR blockSize=10);
    ~CMapWordToOb() override;
    INT_PTR GetCount() const {return m_nCount;}
    INT_PTR GetSize() const {return m_nCount;}
    BOOL IsEmpty() const {return !m_nCount;}
    BOOL Lookup(WORD,CObject*&) const;
    CObject*& operator[](WORD);
    CObject* const& operator[](WORD) const;
    void SetAt(WORD key,CObject* value) {(*this)[key]=value;}
    BOOL RemoveKey(WORD);
    void RemoveAll();
    POSITION GetStartPosition() const;
    void GetNextAssoc(POSITION&,WORD&,CObject*&) const;
    UINT GetHashTableSize() const {return m_nHashTableSize;}
    void InitHashTable(UINT,BOOL=TRUE);
    UINT HashKey(WORD key) const {return key>>4;}
    CAssoc* NewAssoc();
    void FreeAssoc(CAssoc*);
    CAssoc* GetAssocAt(WORD,UINT&,UINT&) const;
    void Serialize(CArchive&) override;
    CAssoc** m_pHashTable;
    UINT m_nHashTableSize;
    INT_PTR m_nCount;
    CAssoc* m_pFreeList;
    struct CPlex* m_pBlocks;
    INT_PTR m_nBlockSize;
};
static_assert(sizeof(CMapWordToOb)==56,"native word/object map");
OPENMFC_DECLARE_MAP_WRAPPER(CMapWordToPtr, WORD, WORD, void*, void*, WORD, DECLARE_DYNAMIC)

// String map classes (non-template versions for ABI compatibility)
class CMapStringToOb : public CObject {
    DECLARE_SERIAL(CMapStringToOb)
public:
    typedef const wchar_t* BASE_KEY;
    typedef CMap<CString, const CString&, CObject*, CObject*>::POSITION POSITION;
    explicit CMapStringToOb(INT_PTR nBlockSize = 10);
    virtual ~CMapStringToOb();

    INT_PTR GetCount() const;
    BOOL IsEmpty() const;
    BOOL Lookup(const CString& key, CObject*& rValue) const;
    BOOL Lookup(const wchar_t* key, CObject*& rValue) const;
    BOOL LookupKey(const CString& key, const wchar_t*& rKey) const;
    BOOL LookupKey(const wchar_t* key, const wchar_t*& rKey) const;
    CObject*& operator[](const CString& key);
    CObject*& operator[](const wchar_t* key);
    const CObject* operator[](const wchar_t* key) const;
    void SetAt(const CString& key, CObject* newValue);
    void SetAt(const wchar_t* key, CObject* newValue);
    BOOL RemoveKey(const CString& key);
    BOOL RemoveKey(const wchar_t* key);
    void RemoveAll();
    POSITION GetStartPosition() const;
    void GetNextAssoc(POSITION& rNextPosition, CString& rKey, CObject*& rValue) const;
    UINT GetHashTableSize() const;
    void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);
    virtual void Serialize(CArchive& ar) override;
};

class CMapStringToPtr : public CObject {
    DECLARE_DYNAMIC(CMapStringToPtr)
public:
    typedef const wchar_t* BASE_KEY;
    typedef CMap<CString, const CString&, void*, void*>::POSITION POSITION;
    explicit CMapStringToPtr(INT_PTR nBlockSize = 10);
    virtual ~CMapStringToPtr();

    INT_PTR GetCount() const;
    BOOL IsEmpty() const;
    BOOL Lookup(const CString& key, void*& rValue) const;
    BOOL Lookup(const wchar_t* key, void*& rValue) const;
    BOOL LookupKey(const CString& key, const wchar_t*& rKey) const;
    BOOL LookupKey(const wchar_t* key, const wchar_t*& rKey) const;
    void*& operator[](const CString& key);
    void*& operator[](const wchar_t* key);
    const void* operator[](const wchar_t* key) const;
    void SetAt(const CString& key, void* newValue);
    void SetAt(const wchar_t* key, void* newValue);
    BOOL RemoveKey(const CString& key);
    BOOL RemoveKey(const wchar_t* key);
    void RemoveAll();
    POSITION GetStartPosition() const;
    void GetNextAssoc(POSITION& rNextPosition, CString& rKey, void*& rValue) const;
    UINT GetHashTableSize() const;
    void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);
    virtual void Serialize(CArchive& ar) override;
};

class CMapStringToString : public CObject {
    DECLARE_SERIAL(CMapStringToString)
public:
    typedef const wchar_t* BASE_KEY;
    typedef CMap<CString, const CString&, CString, const CString&>::POSITION POSITION;
    struct CPair {
        const CPair* pNextAssoc;
        UINT nHashValue;
        CString key;
        CString value;
    };

    explicit CMapStringToString(INT_PTR nBlockSize = 10);
    virtual ~CMapStringToString();

    INT_PTR GetCount() const;
    BOOL IsEmpty() const;
    BOOL Lookup(const CString& key, CString& rValue) const;
    BOOL Lookup(const wchar_t* key, CString& rValue) const;
    BOOL LookupKey(const CString& key, const wchar_t*& rKey) const;
    BOOL LookupKey(const wchar_t* key, const wchar_t*& rKey) const;
    CString& operator[](const CString& key);
    CString& operator[](const wchar_t* key);
    const CString& operator[](const wchar_t* key) const;
    void SetAt(const CString& key, const CString& newValue);
    void SetAt(const wchar_t* key, const CString& newValue);
    void SetAt(const wchar_t* key, const wchar_t* newValue);
    BOOL RemoveKey(const CString& key);
    BOOL RemoveKey(const wchar_t* key);
    void RemoveAll();
    POSITION GetStartPosition() const;
    void GetNextAssoc(POSITION& rNextPosition, CString& rKey, CString& rValue) const;
    UINT GetHashTableSize() const;
    void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);
    CPair* PLookup(const wchar_t* key);
    const CPair* PLookup(const wchar_t* key) const;
    CPair* PGetFirstAssoc();
    const CPair* PGetFirstAssoc() const;
    CPair* PGetNextAssoc(const CPair* pAssocRet);
    const CPair* PGetNextAssoc(const CPair* pAssocRet) const;
    virtual void Serialize(CArchive& ar) override;
};

#undef OPENMFC_DECLARE_NATIVE_ARRAY_WRAPPER
#undef OPENMFC_DECLARE_ARRAY_WRAPPER
#undef OPENMFC_DECLARE_LIST_WRAPPER
#undef OPENMFC_DECLARE_MAP_WRAPPER

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

// CWinThread forward declaration - defined in afxwin.h
class CWinThread;

// Collection serialization operators
template<class TYPE, class ARG_TYPE>
CArchive& operator<<(CArchive& ar, const CArray<TYPE, ARG_TYPE>& array) {
    const_cast<CArray<TYPE, ARG_TYPE>&>(array).Serialize(ar);
    return ar;
}

template<class TYPE, class ARG_TYPE>
CArchive& operator>>(CArchive& ar, CArray<TYPE, ARG_TYPE>& array) {
    array.Serialize(ar);
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
// Order: lpszClassName, nObjectSize, wSchema, pfnCreateObject, pfnGetBaseClass, pNextClass,
// pClassInit. CObject is the root: m_pfnGetBaseClass must stay NULL, not a thunk returning
// null -- retail's IsDerivedFrom loop tests the pointer and then dereferences whatever the
// call returns, so a root that supplies a thunk would fault on the next iteration.
OPENMFC_INLINE_VAR CRuntimeClass CObject::classCObject = {
    "CObject",
    sizeof(CObject),
    0xFFFF,         // m_wSchema
    nullptr,        // m_pfnCreateObject
    nullptr,        // m_pfnGetBaseClass - CObject is root
    nullptr,        // m_pNextClass
    nullptr         // m_pClassInit
};

#endif
// CObject::Serialize default implementation (does nothing for base class)
inline void CObject::Serialize(CArchive& ar) {
    (void)ar;  // Base CObject::Serialize does nothing
}
#endif
