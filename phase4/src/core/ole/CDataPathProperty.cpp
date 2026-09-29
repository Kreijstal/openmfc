// CDataPathProperty -- OpenMFC implementation.
//
// =============================================================================
// Every body below is transcribed from the retail export in mfc140u.dll.  None
// of the five exports (the four Open overloads and ResetData) is in
// mfc140u_rva_symbols.json; their RVAs were read straight out of the mfc140u
// export table by ordinal (ordrva.py, Open ordinals 11639..11642, ResetData
// 12503) and each quoted RVA is that function's ENTRY in mfc140u.  The four
// Open bodies are instruction-for-instruction identical to the mfc140.dll
// (ANSI) twins at 0x1ed1a0 / 0x1ed1d0 / 0x1ed230 / 0x1ed250 (mfc140), apart
// from the rip-relative displacements (direct call/jmp targets, the strlen vs
// wcslen IAT slot and the CFG dispatch slot).
//
// CDataPathProperty is not declared in OpenMFC's public headers (the class is
// constructed inline by client code -- its ctors live in afxctl.inl -- so every
// object this DLL ever sees was laid out and vtabled by the client's MSVC
// compiler).  Every export here therefore takes a `void* pThis` and views the
// object through the local shadow struct below, the same arrangement as
// PX_CDataPathProperty in core/ole/CPropExchange.cpp and the base-class
// exports in core/ole/CAsyncMonikerFile.cpp.
//
// Retail layout (afxctl.h:453 `class CDataPathProperty: public
// CAsyncMonikerFile` declares, privately at afxctl.h:510-511, `COleControl*
// m_pControl; CString m_strPath;` as its only data members):
//   0x00..0x5f  CAsyncMonikerFile base (sizeof 0x60, see CAsyncMonikerFile.cpp)
//   0x60        m_pControl    -- read by Open(CFileException*) `mov 0x60(%rcx)`,
//                               written by both COleControl* overloads
//                               `mov %rdx,0x60(%rcx)` / `mov %r8,0x60(%rcx)`
//   0x68        m_strPath     -- read by Open(CFileException*) `mov 0x68(%rcx),%rdx`
//                               (the CString's m_pszData, i.e. its LPCTSTR), and
//                               SetString target `lea 0x68(%rdi),%rcx`
//   sizeof == 0x70 (CRuntimeClass m_nObjectSize, see CPropExchange.cpp)
//
// Virtual dispatch: three of the four overloads end in a vcall through byte
// offset 0x160.  The two LPCTSTR overloads (non-volatile this in %rdi, pError
// in %rsi) do
//   mov (%rdi),%rax ; mov %rsi,%rdx ; mov %rdi,%rcx ; mov 0x160(%rax),%rax ;
//   <restore %rbx/%rsi, add $0x20,%rsp, pop %rdi> ;
//   rex.W jmp *<__guard_dispatch_icall_fptr>
// and the leaf Open(COleControl*, CFileException*) does
//   mov (%rcx),%rax ; mov %rdx,0x60(%rcx) ; mov %r8,%rdx ;
//   mov 0x160(%rax),%rax ; rex.W jmp *<__guard_dispatch_icall_fptr>
// i.e. a CFG-checked tail call of vtable byte offset 0x160 = slot 44 with
// (this, pError).  Slot 44 is `Open(CFileException* pError)`: the slot number
// and that identification are established in core/ole/CPropExchange.cpp
// (kSlotDataPathOpen_pError, cross-checked there with cl.exe
// /d1reportSingleClassLayout: slots 44..46 are CDataPathProperty's three new
// Open overloads, the last-declared Open(CFileException*) at 44, and 47 is
// ResetData).  The indirect jump's slot at mfc140u 0x1802c7b30 is not an
// import (iatu.py); it is the GuardCFDispatchFunctionPointer recorded in
// mfc140u's IMAGE_LOAD_CONFIG_DIRECTORY (read at load-config +0x78), i.e.
// MSVC's Control Flow Guard __guard_dispatch_icall_fptr, which validates and
// jumps to %rax.  It is not modelled -- the call goes straight to the vtable
// entry.
//
// No NULL-this guards: retail has none on any of these bodies, and none is
// added here.
// =============================================================================

// Deliberately header-light: pulling in openmfc/afxole.h (for COleControl)
// makes checkfile.sh's link audit fail (observed: new undefined references to
// CWnd::FromHandle, AfxGetThread, the CCmdTarget/CWnd/CWinApp/CWinThread
// class* statics, sized operator delete and the cxxabi typeinfo vtables),
// and none of it is needed -- the two object members touched are a pointer
// and a CString's m_pszData, and the path is assigned through the exported
// SetString thunk retail itself calls.  (openmfc/afxstr.h on its own does
// pass the audit; it is simply not needed.)
#include <cstddef>
#include <objbase.h>   // IUnknown
#include <cwchar>      // wcslen

