// CAsyncMonikerFile — OpenMFC implementation.
// Sources: filecore.cpp, manual_small_stub_implementations.cpp
//
// =============================================================================
// Every body below is transcribed from the retail export in mfc140u.dll; each
// RVA quoted is an mfc140u function ENTRY unless it says otherwise (the wave-2
// disassembler read mfc140u directly for all of them).  CAsyncMonikerFile and
// its callback object are not declared in OpenMFC's public headers, so the
// retail member layout is pinned here as offset constants (from the retail
// ctor/dtor) and every export takes a `void* pThis` -- the same arrangement as
// CDataPathProperty in core/ole/CPropExchange.cpp.
//
// One blanket deviation, recorded here instead of on every body: each exported
// thunk below opens with an `if (!pThis)` guard that returns 0 / FALSE / void.
// Retail has no such guard anywhere in this class -- a NULL `this` faults --
// so that line is never part of the transcription; it is the same convention
// core/ole/CMonikerFile.cpp spells out per body.
//
// Retail layout (client MSVC objects are allocated with it; sizeof == 0x60,
// which is what the CAsyncMonikerFile CRuntimeClass descriptor in
// core/ole/RuntimeClasses.cpp records):
//   CFile            0x00 vfptr, 0x08 m_hFile, 0x10 m_bCloseOnDelete,
//                    0x18 m_strFileName, 0x20 m_pTM
//   COleStreamFile   0x28 m_lpStream, 0x30 m_strStorageName
//   CMonikerFile     0x38 m_Moniker
//   CAsyncMonikerFile 0x40 m_pAfxBSCCurrent, 0x48 m_bStopBindingReceived,
//                    0x50 m_Binding, 0x58 m_pFormatEtc
// (retail offsets; the stream slot this DLL actually uses is discussed below)
// The CAsyncMonikerFile offsets come from the retail ctor (RVA 0x243110:
// `movq $0,0x40(%rbx)`, IPTR ctor on `lea 0x50(%rbx)`, `movq $0,0x58(%rbx)`),
// Close (0x243240: `lea 0x50(%rcx)`, `cmpl $0,0x48(%rbx)`) and EndCallbacks
// (0x2431c0: `mov 0x40(%rcx)`); 0x28 from Read (0x243200: `mov 0x28(%rcx)`)
// and the exported COleStreamFile::Attach (0x264f20: `mov %rdx,0x28(%rcx)` /
// `movl $0,0x10(%rcx)`); 0x38 from PostBindToStream (0x2439c0 -> helper
// 0x243d34 `mov 0x38(%rcx),%rax`).
//
// NOTE the OpenMFC header lays COleStreamFile out differently: include/openmfc
// afx.h's CFile is 0x18 bytes (no m_bCloseOnDelete / m_pTM), so OpenMFC's
// m_lpStream sits at +0x18, not retail's +0x28, and there is no
// m_strStorageName.  Every COleStreamFile thunk in this DLL (Read/Seek/
// GetLength/Close/Attach/Detach, core/ole/Thunks.cpp and COleStreamFile.cpp,
// through the C++ member itself) and core/ole/CMonikerFile.cpp's Attach/
// Close/Detach (through the Stream() view in detail/COleStreamFileSupport.h)
// use the +0x18 slot, so this file uses that same slot via Stream() for the
// stream -- Read reads it and OnDataAvailable
// attaches through the exported COleStreamFile::Attach thunk, exactly the
// export retail calls.  A stream at retail's +0x28 would be invisible to the
// base-class Close that has to release it.  The 0x10-byte header gap (a
// client reading its public m_lpStream sees +0x28) is reported, not patched.
// All CMonikerFile / CAsyncMonikerFile members below +0x38 are retail offsets
// and agree with CMonikerFile.cpp's kOff_m_Moniker.
//
// Virtual dispatch on the CAsyncMonikerFile object goes through its own MSVC
// vtable by slot index (the retail call sites are `mov (%rcx),%rax ; call
// *0xNN(%rax)`).  Slot numbers were read from the retail CAsyncMonikerFile
// vftable (mfc140u .rdata 0x32b520, 44 slots, dumped with vtdump_u.py):
//   22 Close                         (0xb0)
//   27 Open(IMoniker*, CFileException*)                (0xd8)
//   34 Open(IMoniker*, IBindHost*, CFileException*)    (0x110)
//   35 Open(LPCTSTR, IBindHost*, CFileException*)      (0x118)
//   36 CreateBindStatusCallback  37 GetBindInfo  38 GetPriority
//   39 OnDataAvailable  40 OnLowResource  41 OnStartBinding
//   42 OnProgress  43 OnStopBinding                    (0x120..0x158)
// That vtable is what a client-derived class (the normal use of this class)
// carries.  OpenMFC has no CAsyncMonikerFile vtable of its own, so the ctor
// cannot plant one the way retail does (`lea 0x32b520 ; mov %rax,(%rbx)`);
// a directly instantiated CAsyncMonikerFile keeps the gcc COleStreamFile vptr
// the base ctor thunk installs.  A hand-authored MSVC vtable in the style of
// core/file/CFileVtable.cpp is the follow-up (reported).
//
// Exceptions: every callback method has a catch handler (the `mov 0xNN(%rsp),
// %eax` landing pads after each METHOD_PROLOGUE_EX pop) whose funclet stores
// `hr = COleException::Process(e); e->Delete()` into the frame; the funclets
// sit at the end of .text (0x2c2545..0x2c2648 have exactly that shape, and
// 0x2c25fc is unmistakably OnDataAvailable's -- it also does
// m_pOwner->SetFormatEtc(NULL) first).  The destructor has one too: a
// catch (CException*) around Close() that shows an AfxMessageBox and deletes
// the exception (funclet 0x2c26ad, see the dtor).  OpenMFC raises its MFC
// exceptions through _CxxThrowException, which a gcc catch clause in this
// DLL cannot see (see the note in core/ole/COleDocument.cpp), so those
// handlers are NOT reproduced: an exception thrown by an override propagates
// out of the COM callback (and out of the destructor).
// =============================================================================

#define OPENMFC_APPCORE_IMPL

#include "detail/FilecoreSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/COleStreamFileSupport.h"   // Stream(): the DLL's m_lpStream slot

#include <cstdlib>
#include <cstring>
#include <new>

// -----------------------------------------------------------------------------
// Thunks from other translation units (each definition checked in the tree;
// the CMonikerFile ones against the core/ole/CMonikerFile.cpp of this wave,
// whose parameter lists these declarations match).
// -----------------------------------------------------------------------------
// core/ole/COleStreamFile.cpp
extern "C" void* MS_ABI impl___0COleStreamFile__QEAA_PEAUIStream___Z(void* pThis, IStream* lpStream);
// core/ole/Thunks.cpp:1403 -- COleStreamFile::Attach (retail export 0x264f20)
extern "C" void MS_ABI impl__Attach_COleStreamFile__QEAAXPEAUIStream___Z(COleStreamFile* pThis, IStream* pStream);
// core/ole/CMonikerFile.cpp
extern "C" void MS_ABI impl___1CMonikerFile__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__Close_CMonikerFile__UEAAXXZ(void* pThis);
extern "C" int MS_ABI impl__Attach_CMonikerFile__IEAAHPEB_WPEAUIBindHost__PEAUIBindStatusCallback__PEAUIBindCtx__PEAVCFileException___Z(
    void* pThis, const wchar_t* lpszURL, IBindHost* pBindHost, IBindStatusCallback* pBSC,
    IBindCtx* pBindCtx, CFileException* pError);
extern "C" int MS_ABI impl__Attach_CMonikerFile__IEAAHPEAUIMoniker__PEAUIBindHost__PEAUIBindStatusCallback__PEAUIBindCtx__PEAVCFileException___Z(
    void* pThis, IMoniker* pMoniker, IBindHost* pBindHost, IBindStatusCallback* pBSC,
    IBindCtx* pBindCtx, CFileException* pError);
extern "C" IBindCtx* MS_ABI impl__CreateBindContext_CMonikerFile__IEAAPEAUIBindCtx__PEAVCFileException___Z(
    void* pThis, CFileException* pError);
