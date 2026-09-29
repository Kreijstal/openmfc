// CMultiPageDHtmlDialog — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// Retail layout (afxdhtml.h; read from the mfc140u.dll exports cited below):
//   class CMultiPageDHtmlDialog : public CDHtmlDialog {
//       ...                                      // CDHtmlDialog occupies [0, 0x298)
//       const DHtmlEventMapEntry* m_pCurrentMap; // +0x298
//   };                                           // sizeof == 0x2a0
// CDHtmlDialog is itself `: public CDialog, public CDHtmlEventSink`, and its
// CDHtmlEventSink subobject (the second vptr) sits at +0x130.  Evidence: every
// CMultiPageDHtmlDialog ctor (mfc140u 0x2152b0 / 0x215350 / 0x215390) stores
// NULL to +0x298 after the CDHtmlDialog base ctor returns, then installs the
// primary vftable (mfc140u 0x180326a58) at +0 and the CDHtmlEventSink vftable
// (mfc140u 0x180326a00) at +0x130; the scalar-deleting dtor (primary vftable
// slot 1, mfc140u 0x2152f0) passes the size 0x2a0 in %edx on its flag-4
// delete path (`mov $0x2a0,%edx`; the flag-1-only path calls the CRT `free`
// import instead); and the class's CRuntimeClass descriptor
// (detail/COleControlModuleSupport.h) records m_nObjectSize 672.  Retail
// CDHtmlDialog's own scalar-deleting dtor (mfc140u 0x211190) uses 0x298, and
// its descriptor records 664 (0x298).
//
// include/openmfc does NOT declare CMultiPageDHtmlDialog, so every thunk here
// takes void* and the offsets are pinned below.  OpenMFC's own CDHtmlDialog
// (include/openmfc/afxwin.h) is a mingw C++ class whose layout is unrelated to
// retail's (it measures 0x2a8 bytes, not 0x298, and has no CDHtmlEventSink
// subobject at +0x130), so no retail CDHtmlDialog offset is dereferenced here.

#include "detail/ManualSmallStubImplementationsSupport.h"

namespace {

// Retail offsets, from the disassembly cited in the header comment.
constexpr std::size_t kSinkSubobjectOffset = 0x130;  // CDHtmlEventSink vptr
constexpr std::size_t kCurrentMapOffset    = 0x298;  // m_pCurrentMap
constexpr std::size_t kRetailObjectSize    = 0x2a0;  // sizeof(CMultiPageDHtmlDialog)

// GetDHtmlEventMap is entered with `this` pointing at the CDHtmlEventSink
// subobject (see below) and reads +0x168 from it; that must be m_pCurrentMap.
static_assert(kCurrentMapOffset - kSinkSubobjectOffset == 0x168,
              "m_pCurrentMap is +0x168 from the CDHtmlEventSink subobject");
static_assert(kCurrentMapOffset + sizeof(void*) == kRetailObjectSize,
              "m_pCurrentMap is the last (only) member CMultiPageDHtmlDialog adds");

inline const void** CurrentMapSlot(void* pFullObject) {
    return reinterpret_cast<const void**>(
        static_cast<unsigned char*>(pFullObject) + kCurrentMapOffset);
}

} // namespace

