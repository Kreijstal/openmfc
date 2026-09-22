// CMonikerFile — OpenMFC implementation.
// Sources: retail mfc140u.dll disassembly (every RVA below is mfc140u unless
// it says otherwise); the shipping afxole.h / afxole.inl on this host
// (atlmfc/include) for member names and declaration order.
//
// =============================================================================
// Retail layout (decoded, not assumed)
//
// CMonikerFile is NOT declared in OpenMFC's public headers, so there is no
// class declaration to read the layout off; it is spelled out here and, for the
// parts they need, in three other files that must stay in step:
// core/ole/CAsyncMonikerFile.cpp (kOff_m_Moniker = 0x38, same value),
// core/ole/RuntimeClasses.cpp (OFILE_DESC(CMonikerFile, 64, ...) -- the 64 is
// sizeof, and OFILE_DESC(COleStreamFile, 56) is the 0x38 base size asserted
// below) and core/ole/CPropExchange.cpp (a 0x58 blob covering the whole
// CFile..CAsyncMonikerFile prefix).
//
// Client (MSVC) code allocates the object with the
// retail layout and constructs it with the INLINE afxole.inl constructor
// (`CMonikerFile::CMonikerFile() { }` -- there is no ??0CMonikerFile export),
// which runs the exported COleStreamFile constructor (core/ole/COleStreamFile.cpp
// placement-news OpenMFC's COleStreamFile into the client's storage), then
// stores the client's own MSVC-layout CMonikerFile vftable and zero-fills
// m_Moniker.
//
//   +0x00  vptr                       MSVC layout, written by the client ctor
//   +0x08  OpenMFC CFile members      m_hFile @+0x08, m_strFileName @+0x10
//   +0x18  OpenMFC COleStreamFile::m_lpStream   -- reached ONLY through the
//          Stream() view in detail/COleStreamFileSupport.h (retail keeps its
//          m_lpStream at +0x28; OpenMFC's ctor and every base thunk use +0x18,
//          so +0x18 is the slot that is consistent inside this DLL)
//   +0x38  IPTR(IMoniker) m_Moniker   a raw IMoniker* -- pinned below
//   sizeof == 0x40
//
// The 0x38 offset is read from the retail bodies: ??1CMonikerFile (0x260b30)
// runs the IPTR destructor on `lea 0x38(%rbx),%rcx`; Close (0x261040) does
// `add $0x38,%rcx` before the IPTR release; Attach(IMoniker*, ...) (0x260f20)
// assigns through `add $0x38,%rcx`.  ??0CAsyncMonikerFile (0x243110) zero-fills
// its own members at +0x40/+0x50/+0x58 right after the CMonikerFile base, which
// pins m_Moniker as the last base member and sizeof(CMonikerFile) == 0x40.
// OpenMFC's COleStreamFile is 0x38 bytes (static_assert below), so +0x38 lies
// exactly one slot past OpenMFC's base and inside the client's 0x40-byte object.
//
// core/ole/CAsyncMonikerFile.cpp (the derived class, owned by another file)
// forward-declares the thunks in this file and MUST use the same
// kOff_m_Moniker = 0x38.
//
// =============================================================================
// Virtual dispatch
//
// Retail makes three virtual calls on `this` inside this class: Close (vtable
// +0xb0, slot 22), Open(LPCTSTR, IBindHost*, IBindStatusCallback*, IBindCtx*,
// CFileException*) (+0xd0, slot 26) and PostBindToStream (+0xe8, slot 29).
// The object's vptr is the client's MSVC-layout vftable (see above), so the
// dispatch is made through those fixed MSVC slot indices, exactly as
// core/file/FileDispatch.cpp does for the CFile family (its 22 == Close agrees).
// The DLL must NOT use C++ virtual syntax on these objects: mingw would index
// the MSVC vtable with Itanium slot numbers.  The slot numbers are read off
// the retail CMonikerFile vftable itself (.rdata 0x32b688, mfc140u, each entry
// resolved through the export table): 21 Flush 0x260ba0, 22 Close 0x261040,
// 23 GetBufferPtr (folded 0x71e0), 24 GetStorageName 0x2651e0 (COleStreamFile's
// one added virtual), 25 Open(IMoniker*,IBindHost*,...) 0x260ee0,
// 26 Open(LPCTSTR,IBindHost*,...) 0x260be0, 27 Open(IMoniker*,CFileException*)
// 0x260e30, 28 Open(LPCTSTR,CFileException*) 0x260d80, 29 PostBindToStream
// 0x2610f0.  (MSVC lays the four Open overloads out in reverse declaration
// order, which is why 26 is the five-parameter LPCTSTR overload and not the
// one declaration-order counting would suggest; the retail call at 0x260dfb
// inside Open(LPCTSTR, CFileException*) 0x260d80 passes six arguments with the
// URL in rdx, consistent with that.)
//
// =============================================================================
// Exceptions
//
// Retail's Detach wraps Close() in try/catch(CFileException*), and the
// destructor (like ~COleStreamFile and ~CFile) wraps its Close() in
// try/catch(CException*) that reports and deletes the exception.  This DLL raises
// its MFC exceptions through _CxxThrowException (detail/MfcExceptionsSupport.h),
// which a gcc catch clause cannot see (verified note in core/ole/COleDocument.cpp),
// so that catch arm is not reproduced; see Detach.  AfxThrowInvalidArgException
// (retail's ENSURE failures) is raised through its impl__ thunk as retail does.
//
// =============================================================================
// urlmon
//
// Retail delay-loads urlmon.dll (CreateURLMoniker, RegisterBindStatusCallback,
// CreateAsyncBindCtx are delay-load slots at 0x1803e9520/28/38, resolved with
// dlyu.py).  build_phase4.sh does not link -lurlmon, so the same three entry
// points are resolved here with LoadLibraryW/GetProcAddress on first use, which
// is what a delay-load import does.  Deviation: a resolve failure returns
// HRESULT_FROM_WIN32(ERROR_MOD_NOT_FOUND / ERROR_PROC_NOT_FOUND) where the
// delay-load helper would raise an SEH exception.