extern "C" IBindHost* MS_ABI impl__CreateBindHost_CMonikerFile__KAPEAUIBindHost__XZ();
extern "C" int MS_ABI impl__PostBindToStream_CMonikerFile__MEAAHPEAVCFileException___Z(void* pThis, CFileException* pError);
// core/runtime/AFX_MAINTAIN_STATE2.cpp (retail ctor 0x133170 / dtor 0x1331c0)
extern "C" void* MS_ABI impl___0AFX_MAINTAIN_STATE2__QEAA_PEAVAFX_MODULE_STATE___Z(void* pThis, void* pState);
extern "C" void MS_ABI impl___1AFX_MAINTAIN_STATE2__QEAA_XZ(void* pThis);
// featurepack/CMFC_misc_stubs.cpp (retail 0x25f240 / 0x25f260)
extern "C" void MS_ABI impl__AfxOleLockApp__YAXXZ();
extern "C" void MS_ABI impl__AfxOleUnlockApp__YAXXZ();
// core/exceptions/CFileException.cpp
extern "C" int MS_ABI impl__OsErrorToException_CFileException__SAHJ_Z(long lOsError);

// -----------------------------------------------------------------------------
// This file's own exports that other bodies here call directly (retail makes
// the same calls non-virtually: `call 0x2434f0` / `call 0x243560` /
// `call 0x2431c0` / `call 0x243240`).
// -----------------------------------------------------------------------------
extern "C" int MS_ABI impl__Open_CAsyncMonikerFile__UEAAHPEB_WPEAVCFileException___Z(
    void* pThis, const wchar_t* lpszURL, CFileException* pError);
extern "C" int MS_ABI impl__Open_CAsyncMonikerFile__UEAAHPEAUIMoniker__PEAVCFileException___Z(
    void* pThis, IMoniker* pMoniker, CFileException* pError);
extern "C" void MS_ABI impl__EndCallbacks_CAsyncMonikerFile__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__Close_CAsyncMonikerFile__UEAAXXZ(void* pThis);

namespace {

// --- retail member offsets (see the file header for the RVAs) ---------------
// (the stream slot is not listed: it is COleStreamFileSupport.h's Stream(),
// OpenMFC's +0x18 -- see the file header)
enum : size_t {
    kOff_m_Moniker              = 0x38,   // CMonikerFile   (IPTR(IMoniker))
    kOff_m_pAfxBSCCurrent       = 0x40,   // CAsyncMonikerFile
    kOff_m_bStopBindingReceived = 0x48,
    kOff_m_Binding              = 0x50,   //                (IPTR(IBinding))
    kOff_m_pFormatEtc           = 0x58,
};
// CFileException::m_cause is what Open(..., IBindHost*, pError) tests
// (`cmpl $0,0x10(%rdi)`); OpenMFC's class has it at the same offset.
static_assert(offsetof(CFileException, m_cause) == 0x10, "CFileException::m_cause");

// --- CAsyncMonikerFile MSVC vtable slots (retail vftable 0x32b520) ----------
enum {
    kSlot_Close                    = 22,
    kSlot_Open_Moniker_pError      = 27,
    kSlot_Open_Moniker_BindHost    = 34,
    kSlot_Open_Url_BindHost        = 35,
    kSlot_CreateBindStatusCallback = 36,
    kSlot_GetBindInfo              = 37,
    kSlot_GetPriority              = 38,
    kSlot_OnDataAvailable          = 39,
    kSlot_OnLowResource            = 40,
    kSlot_OnStartBinding           = 41,
    kSlot_OnProgress               = 42,
    kSlot_OnStopBinding            = 43,
};

template <class T>
inline T& AmfMember(void* pObj, size_t off) {
    return *reinterpret_cast<T*>(static_cast<char*>(pObj) + off);
}
// `mov (%rcx),%rax ; call *0xNN(%rax)`: slot NN/8 of the object's own vtable.
template <class Fn>
inline Fn AmfSlot(void* pObj, int slot) {
    return reinterpret_cast<Fn>((*static_cast<void***>(pObj))[slot]);
}

using CloseFn            = void (MS_ABI*)(void*);
using OpenMonikerErrFn   = int  (MS_ABI*)(void*, IMoniker*, CFileException*);
using OpenMonikerHostFn  = int  (MS_ABI*)(void*, IMoniker*, IBindHost*, CFileException*);
using OpenUrlHostFn      = int  (MS_ABI*)(void*, const wchar_t*, IBindHost*, CFileException*);
using CreateBSCFn        = IUnknown* (MS_ABI*)(void*, IUnknown*);
using GetBindInfoFn      = unsigned long (MS_ABI*)(const void*);
using GetPriorityFn      = long (MS_ABI*)(const void*);
using OnDataAvailableFn  = void (MS_ABI*)(void*, unsigned long, unsigned long);
using VoidFn             = void (MS_ABI*)(void*);
using OnProgressFn       = void (MS_ABI*)(void*, unsigned long, unsigned long, unsigned long, const wchar_t*);
using OnStopBindingFn    = void (MS_ABI*)(void*, long, const wchar_t*);

// --- GUIDs read out of mfc140u .rdata (the `lea` operands at the QI sites) --
// 0x2d9e88: {79EAC9C1-BAF9-11CE-8C82-00AA004BA90B} = IID_IBindStatusCallback
const GUID kIID_IBindStatusCallback =
    { 0x79EAC9C1, 0xBAF9, 0x11CE, { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B } };
// 0x2d9e98: {FC4801A1-2BA9-11CF-A229-00AA003D7352} = IID_IBindHost (== SID_SBindHost)
const GUID kIID_IBindHost =
    { 0xFC4801A1, 0x2BA9, 0x11CF, { 0xA2, 0x29, 0x00, 0xAA, 0x00, 0x3D, 0x73, 0x52 } };
// 0x2d9f70: {6D5140C1-7436-11CE-8034-00AA006009FA} = IID_IServiceProvider
const GUID kIID_IServiceProvider =
    { 0x6D5140C1, 0x7436, 0x11CE, { 0x80, 0x34, 0x00, 0xAA, 0x00, 0x60, 0x09, 0xFA } };
// 0x2d9a58: {00000000-0000-0000-C000-000000000046} = IID_IUnknown
const GUID kIID_IUnknown =
    { 0x00000000, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };

// METHOD_PROLOGUE_EX as the callback methods spell it: AFX_MAINTAIN_STATE2
// built from the callback's m_pModuleState (`mov 0x30(%rcx),%rdx ; call
// 0x133170`) and popped on every exit (0x1331c0).  Same shape as the
// MaintainState helper in featurepack/controls/CMFCBaseAccessibleObject.cpp.
struct MaintainState {
    void* m_slot[2];
    explicit MaintainState(void* pModuleState) {
        impl___0AFX_MAINTAIN_STATE2__QEAA_PEAVAFX_MODULE_STATE___Z(m_slot, pModuleState);
    }
    ~MaintainState() { impl___1AFX_MAINTAIN_STATE2__QEAA_XZ(m_slot); }
    MaintainState(const MaintainState&) = delete;
    MaintainState& operator=(const MaintainState&) = delete;
};

// _AfxFillOleFileException (mfc140u 0x264f74) and _AfxThrowOleFileException
// (0x264fe4), the pair COleStreamFile's I/O uses; the same transcription as
// in core/ole/COleDocument.cpp (file-local there too).  Only FAILED(sc)
// throws -- `test %ecx,%ecx ; jns ret`.
void FillOleFileException(int& cause, long& lOsError, HRESULT sc) {
    if (FAILED(sc) && (sc & 0x1FFF0000) == 0x00030000 /*FACILITY_STORAGE*/ &&
        (sc & 0xFFFF) < 0x100) {
        lOsError = sc & 0xFFFF;
        cause = impl__OsErrorToException_CFileException__SAHJ_Z(lOsError);
        return;
    }
    lOsError = sc;
    if (sc == static_cast<HRESULT>(0x80030100) /*STG_E_INUSE*/ ||
        (static_cast<unsigned long>(sc) - 0x80030101UL) <= 4 /*STG_E_NOTCURRENT..STG_E_OLDDLL*/ ||
        sc == static_cast<HRESULT>(0x80030106) /*STG_E_SHAREREQUIRED*/) {
        cause = CFileException::sharingViolation;
    } else {
        cause = CFileException::genericException;
    }
}
void ThrowOleFileException(HRESULT sc) {
    if (!FAILED(sc)) return;
    int cause = CFileException::none;
    long lOsError = -1;
    FillOleFileException(cause, lOsError, sc);
    impl__AfxThrowFileException__YAXHJPEB_W_Z(cause, lOsError, nullptr);
}

// urlmon!IsAsyncMoniker is a DELAY-LOAD import in retail (slot 0x1803e9530,
// resolved with dlyu.py).  OpenMFC's link line carries no urlmon import
// library, so the same late binding is done by hand here.
typedef HRESULT (WINAPI* IsAsyncMonikerFn)(IMoniker*);
IsAsyncMonikerFn GetIsAsyncMoniker() {
    static IsAsyncMonikerFn s_pfn = nullptr;
    static bool s_bTried = false;
    if (!s_bTried) {
        s_bTried = true;
        HMODULE hUrlMon = ::LoadLibraryW(L"urlmon.dll");
        if (hUrlMon) {
            s_pfn = reinterpret_cast<IsAsyncMonikerFn>(::GetProcAddress(hUrlMon, "IsAsyncMoniker"));
        }
    }
    return s_pfn;
}

// -----------------------------------------------------------------------------
// _AfxBindStatusCallback -- the DLL-internal IBindStatusCallback that
// CreateBindStatusCallback (0x243a00) allocates with operator new(0x38) and
// constructs at 0x242a68.  Retail layout (from that ctor):
//   0x00 vfptr (IBindStatusCallback, retail vftable 0x32b780, 11 slots)
//   0x08 m_dwRef                      `movl $0,0x8(%rcx)`
//   0x10 m_pUnkControlling            `cmovne %rdi,%rbx ; mov %rbx,0x10(%rsi)`
//        = pUnkControlling if given, else &m_xInnerUnknown
//   0x18 m_xInnerUnknown vfptr        (retail vftable 0x32b7e0, 3 slots)
//   0x20 m_xInnerUnknown.m_pThis      helper 0x242980 `mov %rdx,0x8(%rcx)`
//   0x28 m_pOwner  (CAsyncMonikerFile*)
//   0x30 m_pModuleState  = AfxGetModuleState()      (0x133930)
// then AfxOleLockApp() (0x25f240).  The dtor (0x242ad4) pushes the module
// state and calls AfxOleUnlockApp() (0x25f260).
//
// The object never leaves this DLL except through its two interfaces, so it
// is an ordinary mingw COM object here (the same treatment as the UIAnimation
// handlers in core/animation/CAnimationController.cpp); the member order and
// size still match retail.
//
// Outer IUnknown (0x242b60 / 0x242b20 / 0x242b40): every call is forwarded to
// m_pUnkControlling -- `mov 0x10(%rcx),%rcx ; mov (%rcx),%rax ; jmp slot`.
// Inner unknown (0x2429e0 / 0x242990 / 0x2429b0): QueryInterface answers
// IID_IUnknown with itself and IID_IBindStatusCallback with the outer object
// (AddRef'ing what it hands out), E_POINTER for a NULL out-pointer,
// E_NOINTERFACE otherwise; AddRef/Release are InterlockedIncrement/Decrement
// on m_dwRef and Release deletes the object at zero (0x243064: dtor 0x242ad4
// + sized operator delete of 0x38; here the explicit dtor + free).
// -----------------------------------------------------------------------------
struct AfxBindStatusCallback;

struct AfxBSCInnerUnknown : IUnknown {
    AfxBindStatusCallback* m_pThis;