class COleControl;
class CFileException;

#ifndef MS_ABI
  #ifdef __GNUC__
    #define MS_ABI __attribute__((ms_abi))
  #else
    #define MS_ABI
  #endif
#endif

// -----------------------------------------------------------------------------
// Thunks from other translation units (definition checked in the tree).
// -----------------------------------------------------------------------------
// core/ole/CAsyncMonikerFile.cpp:924 -- CAsyncMonikerFile::Open(LPCTSTR,
// IUnknown*, CFileException*), retail RVA 0x2436a0 (mfc140u).  The definition
// there takes exactly (void* pThis, const wchar_t*, IUnknown*, CFileException*).
extern "C" int MS_ABI impl__Open_CAsyncMonikerFile__UEAAHPEB_WPEAUIUnknown__PEAVCFileException___Z(
    void* pThis, const wchar_t* lpszURL, IUnknown* pUnknown, CFileException* pError);
// core/collections/Thunks.cpp:1207 -- ATL::CSimpleStringT<wchar_t,1>::
// SetString(PCXSTR, int), export ordinal 13619 at RVA 0x2e30 (mfc140u): the
// very function the retail SetPath sequence calls (`call 0x180002e30`).
extern "C" void MS_ABI impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_WH_Z(
    void* pThis, const wchar_t* psz, int n);

namespace {

// CDataPathProperty as retail lays it out (see the file header).  Only the
// vfptr and the two CDataPathProperty members are viewed; the base is opaque.
struct DPP_CDataPathProperty {
    void**        vptr;          // 0x00  client MSVC vftable
    unsigned char _base[0x58];   // 0x08..0x5f  CFile/COleStreamFile/CMonikerFile/CAsyncMonikerFile
    COleControl*  m_pControl;    // 0x60
    wchar_t*      m_strPath;     // 0x68  CString -- a single m_pszData pointer
};
static_assert(offsetof(DPP_CDataPathProperty, m_pControl) == 0x60, "CDataPathProperty::m_pControl");
static_assert(offsetof(DPP_CDataPathProperty, m_strPath) == 0x68, "CDataPathProperty::m_strPath");
static_assert(sizeof(DPP_CDataPathProperty) == 0x70, "CDataPathProperty is 0x70 bytes (CRuntimeClass m_nObjectSize)");

// Open(CFileException*) reads `mov 0x1d8(%rax),%r8` off m_pControl.
// include/openmfc/afxole.h:2053 records COleControl::m_pClientSite
// (IOleClientSite*) at 0x1D8, and core/ole/CPropExchange.cpp static_asserts
// offsetof(COleControl, m_pClientSite) == 0x1d8; that header is not included
// here (see above), so the member is read by that byte offset.
constexpr std::size_t kOff_COleControl_m_pClientSite = 0x1d8;

inline IUnknown* clientSiteOf(COleControl* pControl) {
    return *reinterpret_cast<IUnknown* const*>(
        reinterpret_cast<const unsigned char*>(pControl) + kOff_COleControl_m_pClientSite);
}

inline DPP_CDataPathProperty* dpp(void* pThis) {
    return static_cast<DPP_CDataPathProperty*>(pThis);
}

// vtable slot 44 (byte offset 0x160) = CDataPathProperty::Open(CFileException*).
enum { kSlot_Open_pError = 44 };
using OpenErrorFn = int (MS_ABI*)(void* pThis, CFileException* pError);

inline int callOpenError(void* pThis, CFileException* pError) {
    return reinterpret_cast<OpenErrorFn>(dpp(pThis)->vptr[kSlot_Open_pError])(pThis, pError);
}

// SetPath(lpszPath) as the three LPCTSTR/COleControl* overloads inline it:
//   test %rdx,%rdx ; jne -> call *wcslen  (mfc140u IAT 0x1802c7748, iatu.py)
//                  ; je  -> xor %eax,%eax
//   lea 0x68(%rdi),%rcx ; mov %eax,%r8d ; mov %rbx,%rdx ; call 0x180002e30
// i.e. m_strPath.SetString(lpszPath, lpszPath ? wcslen(lpszPath) : 0).  The
// callee 0x2e30 (mfc140u) is absent from mfc140u_rva_symbols.json but the
// mfc140u export table maps ordinal 13619,
// ?SetString@?$CSimpleStringT@_W$00@ATL@@QEAAXPEB_WH@Z, to exactly that RVA
// (ordrva.py); the ANSI twin calls 0x2e60 (mfc140), which the mfc140 map names
// ?SetString@?$CSimpleStringT@D$00@ATL@@QEAAXPEBDH@Z.  So the same call is made
// here through that export's thunk, on the CString at +0x68.
// Caveat inherited from that thunk (core/collections/Thunks.cpp:1207, not this
// file): it does Empty() then AppendPszN(), so unlike ATL's SetString it does
// not survive a source pointer aliasing the CString's own buffer.
inline void setPath(void* pThis, const wchar_t* lpszPath) {
    const int nLength = lpszPath != nullptr ? static_cast<int>(wcslen(lpszPath)) : 0;
    impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_WH_Z(&dpp(pThis)->m_strPath, lpszPath, nLength);
}

}  // namespace