#include <cstddef>
#include <cstdlib>
#include <new>
#include <windows.h>
#include <objbase.h>
#include <urlmon.h>
#include "openmfc/afxwin.h"
#include "openmfc/afxole.h"
#include "detail/COleStreamFileSupport.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Thunks this file calls, declared with the signature their mangled name
// describes.  Each definition was located in the tree before it was used:
//   impl__Close_COleStreamFile__UEAAXXZ        core/ole/COleStreamFile.cpp:21
//   impl__Attach_COleStreamFile__QEAAXPEAUIStream___Z   core/ole/Thunks.cpp:1403
//   impl__AfxThrowInvalidArgException__YAXXZ   detail/MfcExceptionsSupport.cpp:35
//   impl__OsErrorToException_CFileException__SAHJ_Z    core/exceptions/CFileException.cpp:109
extern "C" void MS_ABI impl__Close_COleStreamFile__UEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__Attach_COleStreamFile__QEAAXPEAUIStream___Z(COleStreamFile* pThis, IStream* pStream);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" int MS_ABI impl__OsErrorToException_CFileException__SAHJ_Z(long lOsError);

// This file's own thunks that other bodies here call directly (retail calls
// them directly too: Close from the destructor, the two Attach overloads from
// the Open overloads, CreateBindHost / CreateBindContext from Open).
extern "C" void MS_ABI impl__Close_CMonikerFile__UEAAXXZ(void* pThis);
extern "C" IBindHost* MS_ABI impl__CreateBindHost_CMonikerFile__KAPEAUIBindHost__XZ();
extern "C" IBindCtx* MS_ABI impl__CreateBindContext_CMonikerFile__IEAAPEAUIBindCtx__PEAVCFileException___Z(void* pThis, CFileException* pError);
extern "C" int MS_ABI impl__Attach_CMonikerFile__IEAAHPEAUIMoniker__PEAUIBindHost__PEAUIBindStatusCallback__PEAUIBindCtx__PEAVCFileException___Z(
    void* pThis, IMoniker* pMoniker, IBindHost* pBindHost, IBindStatusCallback* pBSC, IBindCtx* pBindCtx, CFileException* pError);
extern "C" int MS_ABI impl__Attach_CMonikerFile__IEAAHPEB_WPEAUIBindHost__PEAUIBindStatusCallback__PEAUIBindCtx__PEAVCFileException___Z(
    void* pThis, const wchar_t* lpszURL, IBindHost* pBindHost, IBindStatusCallback* pBSC, IBindCtx* pBindCtx, CFileException* pError);