    STDMETHODIMP QueryInterface(REFIID riid, void** ppv) override;
    STDMETHODIMP_(ULONG) AddRef() override;
    STDMETHODIMP_(ULONG) Release() override;
};

struct AfxBindStatusCallback : IBindStatusCallback {
    LONG               m_dwRef;
    IUnknown*          m_pUnkControlling;
    AfxBSCInnerUnknown m_xInnerUnknown;
    void*              m_pOwner;          // CAsyncMonikerFile*
    void*              m_pModuleState;    // AFX_MODULE_STATE*

    AfxBindStatusCallback(void* pOwner, IUnknown* pUnkControlling) {
        m_dwRef = 0;
        m_xInnerUnknown.m_pThis = this;
        m_pOwner = pOwner;
        m_pModuleState = impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
        m_pUnkControlling = pUnkControlling ? pUnkControlling
                                            : static_cast<IUnknown*>(&m_xInnerUnknown);
        impl__AfxOleLockApp__YAXXZ();
    }
    ~AfxBindStatusCallback() {
        MaintainState state(m_pModuleState);
        impl__AfxOleUnlockApp__YAXXZ();
    }

    // _AfxBindStatusCallback::Close (0x242b10): `movq $0,0x28(%rcx)`.
    void Close() { m_pOwner = nullptr; }
    // Helper 0x242b80: `lea 0x18(%rcx),%rax` -- the inner unknown.
    IUnknown* GetInnerUnknown() { return &m_xInnerUnknown; }

    // --- IUnknown: forwarded to the controlling unknown -------------------
    STDMETHODIMP QueryInterface(REFIID riid, void** ppv) override {
        return m_pUnkControlling->QueryInterface(riid, ppv);
    }
    STDMETHODIMP_(ULONG) AddRef() override { return m_pUnkControlling->AddRef(); }
    STDMETHODIMP_(ULONG) Release() override { return m_pUnkControlling->Release(); }