// The three constructors are still NOT complete: retail first runs the
// matching CDHtmlDialog base ctor (0x2110b0 / 0x2111e0 / 0x2112e0, mfc140u),
// then stores `m_pCurrentMap = NULL` (`movq $0x0,0x298(%rbx)`) and then
// installs the primary vftable at +0 and the CDHtmlEventSink vftable at
// +0x130 (instruction order in each of 0x2152b0 / 0x215350 / 0x215390,
// mfc140u).  Only the m_pCurrentMap store is reproduced.  The base-ctor call is not: OpenMFC's
// CDHtmlDialog is 8 bytes larger than retail's (0x2a8 vs 0x298), so
// placement-constructing it here would overrun a 0x2a0-byte client
// allocation, and there is no MSVC-layout vftable to install.  The store is
// kept because GetDHtmlEventMap below returns this member.
// Symbol: ??0CMultiPageDHtmlDialog@@QEAA@IIPEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CMultiPageDHtmlDialog__QEAA_IIPEAVCWnd___Z(
    void* pThis, unsigned int nIDTemplate, unsigned int nHtmlResID, void* pParentWnd) {
    (void)nIDTemplate;
    (void)nHtmlResID;
    (void)pParentWnd;
    *CurrentMapSlot(pThis) = nullptr;
    return pThis;
}
// Symbol: ??0CMultiPageDHtmlDialog@@QEAA@PEB_W0PEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CMultiPageDHtmlDialog__QEAA_PEB_W0PEAVCWnd___Z(
    void* pThis, const wchar_t* lpszTemplateName, const wchar_t* szHtmlResID, void* pParentWnd) {
    (void)lpszTemplateName;
    (void)szHtmlResID;
    (void)pParentWnd;
    *CurrentMapSlot(pThis) = nullptr;
    return pThis;
}
// Symbol: ??0CMultiPageDHtmlDialog@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMultiPageDHtmlDialog__QEAA_XZ(void* pThis) {
    *CurrentMapSlot(pThis) = nullptr;
    return pThis;
}

// CMultiPageDHtmlDialog::~CMultiPageDHtmlDialog() -- mfc140u 0x2153d0.  Retail:
//     vptr (+0)       = CMultiPageDHtmlDialog::`vftable'           ; 0x180326a58
//     vptr (+0x130)   = CMultiPageDHtmlDialog::`vftable'{for CDHtmlEventSink} ; 0x180326a00
//     jmp CDHtmlDialog::~CDHtmlDialog                             ; 0x2113e0 (tail call)
// STUB: the constructors above never construct the CDHtmlDialog base, so
// running the base destructor here would tear down an object that was never
// built (and the existing CDHtmlDialog dtor thunk calls `pThis->~CDHtmlDialog()`
// through the mingw vtable, which a client-derived MSVC object does not have).
// This must be implemented together with the constructors.
// Symbol: ??1CMultiPageDHtmlDialog@@UEAA@XZ
extern "C" void MS_ABI impl___1CMultiPageDHtmlDialog__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// CMultiPageDHtmlDialog::GetDHtmlEventMap() -- mfc140u 0x2153f0, complete body:
//     mov 0x168(%rcx),%rax ; ret
// GetDHtmlEventMap is a CDHtmlEventSink virtual, so under the MSVC ABI it is
// entered with `this` already adjusted to the CDHtmlEventSink subobject: the
// CDHtmlEventSink vftable (mfc140u 0x180326a00) holds 0x2153f0 directly in
// slot 7 (+0x38), with no adjustor thunk.  pThis here is therefore
// (full object + 0x130), and +0x168 from it is m_pCurrentMap (+0x298).
// Callers inside OpenMFC must pass the sink-subobject pointer, not the
// full-object pointer.
// Symbol: ?GetDHtmlEventMap@CMultiPageDHtmlDialog@@MEAAPEBUDHtmlEventMapEntry@@XZ
extern "C" const void* MS_ABI impl__GetDHtmlEventMap_CMultiPageDHtmlDialog__MEAAPEBUDHtmlEventMapEntry__XZ(
    void* pThis) {
    return *CurrentMapSlot(static_cast<unsigned char*>(pThis) - kSinkSubobjectOffset);
}