namespace {

// ---------------------------------------------------------------------------
// Layout (see the top note for where each number was read).
constexpr long kOff_m_Moniker       = 0x38;
constexpr long kSizeof_CMonikerFile = 0x40;
static_assert(sizeof(COleStreamFile) == 0x38,
              "OpenMFC COleStreamFile must end where retail's m_Moniker starts");
static_assert(kOff_m_Moniker + static_cast<long>(sizeof(IMoniker*)) == kSizeof_CMonikerFile,
              "m_Moniker is the last CMonikerFile member");

inline IMoniker*& Moniker(void* pThis) {
    return *reinterpret_cast<IMoniker**>(static_cast<char*>(pThis) + kOff_m_Moniker);
}

// CFileException fields retail writes (m_cause +0x10, m_lOsError +0x14,
// m_strFileName +0x18 in every retail body below); OpenMFC's class has them at
// the same offsets.
static_assert(offsetof(CFileException, m_cause) == 0x10, "CFileException::m_cause");
static_assert(offsetof(CFileException, m_lOsError) == 0x14, "CFileException::m_lOsError");
static_assert(offsetof(CFileException, m_strFileName) == 0x18, "CFileException::m_strFileName");

// ---------------------------------------------------------------------------
// IPTR(T) = MFC's CIP<T, &IID_T> (afxcom_.h, not ATL) as retail compiles it,
// on a raw T* slot:
//   0x243cf4  _Release: if (p) p->Release()                 (slot untouched)
//   0x243bec  Release:  _Release, then p = NULL
//   0x243a78  ~CIP (via 0x243c20): _Release only, p left as is (locals going out of scope)
//   0x40c0    ~CIP as inlined in ??1CMonikerFile / Attach(LPCTSTR): same body
//   0x261158  operator&: _Release, p = NULL, return &p     (out-param use)
//   0x243cac  operator=(T*): if (p != q) { p = q; if (q) q->AddRef(); if (old) old->Release(); }
//   0x243b5c  CIP(T* q): p = q, no AddRef (adopts the reference)
//   0x261178  Detach-style clear: p = NULL, no Release
template <class T> inline void IptrRelease(T*& p) {
    if (p) { p->Release(); p = nullptr; }
}
template <class T> inline void IptrAssign(T*& p, T* q) {
    if (p == q) return;
    T* old = p;
    p = q;
    if (q) q->AddRef();
    if (old) old->Release();
}

// ---------------------------------------------------------------------------
// Fixed-slot virtual dispatch through the object's MSVC-layout vftable (top
// note).  Retail call instructions (each a `mov 0xNN(%rax),%rax` followed by the
// CFG-dispatched `call *__guard_dispatch_icall_fptr`): slot 22 (+0xb0) at
// 0x260e52 in Open(IMoniker*,CFileException*) 0x260e30, 0x260c00 in
// Open(LPCTSTR,IBindHost*,...) 0x260be0, 0x260f00 in Open(IMoniker*,IBindHost*,...)
// 0x260ee0, 0x261083 in Detach 0x261070, 0x26112a in PostBindToStream 0x2610f0;
// slot 26 (+0xd0) at 0x260dfb in Open(LPCTSTR,CFileException*) 0x260d80;
// slot 29 (+0xe8) at 0x26100e in Attach(IMoniker*,IBindHost*,...) 0x260f20.
inline void* const* Vtbl(void* p) { return *reinterpret_cast<void* const* const*>(p); }
inline void VClose(void* pThis) {
    typedef void (MS_ABI *Fn)(void*);
    reinterpret_cast<Fn>(Vtbl(pThis)[22])(pThis);
}
inline int VOpenUrl(void* pThis, const wchar_t* lpszURL, IBindHost* pBindHost,
                    IBindStatusCallback* pBSC, IBindCtx* pBindCtx, CFileException* pError) {
    typedef int (MS_ABI *Fn)(void*, const wchar_t*, IBindHost*, IBindStatusCallback*, IBindCtx*, CFileException*);
    return reinterpret_cast<Fn>(Vtbl(pThis)[26])(pThis, lpszURL, pBindHost, pBSC, pBindCtx, pError);
}
inline int VPostBindToStream(void* pThis, CFileException* pError) {
    typedef int (MS_ABI *Fn)(void*, CFileException*);
    return reinterpret_cast<Fn>(Vtbl(pThis)[29])(pThis, pError);
}

// ---------------------------------------------------------------------------
// _AfxFillOleFileException (0x264f74), transcribed:
//   if (FAILED(sc) && (sc & 0x1fff0000) == 0x00030000 /*FACILITY_STORAGE*/
//                  && (sc & 0xffff) < 0x100)
//       { cause = CFileException::OsErrorToException(sc & 0xffff); lOsError = sc & 0xffff; }
//   else if (sc == STG_E_INUSE (0x80030100) || sc == STG_E_SHAREREQUIRED (0x80030106))
//       { cause = sharingViolation (11); lOsError = sc; }
//   else  { cause = genericException (1); lOsError = sc; }
//   (0x80030101..0x80030105 fall into the generic arm by the `lea/cmp $4/jbe` test)
//   pError->m_cause = cause; pError->m_lOsError = lOsError;
// The same mapping is transcribed in core/ole/COleDocument.cpp; kept local
// because that copy is in its own anonymous namespace.
void FillOleFileException(CFileException* pError, HRESULT sc) {
    int cause;
    long lOsError;
    const unsigned long usc = static_cast<unsigned long>(sc);
    if (FAILED(sc) && (usc & 0x1FFF0000UL) == 0x00030000UL && (usc & 0xFFFFUL) < 0x100UL) {
        lOsError = static_cast<long>(usc & 0xFFFFUL);
        cause = impl__OsErrorToException_CFileException__SAHJ_Z(lOsError);
    } else {
        lOsError = static_cast<long>(sc);
        if (usc == 0x80030100UL || usc == 0x80030106UL) {
            cause = CFileException::sharingViolation;
        } else {
            cause = CFileException::genericException;
        }
    }
    pError->m_cause = cause;
    pError->m_lOsError = lOsError;
}

// ---------------------------------------------------------------------------
// urlmon entry points, resolved the way retail's delay-load slots are (top note).
typedef HRESULT (WINAPI *PfnCreateURLMoniker)(IMoniker*, LPCWSTR, IMoniker**);
typedef HRESULT (WINAPI *PfnRegisterBindStatusCallback)(IBindCtx*, IBindStatusCallback*, IBindStatusCallback**, DWORD);
typedef HRESULT (WINAPI *PfnCreateAsyncBindCtx)(DWORD, IBindStatusCallback*, IEnumFORMATETC*, IBindCtx**);

HMODULE UrlmonModule() {
    static HMODULE hUrlmon = nullptr;
    if (!hUrlmon) hUrlmon = ::LoadLibraryW(L"urlmon.dll");
    return hUrlmon;
}
void* ResolveUrlmon(const char* name) {
    HMODULE h = UrlmonModule();
    return h ? reinterpret_cast<void*>(::GetProcAddress(h, name)) : nullptr;
}
// ERROR_MOD_NOT_FOUND when urlmon.dll did not load, ERROR_PROC_NOT_FOUND when
// it loaded but lacks the entry point (top note).
inline HRESULT UrlmonMissing() {
    return HRESULT_FROM_WIN32(UrlmonModule() ? ERROR_PROC_NOT_FOUND : ERROR_MOD_NOT_FOUND);
}

// ---------------------------------------------------------------------------
// The bind host CreateBindHost() manufactures.  Retail: a 0x10-byte object
// (operator new(0x10) at 0x2610ab), ctor 0x2608d4 stores the vftable at .rdata
// 0x3308b8 and m_dwRef = 0 at +0x08.  The vftable (vtdump_u.py 0x3308b8) is
//   0 QueryInterface 0x260930   1 AddRef 0x96a50 (folded)   2 Release 0x260900
//   3 CreateMoniker 0x2609a0    4 MonikerBindToStorage 0x260a00
//   5 MonikerBindToObject 0xf4d0 (folded `mov $0x80004001,%eax; ret` = E_NOTIMPL)
// Scalar deleting destructor 0x260b08: dtor 0x2608ec (vptr store only), then
// operator delete(this, 0x10).
class AfxBindHost : public IBindHost {
public:
    LONG m_dwRef;                                   // +0x08