    // --- IBindStatusCallback ------------------------------------------------
    // OnStartBinding (0x242c30):
    //   if (!pBinding) return E_POINTER;
    //   if (!m_pOwner)  return E_FAIL;
    //   METHOD_PROLOGUE_EX
    //   m_pOwner->SetBinding(pBinding);        ; 0x243d90: if (m_Binding != p) {
    //                                          ;   m_Binding = p; p->AddRef(); old->Release(); }
    //   m_pOwner->OnStartBinding();            ; slot 41
    //   return S_OK;
    STDMETHODIMP OnStartBinding(DWORD /*dwReserved*/, IBinding* pBinding) override {
        if (!pBinding) return E_POINTER;
        if (!m_pOwner) return E_FAIL;
        MaintainState state(m_pModuleState);
        IBinding*& rBinding = AmfMember<IBinding*>(m_pOwner, kOff_m_Binding);
        if (rBinding != pBinding) {
            IBinding* pOld = rBinding;
            rBinding = pBinding;
            pBinding->AddRef();
            if (pOld) pOld->Release();
        }
        AmfSlot<VoidFn>(m_pOwner, kSlot_OnStartBinding)(m_pOwner);
        return S_OK;
    }
    // GetPriority (0x242cc0):
    //   if (!pnPriority) return E_POINTER;
    //   if (!m_pOwner)   return E_FAIL;
    //   METHOD_PROLOGUE_EX
    //   *pnPriority = m_pOwner->GetPriority(); ; slot 38
    //   return S_OK;
    STDMETHODIMP GetPriority(LONG* pnPriority) override {
        if (!pnPriority) return E_POINTER;
        if (!m_pOwner) return E_FAIL;
        MaintainState state(m_pModuleState);
        *pnPriority = AmfSlot<GetPriorityFn>(m_pOwner, kSlot_GetPriority)(m_pOwner);
        return S_OK;
    }
    // OnLowResource (0x242f30):
    //   if (!m_pOwner) return E_FAIL;
    //   METHOD_PROLOGUE_EX
    //   m_pOwner->OnLowResource();             ; slot 40
    //   return S_OK;
    STDMETHODIMP OnLowResource(DWORD /*dwReserved*/) override {
        if (!m_pOwner) return E_FAIL;
        MaintainState state(m_pModuleState);
        AmfSlot<VoidFn>(m_pOwner, kSlot_OnLowResource)(m_pOwner);
        return S_OK;
    }
    // OnProgress (0x242d40):
    //   if (!m_pOwner) return E_FAIL;
    //   METHOD_PROLOGUE_EX
    //   CString str(szStatusText);             ; 0xdcb0 CStringT(LPCWSTR)
    //   m_pOwner->OnProgress(ulProgress, ulProgressMax, ulStatusCode,
    //                        szStatusText ? (LPCTSTR)str : NULL);   ; slot 42
    //   return S_OK;
    // Deviation: the CString copy of szStatusText is not made; the override
    // receives the caller's pointer, which holds the same characters (Unicode
    // build, no conversion).
    STDMETHODIMP OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode,
                            LPCWSTR szStatusText) override {
        if (!m_pOwner) return E_FAIL;
        MaintainState state(m_pModuleState);
        AmfSlot<OnProgressFn>(m_pOwner, kSlot_OnProgress)(
            m_pOwner, ulProgress, ulProgressMax, ulStatusCode, szStatusText ? szStatusText : nullptr);
        return S_OK;
    }
    // OnStopBinding (0x242f90):
    //   if (!m_pOwner) return E_FAIL;
    //   METHOD_PROLOGUE_EX
    //   m_pOwner->m_bStopBindingReceived = TRUE;          ; movl $1,0x48(%rax)
    //   CString str(szError);
    //   m_pOwner->OnStopBinding(hresult, szError ? (LPCTSTR)str : NULL);  ; slot 43
    //   if (m_pOwner) m_pOwner->EndCallbacks();           ; 0x2431c0 -- may delete this
    //   return S_OK;
    // Same CString deviation as OnProgress.  Nothing touches `this` after
    // EndCallbacks (which releases the owner's reference on this object).
    STDMETHODIMP OnStopBinding(HRESULT hresult, LPCWSTR szError) override {
        if (!m_pOwner) return E_FAIL;
        MaintainState state(m_pModuleState);
        AmfMember<int>(m_pOwner, kOff_m_bStopBindingReceived) = TRUE;
        void* pOwner = m_pOwner;
        AmfSlot<OnStopBindingFn>(pOwner, kSlot_OnStopBinding)(pOwner, hresult, szError ? szError : nullptr);
        if (m_pOwner) impl__EndCallbacks_CAsyncMonikerFile__IEAAXXZ(m_pOwner);
        return S_OK;
    }
    // GetBindInfo (0x242b90):
    //   if (!pgrfBINDF || !pbindinfo) return E_POINTER;
    //   if (pbindinfo->cbSize < 0x80) return E_INVALIDARG;   ; sizeof(BINDINFO) on x64
    //   if (!m_pOwner) return E_FAIL;
    //   METHOD_PROLOGUE_EX
    //   pbindinfo->szExtraInfo = NULL;                        ; movq $0,0x8(%rbx)
    //   *pgrfBINDF = m_pOwner->GetBindInfo();                 ; slot 37
    //   return S_OK;
    STDMETHODIMP GetBindInfo(DWORD* pgrfBINDF, BINDINFO* pbindinfo) override {
        if (!pgrfBINDF || !pbindinfo) return E_POINTER;
        if (pbindinfo->cbSize < 0x80) return E_INVALIDARG;
        if (!m_pOwner) return E_FAIL;
        MaintainState state(m_pModuleState);
        pbindinfo->szExtraInfo = nullptr;
        *pgrfBINDF = AmfSlot<GetBindInfoFn>(m_pOwner, kSlot_GetBindInfo)(m_pOwner);
        return S_OK;
    }
    // OnDataAvailable (0x242e20):
    //   if (!m_pOwner) return E_FAIL;
    //   METHOD_PROLOGUE_EX
    //   m_pOwner->SetFormatEtc(pformatetc);                   ; 0x243d88: mov %rdx,0x58(%rcx)
    //   if (grfBSCF & BSCF_FIRSTDATANOTIFICATION) {
    //       if (!pstgmed || !pformatetc) return E_POINTER;    ; (m_pFormatEtc left set)
    //       if (pstgmed->tymed != TYMED_ISTREAM || !pstgmed->pstm) return E_UNEXPECTED;
    //       m_pOwner->Attach(pstgmed->pstm);   ; COleStreamFile::Attach 0x264f20:
    //                                          ;   m_lpStream = p (+0x28); m_bCloseOnDelete = FALSE (+0x10)
    //       pstgmed->pstm->AddRef();
    //   }
    //   m_pOwner->OnDataAvailable(dwSize, grfBSCF);           ; slot 39
    //   m_pOwner->SetFormatEtc(NULL);
    //   return S_OK;
    // (the catch funclet at 0x2c25fc also does SetFormatEtc(NULL) -- not reproduced,
    // see the file header.)  The attach goes through the exported
    // COleStreamFile::Attach thunk (core/ole/Thunks.cpp:1403 -> the C++
    // COleStreamFile::Attach in COleStreamFile.cpp), which stores the stream
    // in OpenMFC's slot (+0x18), has no m_bCloseOnDelete to clear, and --
    // unlike retail's, which ENSUREs a non-NULL argument and only stores --
    // Releases a stream already in the slot (none on a first notification:
    // Open's Close() emptied it).
    STDMETHODIMP OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC* pformatetc,
                                 STGMEDIUM* pstgmed) override {
        if (!m_pOwner) return E_FAIL;
        MaintainState state(m_pModuleState);
        AmfMember<FORMATETC*>(m_pOwner, kOff_m_pFormatEtc) = pformatetc;
        if (grfBSCF & BSCF_FIRSTDATANOTIFICATION) {
            if (!pstgmed || !pformatetc) return E_POINTER;
            if (pstgmed->tymed != TYMED_ISTREAM || !pstgmed->pstm) return E_UNEXPECTED;
            impl__Attach_COleStreamFile__QEAAXPEAUIStream___Z(static_cast<COleStreamFile*>(m_pOwner), pstgmed->pstm);
            pstgmed->pstm->AddRef();
        }
        AmfSlot<OnDataAvailableFn>(m_pOwner, kSlot_OnDataAvailable)(m_pOwner, dwSize, grfBSCF);
        AmfMember<FORMATETC*>(m_pOwner, kOff_m_pFormatEtc) = nullptr;
        return S_OK;
    }
    // OnObjectAvailable: retail slot 10 is the folded body at RVA 0x23a740
    // (mfc140u), `mov $0x8000ffff,%eax ; ret` -- that constant is E_UNEXPECTED,
    // not E_NOTIMPL (0x80004001).
    STDMETHODIMP OnObjectAvailable(REFIID, IUnknown*) override { return E_UNEXPECTED; }
};
static_assert(sizeof(AfxBindStatusCallback) == 0x38, "_AfxBindStatusCallback is 0x38 bytes (the operator-new call at 0x243a15 inside CreateBindStatusCallback, entry 0x243a00)");
static_assert(offsetof(AfxBindStatusCallback, m_dwRef) == 0x08, "_AfxBindStatusCallback::m_dwRef");
static_assert(offsetof(AfxBindStatusCallback, m_pUnkControlling) == 0x10, "_AfxBindStatusCallback::m_pUnkControlling");
static_assert(offsetof(AfxBindStatusCallback, m_xInnerUnknown) == 0x18, "_AfxBindStatusCallback::m_xInnerUnknown");
static_assert(offsetof(AfxBindStatusCallback, m_pOwner) == 0x28, "_AfxBindStatusCallback::m_pOwner");
static_assert(offsetof(AfxBindStatusCallback, m_pModuleState) == 0x30, "_AfxBindStatusCallback::m_pModuleState");