// Symbol: ?Open@CDataPathProperty@@UEAAHPEAVCFileException@@@Z
// Retail RVA 0x1ef430 (mfc140u), complete body:
//   mov 0x60(%rcx),%rax ; test ; je    ; m_pControl
//   mov 0x1d8(%rax),%r8                 ; m_pControl->m_pClientSite  (else r8 = 0)
//   mov %rdx,%r9                        ; pError
//   mov 0x68(%rcx),%rdx                 ; (LPCTSTR)m_strPath
//   jmp 0x1802436a0                     ; CAsyncMonikerFile::Open(LPCTSTR, IUnknown*,
//                                       ;   CFileException*), non-virtual tail call
// i.e.
//   return CAsyncMonikerFile::Open(m_strPath,
//       m_pControl ? m_pControl->m_pClientSite : NULL, pError);
// (the IOleClientSite* travels as the IUnknown* argument).
extern "C" int MS_ABI impl__Open_CDataPathProperty__UEAAHPEAVCFileException___Z(
    void* pThis, CFileException* pError) {
    DPP_CDataPathProperty* self = dpp(pThis);
    IUnknown* pUnk = self->m_pControl != nullptr ? clientSiteOf(self->m_pControl) : nullptr;
    return impl__Open_CAsyncMonikerFile__UEAAHPEB_WPEAUIUnknown__PEAVCFileException___Z(
        pThis, self->m_strPath, pUnk, pError);
}

// Symbol: ?Open@CDataPathProperty@@UEAAHPEAVCOleControl@@PEAVCFileException@@@Z
// Retail RVA 0x1ef4c0 (mfc140u), complete body:
//   mov (%rcx),%rax ; mov %rdx,0x60(%rcx)          ; m_pControl = pControl
//   mov %r8,%rdx ; mov 0x160(%rax),%rax ; jmp       ; return this->Open(pError)  (slot 44)
extern "C" int MS_ABI impl__Open_CDataPathProperty__UEAAHPEAVCOleControl__PEAVCFileException___Z(
    void* pThis, COleControl* pControl, CFileException* pError) {
    dpp(pThis)->m_pControl = pControl;
    return callOpenError(pThis, pError);
}

// Symbol: ?Open@CDataPathProperty@@UEAAHPEB_WPEAVCFileException@@@Z
// Retail RVA 0x1ef460 (mfc140u):
//   SetPath(lpszPath);          ; inlined, see setPath() above
//   return this->Open(pError);  ; vtable slot 44, CFG-checked tail call
// m_pControl is not touched.
extern "C" int MS_ABI impl__Open_CDataPathProperty__UEAAHPEB_WPEAVCFileException___Z(
    void* pThis, const wchar_t* lpszPath, CFileException* pError) {
    setPath(pThis, lpszPath);
    return callOpenError(pThis, pError);
}

// Symbol: ?Open@CDataPathProperty@@UEAAHPEB_WPEAVCOleControl@@PEAVCFileException@@@Z
// Retail RVA 0x1ef4e0 (mfc140u):
//   m_pControl = pControl;      ; mov %r8,0x60(%rcx) -- stored FIRST, before SetPath
//   SetPath(lpszPath);          ; inlined, see setPath() above
//   return this->Open(pError);  ; vtable slot 44, CFG-checked tail call
extern "C" int MS_ABI impl__Open_CDataPathProperty__UEAAHPEB_WPEAVCOleControl__PEAVCFileException___Z(
    void* pThis, const wchar_t* lpszPath, COleControl* pControl, CFileException* pError) {
    dpp(pThis)->m_pControl = pControl;
    setPath(pThis, lpszPath);
    return callOpenError(pThis, pError);
}

// Symbol: ?ResetData@CDataPathProperty@@UEAAXXZ
// Retail: the mfc140u export table maps this symbol (ordinal 12503) to RVA
// 0x27d0 (mfc140u), whose entire body is `ret $0x0` -- an identical-COMDAT-
// folded empty function shared with other no-op members (the mfc140u symbol
// map names that RVA ?AddDockSite@CFrameWndEx@@QEAAXXZ).  The base-class
// ResetData genuinely does nothing ("Derived classes should override this",
// afxctl.h); the empty body below IS the transcription, not a placeholder.
extern "C" void MS_ABI impl__ResetData_CDataPathProperty__UEAAXXZ(void* pThis) {
    (void)pThis;
}