    AfxBindHost() : m_dwRef(0) {}

    // 0x260930: if (!ppv) return E_POINTER; if (riid == IID_IUnknown ||
    // riid == IID_IBindHost) { *ppv = this; InterlockedIncrement(&m_dwRef);
    // return S_OK; } return E_NOINTERFACE.  (IIDs at .rdata 0x2d9a58 / 0x2d9e98,
    // compared with a 16-byte memcmp at 0x1d37f0.)
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv) override {
        if (!ppv) return E_POINTER;
        if (IsEqualGUID(riid, IID_IUnknown) || IsEqualGUID(riid, IID_IBindHost)) {
            *ppv = static_cast<IBindHost*>(this);
            ::InterlockedIncrement(&m_dwRef);
            return S_OK;
        }
        return E_NOINTERFACE;
    }
    // 0x96a50: return InterlockedIncrement(&m_dwRef)
    ULONG STDMETHODCALLTYPE AddRef() override {
        return static_cast<ULONG>(::InterlockedIncrement(&m_dwRef));
    }
    // 0x260900: n = InterlockedDecrement(&m_dwRef); if (n == 0 && this) delete this; return n
    // (`delete this` is the scalar deleting destructor 0x260b08; the storage
    // came from malloc in CreateBindHost, so it goes back with free -- see there.)
    ULONG STDMETHODCALLTYPE Release() override {
        LONG n = ::InterlockedDecrement(&m_dwRef);
        if (n == 0) {
            this->~AfxBindHost();
            ::free(this);
        }
        return static_cast<ULONG>(n);
    }
    // 0x2609a0:
    //   if (!szName || !ppmk) return E_POINTER;
    //   if (*szName == 0) return E_INVALIDARG;
    //   *ppmk = NULL;
    //   hr = CreateURLMoniker(NULL, szName, ppmk);       // delay-load 0x1803e9520
    //   if (SUCCEEDED(hr) && *ppmk == NULL) hr = E_FAIL;
    //   return hr;
    // pBC and dwReserved are not read.
    HRESULT STDMETHODCALLTYPE CreateMoniker(LPOLESTR szName, IBindCtx* /*pBC*/,
                                            IMoniker** ppmk, DWORD /*dwReserved*/) override {
        if (!szName || !ppmk) return E_POINTER;
        if (*szName == L'\0') return E_INVALIDARG;
        *ppmk = nullptr;
        static PfnCreateURLMoniker pfn = nullptr;
        if (!pfn) pfn = reinterpret_cast<PfnCreateURLMoniker>(ResolveUrlmon("CreateURLMoniker"));
        if (!pfn) return UrlmonMissing();          // deviation, see top note
        HRESULT hr = pfn(nullptr, szName, ppmk);
        if (SUCCEEDED(hr) && *ppmk == nullptr) hr = E_FAIL;
        return hr;
    }
    // 0x260a00:
    //   if (!pMk || !ppvObj) return E_POINTER;
    //   *ppvObj = NULL;
    //   IPTR(IBindCtx) pBindCtx;
    //   if (pBC) {
    //       pBindCtx = pBC;                                        // AddRef
    //       if (pBSC) { hr = RegisterBindStatusCallback(pBindCtx, pBSC, NULL, 0);   // 0x1803e9528
    //                   if (FAILED(hr)) goto done; }
    //   } else {
    //       hr = pBSC ? CreateAsyncBindCtx(0, pBSC, NULL, &pBindCtx)   // 0x1803e9538
    //                 : CreateBindCtx(0, &pBindCtx);                   // IAT 0x1802c7af0 ole32!CreateBindCtx
    //       if (FAILED(hr)) goto done;
    //       if (pBindCtx == NULL) { hr = E_FAIL; goto done; }
    //   }
    //   hr = pMk->BindToStorage(pBindCtx, NULL, riid, ppvObj);        // IMoniker slot 9 (+0x48)
    // done:
    //   pBindCtx.~IPTR() /*0x243a78*/; return hr;
    HRESULT STDMETHODCALLTYPE MonikerBindToStorage(IMoniker* pMk, IBindCtx* pBC,
                                                   IBindStatusCallback* pBSC, REFIID riid,
                                                   void** ppvObj) override {
        if (!pMk || !ppvObj) return E_POINTER;
        *ppvObj = nullptr;
        IBindCtx* pBindCtx = nullptr;
        HRESULT hr;
        if (pBC) {
            IptrAssign(pBindCtx, pBC);
            if (pBSC) {
                static PfnRegisterBindStatusCallback pfn = nullptr;
                if (!pfn) pfn = reinterpret_cast<PfnRegisterBindStatusCallback>(ResolveUrlmon("RegisterBindStatusCallback"));
                hr = pfn ? pfn(pBindCtx, pBSC, nullptr, 0) : UrlmonMissing();
                if (FAILED(hr)) { IptrRelease(pBindCtx); return hr; }
            }
        } else {
            if (pBSC) {
                static PfnCreateAsyncBindCtx pfn = nullptr;
                if (!pfn) pfn = reinterpret_cast<PfnCreateAsyncBindCtx>(ResolveUrlmon("CreateAsyncBindCtx"));
                hr = pfn ? pfn(0, pBSC, nullptr, &pBindCtx) : UrlmonMissing();
            } else {
                hr = ::CreateBindCtx(0, &pBindCtx);
            }
            if (FAILED(hr)) { IptrRelease(pBindCtx); return hr; }
            if (!pBindCtx) return E_FAIL;
        }
        hr = pMk->BindToStorage(pBindCtx, nullptr, riid, ppvObj);
        IptrRelease(pBindCtx);
        return hr;
    }
    // vftable slot 5 is the folded `return E_NOTIMPL` body at 0xf4d0.
    HRESULT STDMETHODCALLTYPE MonikerBindToObject(IMoniker*, IBindCtx*, IBindStatusCallback*,
                                                  REFIID, void**) override {
        return E_NOTIMPL;
    }
};
static_assert(sizeof(AfxBindHost) == 0x10, "retail bind host is vptr + LONG");

} // namespace