// Inner unknown QueryInterface (0x2429e0).
STDMETHODIMP AfxBSCInnerUnknown::QueryInterface(REFIID riid, void** ppv) {
    if (!ppv) return E_POINTER;
    if (IsEqualGUID(riid, kIID_IUnknown)) {
        *ppv = static_cast<IUnknown*>(this);
        AddRef();
        return S_OK;
    }
    if (IsEqualGUID(riid, kIID_IBindStatusCallback)) {
        *ppv = static_cast<IBindStatusCallback*>(m_pThis);
        m_pThis->AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}
// Inner AddRef (0x242990): `lock xadd` on m_pThis->m_dwRef, returns the new count.
STDMETHODIMP_(ULONG) AfxBSCInnerUnknown::AddRef() {
    return static_cast<ULONG>(::InterlockedIncrement(&m_pThis->m_dwRef));
}
// Inner Release (0x2429b0): decrement; at zero (and a live m_pThis) delete
// the object (0x243064), return the count.
STDMETHODIMP_(ULONG) AfxBSCInnerUnknown::Release() {
    const LONG n = ::InterlockedDecrement(&m_pThis->m_dwRef);
    if (n == 0 && m_pThis) {
        AfxBindStatusCallback* p = m_pThis;
        p->~AfxBindStatusCallback();
        std::free(p);
    }
    return static_cast<ULONG>(n);
}

// Helper 0x243088: IBindHost* from an IServiceProvider* --
//   hr = pSP->QueryService(SID_SBindHost, IID_IBindHost, &p);  (slot 3, both GUIDs 0x2d9e98)
//   return SUCCEEDED(hr) ? p : NULL;
// The reference QueryService took comes back with the pointer.
IBindHost* BindHostFromServiceProvider(IServiceProvider* pSP) {
    void* p = nullptr;
    HRESULT hr = pSP->QueryService(kIID_IBindHost, kIID_IBindHost, &p);
    return SUCCEEDED(hr) ? static_cast<IBindHost*>(p) : nullptr;
}
// Helper 0x2430bc: IBindHost* from any IUnknown* --
//   IPTR(IBindHost) tmp; hr = (tmp = pUnk);   (QI IID_IBindHost, 0x243b70; NULL -> E_NOINTERFACE)
//   return SUCCEEDED(hr) ? (IBindHost*)tmp : NULL;   (tmp's dtor releases the QI reference)
// Retail returns the raw pointer after tmp has released it and the caller's
// IPTR::operator=(T*) (0x243cac) AddRefs it again; here the QI reference is
// simply handed back to the caller, which is the same net count.
IBindHost* BindHostFromUnknown(IUnknown* pUnk) {
    if (!pUnk) return nullptr;
    void* p = nullptr;
    HRESULT hr = pUnk->QueryInterface(kIID_IBindHost, &p);
    return SUCCEEDED(hr) ? static_cast<IBindHost*>(p) : nullptr;
}

// The shared tail of the LPCTSTR / IMoniker Open(..., IBindHost*, pError)
// bodies (0x2433d0 / 0x2432a0), from the Close() on.  Returns through
// bResult; the caller has already handled the NULL-pBindHost case.
//   Close();                                              ; slot 22 (virtual)
//   IPTR(IBindCtx) pBindCtx = CreateBindContext(pError);  ; direct call to the
//                                                         ; CMonikerFile export body (0x71e0)
//   if (pError && pError->m_cause != CFileException::none) return FALSE;
//   IPTR(IUnknown) pUnk = CreateBindStatusCallback(NULL); ; slot 36 (virtual)
//   IPTR(IBindStatusCallback) pBSC; pBSC = pUnk;          ; QI, 0x243af8 (result ignored)
//   m_bStopBindingReceived = FALSE;                       ; movl $0,0x48(%rbx)
//   bResult = CMonikerFile::Attach(<key>, pBindHost, pBSC, pBindCtx, pError);
//                                                         ; direct 0x260c20 / 0x260f20
//   ~pBSC; ~pUnk; ~pBindCtx;
template <class AttachFn, class Key>
int OpenWithBindHost(void* pThis, AttachFn attach, Key key, IBindHost* pBindHost, CFileException* pError) {
    AmfSlot<CloseFn>(pThis, kSlot_Close)(pThis);
    IBindCtx* pBindCtx = impl__CreateBindContext_CMonikerFile__IEAAPEAUIBindCtx__PEAVCFileException___Z(pThis, pError);
    if (pError && pError->m_cause != CFileException::none) {
        if (pBindCtx) pBindCtx->Release();
        return FALSE;
    }
    IUnknown* pUnk = AmfSlot<CreateBSCFn>(pThis, kSlot_CreateBindStatusCallback)(pThis, nullptr);
    IBindStatusCallback* pBSC = nullptr;
    if (pUnk) {
        void* p = nullptr;
        if (SUCCEEDED(pUnk->QueryInterface(kIID_IBindStatusCallback, &p))) {
            pBSC = static_cast<IBindStatusCallback*>(p);
        }
    }
    AmfMember<int>(pThis, kOff_m_bStopBindingReceived) = FALSE;
    int bResult = attach(pThis, key, pBindHost, pBSC, pBindCtx, pError);
    if (pBSC) pBSC->Release();
    if (pUnk) pUnk->Release();
    if (pBindCtx) pBindCtx->Release();
    return bResult;
}

}  // namespace

// Symbol: ??0CAsyncMonikerFile@@QEAA@XZ
// Retail RVA 0x243110 (mfc140u):
//   CMonikerFile::CMonikerFile();          ; 0x243d5c (non-exported out-of-line helper):
//                                          ;   COleStreamFile::COleStreamFile(NULL)  (0x264d10, exported)
//                                          ;   vfptr = CMonikerFile vftable ; m_Moniker (+0x38) = NULL
//   vfptr = CAsyncMonikerFile vftable (0x32b520)
//   m_pAfxBSCCurrent = NULL;               ; +0x40
//   m_Binding = NULL;                      ; +0x50, IPTR ctor 0x243ae4 -> 0x76b20
//   m_pFormatEtc = NULL;                   ; +0x58
//   return this;
// m_bStopBindingReceived (+0x48) is NOT written by the retail ctor (Open sets
// it before any binding starts); same here.  The vfptr is left as the base
// ctor thunk set it -- OpenMFC has no CAsyncMonikerFile vtable to plant (see
// the file header); a client-derived class overwrites it anyway.
extern "C" void* MS_ABI impl___0CAsyncMonikerFile__QEAA_XZ(void* pThis) {
    if (!pThis) return nullptr;
    impl___0COleStreamFile__QEAA_PEAUIStream___Z(pThis, nullptr);
    AmfMember<IMoniker*>(pThis, kOff_m_Moniker) = nullptr;
    AmfMember<void*>(pThis, kOff_m_pAfxBSCCurrent) = nullptr;
    AmfMember<IBinding*>(pThis, kOff_m_Binding) = nullptr;
    AmfMember<FORMATETC*>(pThis, kOff_m_pFormatEtc) = nullptr;
    return pThis;
}
// Symbol: ??1CAsyncMonikerFile@@UEAA@XZ
// Retail RVA 0x243150 (mfc140u):
//   vfptr = CAsyncMonikerFile vftable      ; lea 0x32b520 ; mov %rax,(%rcx)
//   try { CAsyncMonikerFile::Close(); }    ; direct call 0x243240 (devirtualised:
//                                          ; the vptr was just set to this class's)
//   catch (CException* e) {                ; catch funclet 0x2c26ad, continuation
//                                          ; 0x243185 (`mov 0x38(%rsp),%rbx` re-loads
//                                          ; this); the 0x460-byte frame is its buffer
//       TCHAR szMsg[512]; CString str;
//       if (e->GetErrorMessage(szMsg, 512, NULL))              ; vtable slot 5 (+0x28)
//           str.Format(_T("%Ts (%Ts:%d)\n%Ts"), _T("Exception thrown in destructor"),
//                      _T("...\\oleasmon.cpp"), 361, szMsg);   ; .rdata 0x33d718/0x33d7c8/0x34d980
//       else str.Format(_T("%Ts (%Ts:%d)"), ...same first three...);   ; 0x33d808
//       AfxMessageBox(str, 0, 0);          ; 0x1cec10
//       delete e;                          ; vtable slot 1 (deleting dtor, flag 1)
//   }
//   m_Binding.~IPTR();                     ; 0x243a78 -> 0x243cf4: Release if non-NULL
//   CMonikerFile::~CMonikerFile();         ; 0x260b30 (exported)
// (Same shape as ~CMonikerFile's catch arm at 0x2c36d8, which core/ole/
// CMonikerFile.cpp decoded from FuncInfo4 as type .PEAVCException@@; the
// funclet here is the only user of the oleasmon.cpp path string.)  The unwind
// funclets 0x2c2691 (~CMonikerFile on this) / 0x2c269d (m_Binding release)
// only run for an exception the catch arm does not take.
// Not reproduced: the vptr store (no OpenMFC CAsyncMonikerFile vftable, see
// the file header) and the catch arm (a gcc catch cannot see this DLL's MFC
// exceptions -- file header).  This file's own Close only calls IBinding::
// Abort, EndCallbacks and CMonikerFile::Close, none of which throws, so the
// arm is unreachable through this DLL's Close; the message box is not shown.
extern "C" void MS_ABI impl___1CAsyncMonikerFile__UEAA_XZ(void* pThis) {
    if (!pThis) return;
    impl__Close_CAsyncMonikerFile__UEAAXXZ(pThis);
    IBinding*& rBinding = AmfMember<IBinding*>(pThis, kOff_m_Binding);
    if (rBinding) rBinding->Release();
    impl___1CMonikerFile__UEAA_XZ(pThis);
}
// Symbol: ?Close@CAsyncMonikerFile@@UEAAXXZ
// Retail RVA 0x243240 (mfc140u):
//   if (m_Binding) {                       ; +0x50
//       if (!m_bStopBindingReceived)       ; cmpl $0,0x48(%rbx)
//           m_Binding->Abort();            ; IBinding slot 3 (0x18), via IPTR operator-> 0x243c08
//       m_Binding.Release();               ; 0x243bec: Release, then NULL
//   }
//   EndCallbacks();                        ; direct 0x2431c0
//   CMonikerFile::Close();                 ; tail jmp 0x261040 (exported)
extern "C" void MS_ABI impl__Close_CAsyncMonikerFile__UEAAXXZ(void* pThis) {
    if (!pThis) return;
    IBinding*& rBinding = AmfMember<IBinding*>(pThis, kOff_m_Binding);
    if (rBinding) {
        if (AmfMember<int>(pThis, kOff_m_bStopBindingReceived) == 0) {
            rBinding->Abort();
        }
        rBinding->Release();
        rBinding = nullptr;
    }
    impl__EndCallbacks_CAsyncMonikerFile__IEAAXXZ(pThis);
    impl__Close_CMonikerFile__UEAAXXZ(pThis);
}