// CMultiPageDHtmlDialog::GetEventMapForUrl(LPCTSTR) -- the export (ordinal
// 5337) resolves in mfc140u's export table to RVA 0x71e0, the shared
// ICF-folded `xor %eax,%eax ; ret`; the RVA symbol map keeps one name per RVA
// and files 0x71e0 under another symbol, so the export has no map entry.
// Cross-checked against the vftable:
// GetEventMapForUrl is the only virtual CMultiPageDHtmlDialog introduces
// (afxdhtml.h), CDHtmlDialog's primary vftable (mfc140u 0x180326e88) has 121
// slots (0..120), so it lands in slot 121 (+0x3c8) of
// CMultiPageDHtmlDialog's primary vftable (mfc140u 0x180326a58) -- which holds
// 0x71e0.  OnNavigateComplete (below) calls exactly vtable +0x3c8 with the URL
// and stores the result to m_pCurrentMap, confirming the slot.  The base
// implementation returns NULL; DECLARE_DHTML_URL_EVENT_MAP overrides it.
// This is the complete retail body.
// Symbol: ?GetEventMapForUrl@CMultiPageDHtmlDialog@@MEAAPEBUDHtmlEventMapEntry@@PEB_W@Z
extern "C" const void* MS_ABI impl__GetEventMapForUrl_CMultiPageDHtmlDialog__MEAAPEBUDHtmlEventMapEntry__PEB_W_Z(
    void* pThis, const wchar_t* szUrl) {
    (void)pThis;
    (void)szUrl;
    return nullptr;
}

// CMultiPageDHtmlDialog::OnNavigateComplete(LPDISPATCH, LPCTSTR) -- not in
// the RVA symbol map, but its export (ordinal 10540) resolves in mfc140u's
// export table to 0x215400, which is also slot 103 (+0x338) of the primary vftable
// (mfc140u 0x180326a58), the slot CDHtmlDialog's vftable fills with its own
// OnNavigateComplete (0x211700, between OnBeforeNavigate and
// OnDocumentComplete).  Body at mfc140u 0x215400, transcribed:
//     if (pDisp != m_pBrowserApp /* +0x158 */) return;
//     CString strUrl(szUrl);                                  ; 0xdcb0
//     if (_wcsicmp(strUrl.Left(4), L"res:") == 0) {          ; Left 0x128f0, IAT 0x1802c7788, literal 0x34c768
//         LPCWSTR p = wcsrchr(strUrl, L'/');                 ; IAT 0x1802c73e8 (inline ReverseFind)
//         if (p && (p - strUrl) >= 0) {
//             int n = p - strUrl;
//             strUrl = strUrl.Mid(n + 1, strUrl.GetLength() - (n + 1));   ; Mid 0x12a80, assign 0xde30
//         }
//     }
//     m_pCurrentMap /* +0x298 */ = this->GetEventMapForUrl(strUrl);  ; virtual, vtable +0x3c8 (slot 121)
//     CDHtmlDialog::OnNavigateComplete(pDisp, szUrl);         ; 0x211700, direct call, ORIGINAL szUrl
// STUB: m_pBrowserApp (+0x158) does not exist in OpenMFC's CDHtmlDialog
// layout, and the objects are never base-constructed (see the ctors), so
// +0x158 of a client object is never initialised and the pDisp test would
// compare against garbage; the GetEventMapForUrl call must dispatch through
// vftable +0x3c8 to reach the client's DECLARE_DHTML_URL_EVENT_MAP override,
// but the ctors above install no vftable, so only a client-DERIVED object
// (whose own ctor stores its MSVC vftable) would have a usable +0 -- a
// directly instantiated CMultiPageDHtmlDialog would not; and the only base thunk,
// impl__OnNavigateComplete_CDHtmlDialog__UEAAXPEAUIDispatch__PEB_W_Z,
// re-dispatches virtually through the mingw vtable rather than running the
// base body.  Calling GetEventMapForUrl non-virtually would always store NULL
// and silently drop the client's map, so this is left a no-op.
// Symbol: ?OnNavigateComplete@CMultiPageDHtmlDialog@@MEAAXPEAUIDispatch@@PEB_W@Z
extern "C" void MS_ABI impl__OnNavigateComplete_CMultiPageDHtmlDialog__MEAAXPEAUIDispatch__PEB_W_Z(
    void* pThis, IDispatch* pDisp, const wchar_t* szUrl) {
    (void)pThis;
    (void)pDisp;
    (void)szUrl;
}