// ---------------------------------------------------------------------------
// static IBindHost* CMonikerFile::CreateBindHost() -- retail 0x2610a0:
//   p = operator new(0x10);          // 0x27f0
//   if (p) p = new(p) AfxBindHost;   // 0x2608d4 (vptr, m_dwRef = 0)
//   p->AddRef();                     // vtable slot 1, unconditionally
//   return p;
// Two deviations: (1) the 0x10 bytes come from malloc + placement-new rather
// than operator new.  Not because operator new would fail to link -- the DLL is
// linked with g++, `_ZdlPvy` is already an undefined symbol of this TU and
// core/file/CFileVtable.cpp:51 calls ::operator delete -- but because the
// workflow's per-file link audit (checkfile.sh) rejects any NEW `_Z...`
// undefined symbol in a file, and `new B()` emits `_Znwy`.  malloc/placement-new
// is equivalent here: nothing outside this file allocates or frees an
// AfxBindHost, and AfxBindHost::Release frees it with free.  (2) retail's
// operator new (0x27f0) loops malloc and the AFX_MODULE_THREAD_STATE new
// handler (+0x50, installed by ??0AFX_MODULE_THREAD_STATE as 0x8a8b0 =
// AfxNewHandler, which tail-calls AfxThrowMemoryException 0x2276c0), so under
// the default handler it throws and the NULL branch (which would fault on the
// AddRef) is never taken; it returns NULL only if a client cleared the handler
// with AfxSetNewHandler(NULL).  Here a NULL from malloc is returned as-is and
// the caller's ENSURE(pBindHost) then raises the invalid-arg exception.
// Symbol: ?CreateBindHost@CMonikerFile@@KAPEAUIBindHost@@XZ
extern "C" IBindHost* MS_ABI impl__CreateBindHost_CMonikerFile__KAPEAUIBindHost__XZ() {
    void* mem = ::malloc(sizeof(AfxBindHost));
    if (!mem) return nullptr;
    AfxBindHost* p = new (mem) AfxBindHost();
    p->AddRef();
    return p;
}

// IBindCtx* CMonikerFile::CreateBindContext(CFileException*) -- the export
// resolves to 0x71e0, the linker-folded `xor %eax,%eax; ret` body shared with
// ~130 other "return 0" exports: retail returns NULL and never touches pError.
// The bind host then creates the real bind context inside
// MonikerBindToStorage (CreateBindCtx / CreateAsyncBindCtx, above).  A previous
// body here called ::CreateBindCtx itself; that was not what retail does.
// Symbol: ?CreateBindContext@CMonikerFile@@IEAAPEAUIBindCtx@@PEAVCFileException@@@Z
extern "C" IBindCtx* MS_ABI impl__CreateBindContext_CMonikerFile__IEAAPEAUIBindCtx__PEAVCFileException___Z(
    void* /*pThis*/, CFileException* /*pError*/) {
    return nullptr;
}

// void CMonikerFile::Close() -- retail 0x261040:
//   if (m_Moniker) m_Moniker.Release();      // Release() + NULL (0x243bec)
//   COleStreamFile::Close();                 // tail jump 0x265760
// Symbol: ?Close@CMonikerFile@@UEAAXXZ
extern "C" void MS_ABI impl__Close_CMonikerFile__UEAAXXZ(void* pThis) {
    if (!pThis) return;   // deviation: retail has no guard (it would fault)
    IptrRelease(Moniker(pThis));
    impl__Close_COleStreamFile__UEAAXXZ(pThis);
}

// void CMonikerFile::Flush() -- retail 0x260ba0:
//   ENSURE(GetStream() != NULL);             // 0x2657a0, else AfxThrowInvalidArgException 0x227720
//   GetStream()->Commit(STGC_DEFAULT);       // IStream slot 8 (+0x40), edx = 0; HRESULT ignored
// (Retail does not go through COleStreamFile::Flush.)
// Symbol: ?Flush@CMonikerFile@@UEAAXXZ
extern "C" void MS_ABI impl__Flush_CMonikerFile__UEAAXXZ(void* pThis) {
    if (!pThis) return;   // deviation: retail has no guard (it would fault)
    IStream* s = Stream(pThis);
    if (!s) impl__AfxThrowInvalidArgException__YAXXZ();
    s->Commit(STGC_DEFAULT);
}