// Symbol: ?CreateBindStatusCallback@CAsyncMonikerFile@@MEAAPEAUIUnknown@@PEAU2@@Z
// Retail RVA 0x243a00 (mfc140u):
//   p = operator new(0x38);                            ; 0x27f0 (??2@YAPEAX_K@Z)
//   if (p) p = new(p) _AfxBindStatusCallback(this, pUnkControlling);   ; 0x242a68
//   m_pAfxBSCCurrent = p;                              ; +0x40
//   IUnknown* pUnk = &p->m_xInnerUnknown;              ; 0x242b80 (no NULL test)
//   pUnk->AddRef();                                    ; slot 1
//   pUnk->AddRef();                                    ; slot 1 -- one reference for the
//                                                      ; returned pointer, one held through
//                                                      ; m_pAfxBSCCurrent until EndCallbacks
//   return pUnk;
// The storage comes from malloc: retail's operator new (entry 0x27f0) is a
// malloc wrapper -- its `call *0x1802c74c8` resolves to
// api-ms-win-crt-heap-l1-1-0!malloc -- which, when malloc returns NULL, retries
// through AfxGetModuleState()'s new handler (+0x50) and RETURNS NULL if no
// handler is installed or the handler declines.  It does not itself throw, so
// no CMemoryException claim is made here.  Storage goes back through free in
// the inner Release.
// Deviation: retail does not test that NULL -- it stores it in
// m_pAfxBSCCurrent, then 0x242b80 yields 0x18 and the following
// `mov (%rax),%rcx` faults; here an allocation failure returns NULL with
// m_pAfxBSCCurrent cleared.
extern "C" IUnknown* MS_ABI impl__CreateBindStatusCallback_CAsyncMonikerFile__MEAAPEAUIUnknown__PEAU2__Z(
    void* pThis, IUnknown* pUnkControlling) {
    if (!pThis) return nullptr;
    void* mem = std::malloc(sizeof(AfxBindStatusCallback));
    AfxBindStatusCallback* p = mem ? new (mem) AfxBindStatusCallback(pThis, pUnkControlling) : nullptr;
    AmfMember<AfxBindStatusCallback*>(pThis, kOff_m_pAfxBSCCurrent) = p;
    if (!p) return nullptr;
    IUnknown* pUnk = p->GetInnerUnknown();
    pUnk->AddRef();
    pUnk->AddRef();
    return pUnk;
}

// Symbol: ?EndCallbacks@CAsyncMonikerFile@@IEAAXXZ
// Retail RVA 0x2431c0 (mfc140u):
//   if (m_pAfxBSCCurrent) {                            ; +0x40
//       m_pAfxBSCCurrent->Close();                     ; 0x242b10: m_pOwner = NULL
//       m_pAfxBSCCurrent->m_xInnerUnknown.Release();   ; 0x242b80 then slot 2
//       m_pAfxBSCCurrent = NULL;
//   }
extern "C" void MS_ABI impl__EndCallbacks_CAsyncMonikerFile__IEAAXXZ(void* pThis) {
    if (!pThis) return;
    AfxBindStatusCallback*& rBSC = AmfMember<AfxBindStatusCallback*>(pThis, kOff_m_pAfxBSCCurrent);
    if (rBSC) {
        rBSC->Close();
        rBSC->GetInnerUnknown()->Release();
        rBSC = nullptr;
    }
}

// Symbol: ?GetBindInfo@CAsyncMonikerFile@@MEBAKXZ
// Retail RVA 0x5da20 (mfc140u, a folded body): `mov $0x3,%eax ; ret` --
// BINDF_ASYNCHRONOUS | BINDF_ASYNCSTORAGE.
extern "C" unsigned long MS_ABI impl__GetBindInfo_CAsyncMonikerFile__MEBAKXZ(const void* pThis) {
    (void)pThis;
    return BINDF_ASYNCHRONOUS | BINDF_ASYNCSTORAGE;
}

// Symbol: ?GetPriority@CAsyncMonikerFile@@MEBAJXZ
// Retail RVA 0x71e0 (mfc140u, a folded body): `xor %eax,%eax ; ret` --
// THREAD_PRIORITY_NORMAL.
extern "C" long MS_ABI impl__GetPriority_CAsyncMonikerFile__MEBAJXZ(const void* pThis) {
    (void)pThis;
    return THREAD_PRIORITY_NORMAL;
}

// The five default notification overridables are retail's shared empty body
// at RVA 0x27d0 (mfc140u): a bare `ret` (vftable 0x32b520 slots 39..43 all
// point there).
// Symbol: ?OnDataAvailable@CAsyncMonikerFile@@MEAAXKK@Z
extern "C" void MS_ABI impl__OnDataAvailable_CAsyncMonikerFile__MEAAXKK_Z(
    void* pThis, unsigned long dwSize, unsigned long bscfFlag) {
    (void)pThis; (void)dwSize; (void)bscfFlag;
}

// Symbol: ?OnLowResource@CAsyncMonikerFile@@MEAAXXZ
extern "C" void MS_ABI impl__OnLowResource_CAsyncMonikerFile__MEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?OnStartBinding@CAsyncMonikerFile@@MEAAXXZ
extern "C" void MS_ABI impl__OnStartBinding_CAsyncMonikerFile__MEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?OnProgress@CAsyncMonikerFile@@MEAAXKKKPEB_W@Z
extern "C" void MS_ABI impl__OnProgress_CAsyncMonikerFile__MEAAXKKKPEB_W_Z(
    void* pThis, unsigned long ulProgress, unsigned long ulProgressMax,
    unsigned long ulStatusCode, const wchar_t* szStatusText) {
    (void)pThis; (void)ulProgress; (void)ulProgressMax; (void)ulStatusCode; (void)szStatusText;
}

// Symbol: ?OnStopBinding@CAsyncMonikerFile@@MEAAXJPEB_W@Z
extern "C" void MS_ABI impl__OnStopBinding_CAsyncMonikerFile__MEAAXJPEB_W_Z(
    void* pThis, long hresult, const wchar_t* szError) {
    (void)pThis; (void)hresult; (void)szError;
}

// Symbol: ?Open@CAsyncMonikerFile@@UEAAHPEAUIMoniker@@PEAUIBindHost@@PEAVCFileException@@@Z
// Retail RVA 0x2432a0 (mfc140u):
//   if (!pBindHost) return Open(pMoniker, pError);      ; direct call 0x243560
//   ... the OpenWithBindHost sequence above, attaching through
//   CMonikerFile::Attach(IMoniker*, ...) (direct call 0x260f20).
extern "C" int MS_ABI impl__Open_CAsyncMonikerFile__UEAAHPEAUIMoniker__PEAUIBindHost__PEAVCFileException___Z(
    void* pThis, IMoniker* pMoniker, IBindHost* pBindHost, CFileException* pError) {
    if (!pThis) return FALSE;
    if (!pBindHost) {
        return impl__Open_CAsyncMonikerFile__UEAAHPEAUIMoniker__PEAVCFileException___Z(pThis, pMoniker, pError);
    }
    return OpenWithBindHost(
        pThis,
        impl__Attach_CMonikerFile__IEAAHPEAUIMoniker__PEAUIBindHost__PEAUIBindStatusCallback__PEAUIBindCtx__PEAVCFileException___Z,
        pMoniker, pBindHost, pError);
}