// BOOL CMonikerFile::Detach(CFileException* pError) -- retail 0x261070:
//   try { Close(); }                          // virtual, slot 22
//   catch (CFileException* e) {               // funclet 0x2c37ea, type .PEAVCFileException@@
//       if (pError) { pError->m_cause = e->m_cause; pError->m_lOsError = e->m_lOsError;
//                     pError->m_strFileName = e->m_strFileName; }
//       if (e) e->Delete();
//       return FALSE;                         // continuation 0x261091
//   }
//   return TRUE;
// Deviation: the catch arm is not reproduced (a gcc catch cannot see this
// DLL's _CxxThrowException-raised MFC exceptions -- see the top note), so a
// Close override that throws unwinds through this frame and pError is never
// filled.  This DLL's own Close (above) does not throw.
// Symbol: ?Detach@CMonikerFile@@QEAAHPEAVCFileException@@@Z
extern "C" int MS_ABI impl__Detach_CMonikerFile__QEAAHPEAVCFileException___Z(void* pThis, CFileException* /*pError*/) {
    if (!pThis) return 0;   // deviation: retail has no guard (it would fault)
    VClose(pThis);
    return 1;
}

// BOOL CMonikerFile::PostBindToStream(CFileException* pError) -- retail 0x2610f0:
//   if (GetStream() != NULL) return TRUE;
//   if (pError) _AfxFillOleFileException(pError, E_UNEXPECTED /*0x8000ffff*/);
//   Close();                                  // virtual, slot 22
//   return FALSE;
// Symbol: ?PostBindToStream@CMonikerFile@@MEAAHPEAVCFileException@@@Z
extern "C" int MS_ABI impl__PostBindToStream_CMonikerFile__MEAAHPEAVCFileException___Z(void* pThis, CFileException* pError) {
    if (!pThis) return 0;   // deviation: retail has no guard (it would fault)
    if (Stream(pThis) != nullptr) return 1;
    if (pError) FillOleFileException(pError, E_UNEXPECTED);
    VClose(pThis);
    return 0;
}

// BOOL CMonikerFile::Attach(IMoniker* pMoniker, IBindHost* pBindHost,
//                           IBindStatusCallback* pBSC, IBindCtx* pBindCtx,
//                           CFileException* pError) -- retail 0x260f20:
//   m_Moniker = pMoniker;                     // IPTR operator= (0x243bd4 -> 0x243cac): AddRef new, Release old
//   IPTR(IStream) pStream;                    // NULL
//   ENSURE(pBindHost != NULL);                // else AfxThrowInvalidArgException (0x261035) -- AFTER the store above
//   hr = pBindHost->MonikerBindToStorage(pMoniker, pBindCtx, pBSC, IID_IStream /*.rdata 0x2d9af8*/, (void**)&pStream);  // IBindHost slot 4 (+0x20)
//   if (FAILED(hr)) { if (pError) _AfxFillOleFileException(pError, hr); pStream.~IPTR() /*0x243a78*/; return FALSE; }
//   if (pStream != NULL && GetStream() == NULL) { COleStreamFile::Attach(pStream); pStream = NULL /*0x261178, no Release*/; }
//   b = PostBindToStream(pError);             // virtual, slot 29
//   pStream.~IPTR();                          // 0x243a78; no-op after the hand-over
//   return b;
// Symbol: ?Attach@CMonikerFile@@IEAAHPEAUIMoniker@@PEAUIBindHost@@PEAUIBindStatusCallback@@PEAUIBindCtx@@PEAVCFileException@@@Z
extern "C" int MS_ABI impl__Attach_CMonikerFile__IEAAHPEAUIMoniker__PEAUIBindHost__PEAUIBindStatusCallback__PEAUIBindCtx__PEAVCFileException___Z(
    void* pThis, IMoniker* pMoniker, IBindHost* pBindHost, IBindStatusCallback* pBSC, IBindCtx* pBindCtx, CFileException* pError) {
    if (!pThis) return 0;   // deviation: retail has no guard (it would fault)
    IptrAssign(Moniker(pThis), pMoniker);
    IStream* pStream = nullptr;
    if (!pBindHost) impl__AfxThrowInvalidArgException__YAXXZ();
    HRESULT hr = pBindHost->MonikerBindToStorage(pMoniker, pBindCtx, pBSC, IID_IStream,
                                                 reinterpret_cast<void**>(&pStream));
    if (FAILED(hr)) {
        if (pError) FillOleFileException(pError, hr);
        IptrRelease(pStream);
        return 0;
    }
    if (pStream != nullptr && Stream(pThis) == nullptr) {
        impl__Attach_COleStreamFile__QEAAXPEAUIStream___Z(static_cast<COleStreamFile*>(pThis), pStream);
        pStream = nullptr;
    }
    int b = VPostBindToStream(pThis, pError);
    IptrRelease(pStream);
    return b;
}

// BOOL CMonikerFile::Attach(LPCTSTR lpszURL, IBindHost* pBindHost,
//                           IBindStatusCallback* pBSC, IBindCtx* pBindCtx,
//                           CFileException* pError) -- retail 0x260c20:
//   [USES_CONVERSION scaffolding: a local whose first qword is zeroed (0x76b20)
//    and whose destructor (0x87594) frees a linked list; in this Unicode image
//    T2COLE is the identity (0x10130, `mov %rcx,%rax`), nothing is ever put in
//    the list, so the scaffolding is omitted here]
//   ENSURE(pBindHost != NULL);                // else AfxThrowInvalidArgException (0x260d76)
//   if (lpszURL == NULL || *lpszURL == 0) {
//       if (pError) { pError->m_cause = CFileException::badPath /*3*/; pError->m_strFileName = lpszURL; }
//       return FALSE;
//   }
//   IPTR(IMoniker) pMoniker;                  // NULL
//   lpszOle = T2COLE(lpszURL);                // identity
//   if (lpszOle == NULL && pError) { pError->m_cause = none /*0*/; pError->m_strFileName = lpszURL; }   // no return; dead here (lpszURL is non-NULL)
//   hr = pBindHost->CreateMoniker(lpszOle, pBindCtx, &pMoniker, 0);   // IBindHost slot 3 (+0x18)
//   if (FAILED(hr)) { if (pError) _AfxFillOleFileException(pError, hr); pMoniker.~IPTR() /*0x40c0*/; return FALSE; }
//   b = Attach(pMoniker, pBindHost, pBSC, pBindCtx, pError);           // direct call 0x260f20
//   pMoniker.~IPTR();                         // 0x40c0
//   return b;
// Symbol: ?Attach@CMonikerFile@@IEAAHPEB_WPEAUIBindHost@@PEAUIBindStatusCallback@@PEAUIBindCtx@@PEAVCFileException@@@Z
extern "C" int MS_ABI impl__Attach_CMonikerFile__IEAAHPEB_WPEAUIBindHost__PEAUIBindStatusCallback__PEAUIBindCtx__PEAVCFileException___Z(
    void* pThis, const wchar_t* lpszURL, IBindHost* pBindHost, IBindStatusCallback* pBSC, IBindCtx* pBindCtx, CFileException* pError) {
    if (!pThis) return 0;   // deviation: retail has no guard (it would fault)
    if (!pBindHost) impl__AfxThrowInvalidArgException__YAXXZ();
    if (lpszURL == nullptr || *lpszURL == L'\0') {
        if (pError) {
            pError->m_cause = CFileException::badPath;
            pError->m_strFileName = lpszURL;
        }
        return 0;
    }
    IMoniker* pMoniker = nullptr;
    HRESULT hr = pBindHost->CreateMoniker(const_cast<wchar_t*>(lpszURL), pBindCtx, &pMoniker, 0);
    if (FAILED(hr)) {
        if (pError) FillOleFileException(pError, hr);
        IptrRelease(pMoniker);
        return 0;
    }
    int b = impl__Attach_CMonikerFile__IEAAHPEAUIMoniker__PEAUIBindHost__PEAUIBindStatusCallback__PEAUIBindCtx__PEAVCFileException___Z(
        pThis, pMoniker, pBindHost, pBSC, pBindCtx, pError);
    IptrRelease(pMoniker);
    return b;
}

// BOOL CMonikerFile::Open(IMoniker* pMoniker, IBindHost* pBindHost,
//                         IBindStatusCallback* pBSC, IBindCtx* pBindCtx,
//                         CFileException* pError) -- retail 0x260ee0:
//   Close();                                  // virtual, slot 22
//   return Attach(pMoniker, pBindHost, pBSC, pBindCtx, pError);   // tail jump 0x260f20
// Symbol: ?Open@CMonikerFile@@MEAAHPEAUIMoniker@@PEAUIBindHost@@PEAUIBindStatusCallback@@PEAUIBindCtx@@PEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CMonikerFile__MEAAHPEAUIMoniker__PEAUIBindHost__PEAUIBindStatusCallback__PEAUIBindCtx__PEAVCFileException___Z(
    void* pThis, IMoniker* pMoniker, IBindHost* pBindHost, IBindStatusCallback* pBSC, IBindCtx* pBindCtx, CFileException* pError) {
    if (!pThis) return 0;   // deviation: retail has no guard (it would fault)
    VClose(pThis);
    return impl__Attach_CMonikerFile__IEAAHPEAUIMoniker__PEAUIBindHost__PEAUIBindStatusCallback__PEAUIBindCtx__PEAVCFileException___Z(
        pThis, pMoniker, pBindHost, pBSC, pBindCtx, pError);
}

// BOOL CMonikerFile::Open(LPCTSTR lpszURL, IBindHost* pBindHost,
//                         IBindStatusCallback* pBSC, IBindCtx* pBindCtx,
//                         CFileException* pError) -- retail 0x260be0:
//   Close();                                  // virtual, slot 22
//   return Attach(lpszURL, pBindHost, pBSC, pBindCtx, pError);     // tail jump 0x260c20
// Symbol: ?Open@CMonikerFile@@MEAAHPEB_WPEAUIBindHost@@PEAUIBindStatusCallback@@PEAUIBindCtx@@PEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CMonikerFile__MEAAHPEB_WPEAUIBindHost__PEAUIBindStatusCallback__PEAUIBindCtx__PEAVCFileException___Z(
    void* pThis, const wchar_t* lpszURL, IBindHost* pBindHost, IBindStatusCallback* pBSC, IBindCtx* pBindCtx, CFileException* pError) {
    if (!pThis) return 0;   // deviation: retail has no guard (it would fault)
    VClose(pThis);
    return impl__Attach_CMonikerFile__IEAAHPEB_WPEAUIBindHost__PEAUIBindStatusCallback__PEAUIBindCtx__PEAVCFileException___Z(
        pThis, lpszURL, pBindHost, pBSC, pBindCtx, pError);
}

// BOOL CMonikerFile::Open(IMoniker* pMoniker, CFileException* pError) -- retail 0x260e30:
//   Close();                                  // virtual, slot 22
//   IPTR(IBindHost) pBindHost(CreateBindHost());        // 0x2610a0; adopts the reference (0x243b5c)
//   IPTR(IBindCtx)  pBindCtx(CreateBindContext(pError)); // 0x71e0 (always NULL); adopts
//   b = Attach(pMoniker, pBindHost, NULL /*pBSC*/, pBindCtx, pError);   // direct call 0x260f20
//   pBindCtx.~IPTR(); pBindHost.~IPTR();      // 0x243a78: Release if non-NULL
//   return b;
// Symbol: ?Open@CMonikerFile@@UEAAHPEAUIMoniker@@PEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CMonikerFile__UEAAHPEAUIMoniker__PEAVCFileException___Z(void* pThis, IMoniker* pMoniker, CFileException* pError) {
    if (!pThis) return 0;   // deviation: retail has no guard (it would fault)
    VClose(pThis);
    IBindHost* pBindHost = impl__CreateBindHost_CMonikerFile__KAPEAUIBindHost__XZ();
    IBindCtx* pBindCtx = impl__CreateBindContext_CMonikerFile__IEAAPEAUIBindCtx__PEAVCFileException___Z(pThis, pError);
    int b = impl__Attach_CMonikerFile__IEAAHPEAUIMoniker__PEAUIBindHost__PEAUIBindStatusCallback__PEAUIBindCtx__PEAVCFileException___Z(
        pThis, pMoniker, pBindHost, nullptr, pBindCtx, pError);
    IptrRelease(pBindCtx);
    IptrRelease(pBindHost);
    return b;
}