// Symbol: ?Open@CAsyncMonikerFile@@UEAAHPEAUIMoniker@@PEAUIServiceProvider@@PEAVCFileException@@@Z
// Retail RVA 0x2437c0 (mfc140u):
//   if (!pServiceProvider) return this->Open(pMoniker, pError);   ; slot 27 (virtual)
//   IPTR(IBindHost) pBindHost;
//   pBindHost = BindHostFromServiceProvider(pServiceProvider);    ; 0x243088, then operator= 0x243cac
//   if (!pBindHost) pBindHost = BindHostFromUnknown(pServiceProvider);   ; 0x2430bc
//   if (pBindHost) return this->Open(pMoniker, pBindHost, pError);  ; slot 34 (virtual)
//   return this->Open(pMoniker, pError);                          ; slot 27 (virtual)
//   (~pBindHost on both paths)
// Reference counting: retail's IPTR::operator=(T*) AddRefs the pointer the
// helper returns, so on the QueryService path one reference is never released
// (QueryService +1, operator= +1, dtor -1).  Not reproduced: here the helper's
// reference is the one the local owns and it is released once.
extern "C" int MS_ABI impl__Open_CAsyncMonikerFile__UEAAHPEAUIMoniker__PEAUIServiceProvider__PEAVCFileException___Z(
    void* pThis, IMoniker* pMoniker, IServiceProvider* pServiceProvider, CFileException* pError) {
    if (!pThis) return FALSE;
    if (!pServiceProvider) {
        return AmfSlot<OpenMonikerErrFn>(pThis, kSlot_Open_Moniker_pError)(pThis, pMoniker, pError);
    }
    IBindHost* pBindHost = BindHostFromServiceProvider(pServiceProvider);
    if (!pBindHost) pBindHost = BindHostFromUnknown(pServiceProvider);
    int bResult;
    if (pBindHost) {
        bResult = AmfSlot<OpenMonikerHostFn>(pThis, kSlot_Open_Moniker_BindHost)(pThis, pMoniker, pBindHost, pError);
    } else {
        bResult = AmfSlot<OpenMonikerErrFn>(pThis, kSlot_Open_Moniker_pError)(pThis, pMoniker, pError);
    }
    if (pBindHost) pBindHost->Release();
    return bResult;
}

// Symbol: ?Open@CAsyncMonikerFile@@UEAAHPEAUIMoniker@@PEAUIUnknown@@PEAVCFileException@@@Z
// Retail RVA 0x2438a0 (mfc140u):
//   if (!pUnknown) return this->Open(pMoniker, pError);           ; slot 27 (virtual)
//   IPTR(IBindHost) pBindHost; IPTR(IServiceProvider) pSP;
//   hr = (pSP = pUnknown);                                        ; QI IID_IServiceProvider, 0x243a80
//   if (SUCCEEDED(hr) && pSP) pBindHost = BindHostFromServiceProvider(pSP);   ; 0x243088
//   if (!pBindHost) pBindHost = BindHostFromUnknown(pSP);         ; 0x2430bc on pSP -- NOT on
//                                                                 ; pUnknown (unlike the LPCTSTR
//                                                                 ; overload); a NULL pSP yields NULL
//   if (pBindHost) return this->Open(pMoniker, pBindHost, pError);  ; slot 34 (virtual)
//   return this->Open(pMoniker, pError);                          ; slot 27 (virtual)
//   (~pSP, ~pBindHost)
// Same reference-count deviation as the IServiceProvider overload.
extern "C" int MS_ABI impl__Open_CAsyncMonikerFile__UEAAHPEAUIMoniker__PEAUIUnknown__PEAVCFileException___Z(
    void* pThis, IMoniker* pMoniker, IUnknown* pUnknown, CFileException* pError) {
    if (!pThis) return FALSE;
    if (!pUnknown) {
        return AmfSlot<OpenMonikerErrFn>(pThis, kSlot_Open_Moniker_pError)(pThis, pMoniker, pError);
    }
    IServiceProvider* pSP = nullptr;
    IBindHost* pBindHost = nullptr;
    {
        void* p = nullptr;
        HRESULT hr = pUnknown->QueryInterface(kIID_IServiceProvider, &p);
        if (SUCCEEDED(hr)) pSP = static_cast<IServiceProvider*>(p);
        if (SUCCEEDED(hr) && pSP) pBindHost = BindHostFromServiceProvider(pSP);
    }
    if (!pBindHost) pBindHost = BindHostFromUnknown(pSP);
    int bResult;
    if (pBindHost) {
        bResult = AmfSlot<OpenMonikerHostFn>(pThis, kSlot_Open_Moniker_BindHost)(pThis, pMoniker, pBindHost, pError);
    } else {
        bResult = AmfSlot<OpenMonikerErrFn>(pThis, kSlot_Open_Moniker_pError)(pThis, pMoniker, pError);
    }
    if (pSP) pSP->Release();
    if (pBindHost) pBindHost->Release();
    return bResult;
}

// Symbol: ?Open@CAsyncMonikerFile@@UEAAHPEAUIMoniker@@PEAVCFileException@@@Z
// Retail RVA 0x243560 (mfc140u):
//   IPTR(IBindHost) pBindHost = CMonikerFile::CreateBindHost();   ; 0x2610a0 (exported);
//                                                                 ; the reference it took is
//                                                                 ; the one the IPTR owns
//   bResult = this->Open(pMoniker, pBindHost, pError);            ; slot 34 (virtual)
//   ~pBindHost;
//   return bResult;
// Deviation: retail's CreateBindHost never returns NULL (it faults instead);
// OpenMFC's may, and passing NULL on to the IBindHost overload would recurse
// straight back here, so a NULL bind host returns FALSE.
extern "C" int MS_ABI impl__Open_CAsyncMonikerFile__UEAAHPEAUIMoniker__PEAVCFileException___Z(
    void* pThis, IMoniker* pMoniker, CFileException* pError) {
    if (!pThis) return FALSE;
    IBindHost* pBindHost = impl__CreateBindHost_CMonikerFile__KAPEAUIBindHost__XZ();
    if (!pBindHost) return FALSE;
    int bResult = AmfSlot<OpenMonikerHostFn>(pThis, kSlot_Open_Moniker_BindHost)(pThis, pMoniker, pBindHost, pError);
    pBindHost->Release();
    return bResult;
}

// Symbol: ?Open@CAsyncMonikerFile@@UEAAHPEB_WPEAUIBindHost@@PEAVCFileException@@@Z
// Retail RVA 0x2433d0 (mfc140u):
//   if (!pBindHost) return Open(lpszURL, pError);       ; direct call 0x2434f0
//   ... the OpenWithBindHost sequence above, attaching through
//   CMonikerFile::Attach(LPCTSTR, ...) (direct call 0x260c20).
extern "C" int MS_ABI impl__Open_CAsyncMonikerFile__UEAAHPEB_WPEAUIBindHost__PEAVCFileException___Z(
    void* pThis, const wchar_t* lpszURL, IBindHost* pBindHost, CFileException* pError) {
    if (!pThis) return FALSE;
    if (!pBindHost) {
        return impl__Open_CAsyncMonikerFile__UEAAHPEB_WPEAVCFileException___Z(pThis, lpszURL, pError);
    }
    return OpenWithBindHost(
        pThis,
        impl__Attach_CMonikerFile__IEAAHPEB_WPEAUIBindHost__PEAUIBindStatusCallback__PEAUIBindCtx__PEAVCFileException___Z,
        lpszURL, pBindHost, pError);
}

// Symbol: ?Open@CAsyncMonikerFile@@UEAAHPEB_WPEAUIServiceProvider@@PEAVCFileException@@@Z
// Retail RVA 0x2435d0 (mfc140u):
//   if (!pServiceProvider) return Open(lpszURL, pError);          ; direct call 0x2434f0
//   IPTR(IBindHost) pBindHost;
//   pBindHost = BindHostFromServiceProvider(pServiceProvider);    ; 0x243088
//   if (!pBindHost) pBindHost = BindHostFromUnknown(pServiceProvider);   ; 0x2430bc
//   if (pBindHost) return this->Open(lpszURL, pBindHost, pError); ; slot 35 (virtual)
//   return Open(lpszURL, pError);                                 ; direct call 0x2434f0
//   (~pBindHost on both paths)
// Same reference-count deviation as the IMoniker/IServiceProvider overload.
extern "C" int MS_ABI impl__Open_CAsyncMonikerFile__UEAAHPEB_WPEAUIServiceProvider__PEAVCFileException___Z(
    void* pThis, const wchar_t* lpszURL, IServiceProvider* pServiceProvider, CFileException* pError) {
    if (!pThis) return FALSE;
    if (!pServiceProvider) {
        return impl__Open_CAsyncMonikerFile__UEAAHPEB_WPEAVCFileException___Z(pThis, lpszURL, pError);
    }
    IBindHost* pBindHost = BindHostFromServiceProvider(pServiceProvider);
    if (!pBindHost) pBindHost = BindHostFromUnknown(pServiceProvider);
    int bResult;
    if (pBindHost) {
        bResult = AmfSlot<OpenUrlHostFn>(pThis, kSlot_Open_Url_BindHost)(pThis, lpszURL, pBindHost, pError);
    } else {
        bResult = impl__Open_CAsyncMonikerFile__UEAAHPEB_WPEAVCFileException___Z(pThis, lpszURL, pError);
    }
    if (pBindHost) pBindHost->Release();
    return bResult;
}

// Symbol: ?Open@CAsyncMonikerFile@@UEAAHPEB_WPEAUIUnknown@@PEAVCFileException@@@Z
// Retail RVA 0x2436a0 (mfc140u):
//   if (!pUnknown) return Open(lpszURL, pError);                  ; direct call 0x2434f0
//   IPTR(IBindHost) pBindHost; IPTR(IServiceProvider) pSP;
//   hr = (pSP = pUnknown);                                        ; QI IID_IServiceProvider, 0x243a80
//   if (SUCCEEDED(hr) && pSP) pBindHost = BindHostFromServiceProvider(pSP);   ; 0x243088
//   if (!pBindHost) pBindHost = BindHostFromUnknown(pUnknown);    ; 0x2430bc on pUnknown
//   if (pBindHost) return this->Open(lpszURL, pBindHost, pError); ; slot 35 (virtual)
//   return Open(lpszURL, pError);                                 ; direct call 0x2434f0
//   (~pSP, ~pBindHost)
// Same reference-count deviation as the IServiceProvider overload.
extern "C" int MS_ABI impl__Open_CAsyncMonikerFile__UEAAHPEB_WPEAUIUnknown__PEAVCFileException___Z(
    void* pThis, const wchar_t* lpszURL, IUnknown* pUnknown, CFileException* pError) {
    if (!pThis) return FALSE;
    if (!pUnknown) {
        return impl__Open_CAsyncMonikerFile__UEAAHPEB_WPEAVCFileException___Z(pThis, lpszURL, pError);
    }
    IServiceProvider* pSP = nullptr;
    IBindHost* pBindHost = nullptr;
    {
        void* p = nullptr;
        HRESULT hr = pUnknown->QueryInterface(kIID_IServiceProvider, &p);
        if (SUCCEEDED(hr)) pSP = static_cast<IServiceProvider*>(p);
        if (SUCCEEDED(hr) && pSP) pBindHost = BindHostFromServiceProvider(pSP);
    }
    if (!pBindHost) pBindHost = BindHostFromUnknown(pUnknown);
    int bResult;
    if (pBindHost) {
        bResult = AmfSlot<OpenUrlHostFn>(pThis, kSlot_Open_Url_BindHost)(pThis, lpszURL, pBindHost, pError);
    } else {
        bResult = impl__Open_CAsyncMonikerFile__UEAAHPEB_WPEAVCFileException___Z(pThis, lpszURL, pError);
    }
    if (pSP) pSP->Release();
    if (pBindHost) pBindHost->Release();
    return bResult;
}

// Symbol: ?Open@CAsyncMonikerFile@@UEAAHPEB_WPEAVCFileException@@@Z
// Retail RVA 0x2434f0 (mfc140u):
//   IPTR(IBindHost) pBindHost = CMonikerFile::CreateBindHost();   ; 0x2610a0 (exported)
//   bResult = this->Open(lpszURL, pBindHost, pError);             ; slot 35 (virtual)
//   ~pBindHost;
//   return bResult;
// Same NULL-bind-host deviation as the IMoniker overload (it would recurse).
extern "C" int MS_ABI impl__Open_CAsyncMonikerFile__UEAAHPEB_WPEAVCFileException___Z(
    void* pThis, const wchar_t* lpszURL, CFileException* pError) {
    if (!pThis) return FALSE;
    IBindHost* pBindHost = impl__CreateBindHost_CMonikerFile__KAPEAUIBindHost__XZ();
    if (!pBindHost) return FALSE;
    int bResult = AmfSlot<OpenUrlHostFn>(pThis, kSlot_Open_Url_BindHost)(pThis, lpszURL, pBindHost, pError);
    pBindHost->Release();
    return bResult;
}

// Symbol: ?PostBindToStream@CAsyncMonikerFile@@MEAAHPEAVCFileException@@@Z
// Retail RVA 0x2439c0 (mfc140u):
//   if (IsAsyncMoniker(m_Moniker) == S_OK) return TRUE;   ; +0x38 via 0x243d34;
//                                                         ; urlmon delay-load slot 0x1803e9530
//   return CMonikerFile::PostBindToStream(pError);        ; direct call 0x2610f0 (exported)
// Deviation: if urlmon.dll cannot be loaded (retail's delay-load helper would
// raise) the moniker is treated as synchronous and the base is called.
extern "C" int MS_ABI impl__PostBindToStream_CAsyncMonikerFile__MEAAHPEAVCFileException___Z(
    void* pThis, CFileException* pError) {
    if (!pThis) return FALSE;
    IsAsyncMonikerFn pfnIsAsyncMoniker = GetIsAsyncMoniker();
    if (pfnIsAsyncMoniker &&
        pfnIsAsyncMoniker(AmfMember<IMoniker*>(pThis, kOff_m_Moniker)) == S_OK) {
        return TRUE;
    }
    return impl__PostBindToStream_CMonikerFile__MEAAHPEAVCFileException___Z(pThis, pError);
}

// Symbol: ?Read@CAsyncMonikerFile@@UEAAIPEAXI@Z
// Retail RVA 0x243200 (mfc140u):
//   DWORD dwRead;                                    ; uninitialised stack slot
//   hr = _AfxStreamRead(m_lpStream, lpBuf, nCount, &dwRead);   ; 0x264f40 on +0x28:
//        nCount == 0       -> dwRead = 0, S_OK
//        !stream || !lpBuf -> E_INVALIDARG (dwRead untouched)
//        else              -> m_lpStream->Read(lpBuf, nCount, &dwRead)   (IStream slot 3)
//   if (hr != S_OK && hr != S_ASYNCHRONOUS && hr != E_PENDING)
//       _AfxThrowOleFileException(hr);               ; 0x264fe4 -- throws only if FAILED(hr)
//   return dwRead;
// m_bStopBindingReceived plays no part in retail's Read.  dwRead starts at 0
// here so an E_PENDING / S_ASYNCHRONOUS result that leaves it unset reads as
// "nothing yet" rather than stack garbage.  The stream is OpenMFC's slot
// (Stream(), +0x18), not retail's +0x28 -- see the file header.
extern "C" unsigned int MS_ABI impl__Read_CAsyncMonikerFile__UEAAIPEAXI_Z(
    void* pThis, void* lpBuf, unsigned int nCount) {
    if (!pThis) return 0;
    IStream* pStream = Stream(pThis);
    DWORD dwRead = 0;
    HRESULT hr;
    if (nCount == 0) {
        hr = S_OK;
    } else if (!pStream || !lpBuf) {
        hr = E_INVALIDARG;
    } else {
        hr = pStream->Read(lpBuf, nCount, &dwRead);
    }
    if (hr != S_OK && hr != S_ASYNCHRONOUS && hr != E_PENDING) {
        ThrowOleFileException(hr);
    }
    return dwRead;
}