// BOOL CMonikerFile::Open(LPCTSTR lpszURL, CFileException* pError) -- retail 0x260d80:
//   IPTR(IBindHost) pBindHost(CreateBindHost());        // 0x2610a0; adopts
//   IPTR(IBindCtx)  pBindCtx(CreateBindContext(pError)); // 0x71e0 (always NULL); adopts
//   b = Open(lpszURL, pBindHost, NULL /*pBSC*/, pBindCtx, pError);   // VIRTUAL, slot 26 (call at 0x260dfb) -- which is what does the Close()
//   pBindCtx.~IPTR(); pBindHost.~IPTR();      // 0x243a78: Release if non-NULL
//   return b;
// Symbol: ?Open@CMonikerFile@@UEAAHPEB_WPEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CMonikerFile__UEAAHPEB_WPEAVCFileException___Z(void* pThis, const wchar_t* lpszURL, CFileException* pError) {
    if (!pThis) return 0;   // deviation: retail has no guard (it would fault)
    IBindHost* pBindHost = impl__CreateBindHost_CMonikerFile__KAPEAUIBindHost__XZ();
    IBindCtx* pBindCtx = impl__CreateBindContext_CMonikerFile__IEAAPEAUIBindCtx__PEAVCFileException___Z(pThis, pError);
    int b = VOpenUrl(pThis, lpszURL, pBindHost, nullptr, pBindCtx, pError);
    IptrRelease(pBindCtx);
    IptrRelease(pBindHost);
    return b;
}

// CMonikerFile::~CMonikerFile -- retail 0x260b30:
//   *(void**)this = &CMonikerFile::vftable;   // .rdata 0x32b688 (mfc140u)
//   try { Close(); }                          // DIRECT call 0x261040 (this file's Close)
//   catch (CException* e) {                   // funclet 0x2c36d8 (FuncInfo4 of 0x260b30: type .PEAVCException@@, continuation +0x35 = 0x260b65)
//       TCHAR szMsg[512]; CString str;        // the 0x460-byte frame
//       if (e->GetErrorMessage(szMsg, 512, NULL))            // vtable slot 5 (+0x28)
//           str.Format(_T("%Ts (%Ts:%d)\n%Ts"), _T("Exception thrown in destructor"), _T("...\\olemon.cpp"), 153, szMsg);
//       else str.Format(_T("%Ts (%Ts:%d)"), ...same first three...);
//       AfxMessageBox(str, 0, 0);             // 0x1cec10
//       delete e;                             // vtable slot 1 (deleting dtor, flag 1)
//   }
//   m_Moniker.~IPTR();                        // 0x40c0: Release if non-NULL (already NULL after Close)
//   COleStreamFile::~COleStreamFile();        // 0x264e80: try { if (m_lpStream && m_bCloseOnDelete) Close(); } catch (CException*) {...}, m_strStorageName dtor, ~CFile 0x227ac0 (same try/catch around Close, then m_strFileName dtor)
// Not reproduced: the catch arm (a gcc catch cannot see this DLL's MFC
// exceptions -- top note; this file's Close and COleStreamFile::Close only
// Release() a stream and cannot throw, so the arm is unreachable through this
// DLL's own Close), the vptr store (this DLL has no MSVC-layout CMonikerFile
// vftable) and the call to the base destructor thunk.  That thunk
// (impl___1COleStreamFile__UEAA_XZ, core/ole/Thunks.cpp:1313) is written as
// `((COleStreamFile*)pThis)->~COleStreamFile()`, a VIRTUAL call that mingw
// compiles as vtable slot 0 -- GetRuntimeClass in the client's MSVC vftable --
// so on a client-built object it tears nothing down.  The base state retail's
// chain would release is therefore handled here directly: the stream was
// already released by Close(), the CFile handle is never opened on a moniker
// file (retail hides CFile::Open with a private using-declaration, and retail's
// ~CFile closes it only under m_bCloseOnDelete anyway), and retail's
// m_strStorageName has no counterpart in OpenMFC's COleStreamFile (that slot is
// `char _olestreamfile_padding[24]`), so there is no string to destroy there.
// That leaves CFile::m_strFileName -- destroyed here through OpenMFC's inline
// CString destructor.  If
// Thunks.cpp:1313 is repaired to a non-virtual base destructor call, replace
// that last line with a call to it so the string is not destroyed twice.
// Symbol: ??1CMonikerFile@@UEAA@XZ
extern "C" void MS_ABI impl___1CMonikerFile__UEAA_XZ(void* pThis) {
    if (!pThis) return;   // deviation: retail has no guard (it would fault)
    impl__Close_CMonikerFile__UEAAXXZ(pThis);
    IptrRelease(Moniker(pThis));
    static_cast<COleStreamFile*>(pThis)->m_strFileName.~CString();
}
