// CMFCFilterChunkValueImpl — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <new>
#include <filter.h>
#include <propidl.h>

// ===========================================================================
// CMFCFilterChunkValueImpl -- the search-filter chunk/value pair of the
// document-handler support (retail atlmfc/include/afxwin.h:6324,
// `class CMFCFilterChunkValueImpl : public ATL::IFilterChunkValue`).
//
// Every body below was transcribed from the retail mfc140u.dll disassembly
// (the method described in the header of core/ole/COleControl.cpp).  Each
// cites the RVA of the function ENTRY in mfc140u.  Ten of the exports (nine
// distinct entries -- SetIntValue and SetLongValue share one) are not in the
// mfc140u RVA map used by `disas.py --u` (SetChunk, SetTextValue, both
// SetBoolValue overloads, SetIntValue, SetLongValue, SetDwordValue,
// SetInt64Value, SetSystemTimeValue, SetFileTimeValue); their entries were
// recovered from the export table by ordinal (urva.py) and cross-checked
// against the class vftable (below).  NOTE: for this class the two retail
// images are NOT byte-identical -- the ANSI mfc140.dll CopyFrom compares vt
// against VT_LPSTR (0x1e) where mfc140u compares VT_LPWSTR (0x1f) -- so every
// body here was read from mfc140u, not from the default (ANSI) image.
// Every Win32/CRT import was resolved from its IAT slot with iatu.py:
// memset, CoTaskMemAlloc, PropVariantCopy, CoTaskMemFree, GetUserDefaultLCID,
// wcslen, SystemTimeToFileTime.
//
// Object layout.  The class is not declared in the OpenMFC public headers, so
// the client allocates it with the retail layout and this file is the only
// place that layout lives.  Member names and order are quoted from the
// shipping afxwin.h (lines 6536-6539):
//     BOOL        m_fIsValid;
//     STAT_CHUNK  m_chunk;
//     PROPVARIANT m_propVariant;
//     CString     m_strValue;
// and every offset is read out of the retail bodies: Clear (0x21dfe0) zeroes
// +0x08 (dword), memsets 0x40 bytes at +0x10 and 0x18 bytes at +0x50, and
// empties the CString at +0x68; the constructor (0x21df00) stores the
// CString's nil buffer at +0x68; the scalar deleting destructor (0x21df50)
// frees with `mov $0x70,%edx` (sized operator delete), pinning sizeof == 0x70.
// The STAT_CHUNK field offsets are pinned by SetChunk's stores (see there).
//
// Virtual dispatch.  CMFCFilterChunkValueImpl's vftable is not exported, so
// the constructor below installs an MSVC-layout vtable authored in this file
// (g_FilterChunkValueVtbl) with the same 20 slots, in the same order, as the
// retail vftable 0x180328b48 (mfc140u; vtdump_u.py):
//      0 scalar deleting dtor 0x21df50   10 SetTextValue           0x21e0c0
//      1 Clear                0x21dfe0   11 SetBoolValue(VARIANT_BOOL) 0x21e1b0
//      2 IsValid              0x21e5c0   12 SetBoolValue(BOOL)     0x21e150
//      3 GetValue             0x21e020   13 SetIntValue            0x21e210
//      4 GetValueNoAlloc      0x21e5a0   14 SetLongValue           0x21e210
//      5 GetString            0x21e590   15 SetDwordValue          0x21e270
//      6 CopyChunk            0x21e090   16 SetInt64Value          0x21e2d0
//      7 GetChunkType         0x1094e0   17 SetSystemTimeValue     0x21e330
//      8 GetChunkGUID         0x21e580   18 SetFileTimeValue       0x21e3b0
//      9 GetChunkPID          0x21e570   19 CopyFrom               0x21e480
// (slots 13/14 are one COMDAT-folded body.  Slots 8/9 resolve to the
// exported CPropertySet::GetList/GetCount, i.e. identical one-instruction
// getters folded together; slot 7's body sits at 0x1094e0, outside this
// class's code, and has no export name in the map -- presumably folded the
// same way, not verified.)
// The slot order is ATL::IFilterChunkValue's declaration order
// (atlmfc/include/atlhandler.h:49) with MSVC's reversed overload grouping for
// SetBoolValue.  Retail bodies DO dispatch through `this`'s own vtable
// (SetChunk/CopyFrom call Clear through slot 1, SetBoolValue(BOOL) calls
// slot 11), and so do the bodies here, so a client-derived override is
// honoured exactly as in retail.
//
// Deviations from retail, marked at each site:
//  (1) No RTTI Complete Object Locator.  The retail vftable is preceded by a
//      COL (vftable[-1] -> 0x365cb8, mfc140u, naming
//      `.?AVCMFCFilterChunkValueImpl@@`); g_FilterChunkValueVtbl has none,
//      so a client typeid / dynamic_cast on an object built here would read
//      garbage.
//  (2) The destructor does not re-install the ATL::IFilterChunkValue base
//      vftable (0x180328c20, mfc140u: every slot but the destructor is
//      _purecall) as its last step; the object keeps OpenMFC's vtable.  Only
//      observable by calling a virtual on a destroyed object.
//  (3) m_strValue is manipulated with OpenMFC's inline CString
//      (include/openmfc/afxstr.h), whose CStringData is byte-compatible with
//      ATL's but whose Empty / operator= differ from ATL's CSimpleStringT in
//      the locked-buffer and foreign-manager edge cases (noted at the sites).
// ===========================================================================

namespace {

struct S_FilterChunkValue {
    void*       vfptr;           // +0x00
    BOOL        m_fIsValid;      // +0x08
    STAT_CHUNK  m_chunk;         // +0x10
    PROPVARIANT m_propVariant;   // +0x50
    CString     m_strValue;      // +0x68
};
static_assert(sizeof(STAT_CHUNK) == 0x40, "Clear memsets 0x40 bytes of m_chunk; CopyChunk copies 4 x 16 bytes");
static_assert(sizeof(PROPVARIANT) == 0x18, "Clear memsets 0x18 bytes of m_propVariant; GetValue CoTaskMemAllocs 0x18");
static_assert(sizeof(CString) == 8, "m_strValue is one pointer (the ctor stores the nil buffer at +0x68)");
static_assert(offsetof(S_FilterChunkValue, m_fIsValid) == 0x08, "Clear: movl $0,0x8(%rcx)");
static_assert(offsetof(S_FilterChunkValue, m_chunk) == 0x10, "Clear: memset(this+0x10, 0, 0x40)");
static_assert(offsetof(S_FilterChunkValue, m_propVariant) == 0x50, "Clear: memset(this+0x50, 0, 0x18)");
static_assert(offsetof(S_FilterChunkValue, m_strValue) == 0x68, "ctor: mov %rax,0x68(%rbx)");
static_assert(sizeof(S_FilterChunkValue) == 0x70, "scalar deleting dtor 0x21df50: mov $0x70,%edx");
// STAT_CHUNK fields as SetChunk (0x21e410, mfc140u) addresses them, relative
// to the object (+0x10 for m_chunk):
static_assert(offsetof(STAT_CHUNK, breakType) == 0x04, "SetChunk: chunkBreakType -> +0x14");
static_assert(offsetof(STAT_CHUNK, flags) == 0x08, "SetChunk: chunkType -> +0x18");
static_assert(offsetof(STAT_CHUNK, locale) == 0x0c, "SetChunk: locale -> +0x1c");
static_assert(offsetof(STAT_CHUNK, attribute) == 0x10, "SetChunk: pkey->fmtid -> +0x20");
static_assert(offsetof(FULLPROPSPEC, psProperty) == 0x10, "SetChunk: ulKind -> +0x30");
static_assert(offsetof(PROPSPEC, propid) == 0x08, "SetChunk: pkey->pid -> +0x38");
static_assert(offsetof(STAT_CHUNK, cwcStartSource) == 0x34, "SetChunk: cwcStartSource -> +0x44");
static_assert(offsetof(STAT_CHUNK, cwcLenSource) == 0x38, "SetChunk: cwcLenSource -> +0x48");
static_assert(offsetof(PROPVARIANT, boolVal) == 0x08, "setters store the value at +0x58");
static_assert(offsetof(PROPERTYKEY, pid) == 0x10, "SetChunk: mov 0x10(%rbx),%eax");

inline S_FilterChunkValue* Self(void* pThis) { return static_cast<S_FilterChunkValue*>(pThis); }

// Virtual dispatch through an object's (MSVC-layout) vtable.
inline void** Vtbl(void* p) { return *static_cast<void***>(p); }

typedef void (MS_ABI *PFN_Clear)(void*);                                    // slot 1
typedef BOOL (MS_ABI *PFN_IsValid)(const void*);                            // slot 2
typedef PROPVARIANT* (MS_ABI *PFN_GetValueNoAlloc)(void*, PROPVARIANT*);    // slot 4 (hidden return buffer in RDX)
typedef CString* (MS_ABI *PFN_GetString)(void*);                            // slot 5
typedef HRESULT (MS_ABI *PFN_CopyChunk)(void*, STAT_CHUNK*);                // slot 6
typedef HRESULT (MS_ABI *PFN_SetBoolValueVB)(void*, const PROPERTYKEY*, VARIANT_BOOL, CHUNKSTATE,
                                             LCID, DWORD, DWORD, CHUNK_BREAKTYPE);  // slot 11

inline void VirtualClear(void* p) { reinterpret_cast<PFN_Clear>(Vtbl(p)[1])(p); }

} // namespace

// ---------------------------------------------------------------------------
// Thunk declarations (definitions below, in marker order).
// ---------------------------------------------------------------------------
extern "C" void MS_ABI impl___1CMFCFilterChunkValueImpl__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__Clear_CMFCFilterChunkValueImpl__UEAAXXZ(void* pThis);
extern "C" HRESULT MS_ABI impl__CopyChunk_CMFCFilterChunkValueImpl__UEAAJPEAUtagSTAT_CHUNK___Z(
    void* pThis, STAT_CHUNK* pStatChunk);
extern "C" void MS_ABI impl__CopyFrom_CMFCFilterChunkValueImpl__UEAAXPEAUIFilterChunkValue_ATL___Z(
    void* pThis, void* pValue);
extern "C" HRESULT MS_ABI impl__GetValue_CMFCFilterChunkValueImpl__UEAAJPEAPEAUtagPROPVARIANT___Z(
    void* pThis, PROPVARIANT** ppPropVariant);
extern "C" HRESULT MS_ABI impl__SetBoolValue_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey__FW4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z(
    void* pThis, const PROPERTYKEY* pkey, VARIANT_BOOL bVal, CHUNKSTATE chunkType, LCID locale,
    DWORD cwcLenSource, DWORD cwcStartSource, CHUNK_BREAKTYPE chunkBreakType);
extern "C" HRESULT MS_ABI impl__SetBoolValue_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey__HW4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z(
    void* pThis, const PROPERTYKEY* pkey, BOOL bVal, CHUNKSTATE chunkType, LCID locale,
    DWORD cwcLenSource, DWORD cwcStartSource, CHUNK_BREAKTYPE chunkBreakType);
extern "C" HRESULT MS_ABI impl__SetChunk_CMFCFilterChunkValueImpl__IEAAJAEBU_tagpropertykey__W4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z(
    void* pThis, const PROPERTYKEY* pkey, CHUNKSTATE chunkType, LCID locale,
    DWORD cwcLenSource, DWORD cwcStartSource, CHUNK_BREAKTYPE chunkBreakType);
extern "C" HRESULT MS_ABI impl__SetDwordValue_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey__KW4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z(
    void* pThis, const PROPERTYKEY* pkey, DWORD dwVal, CHUNKSTATE chunkType, LCID locale,
    DWORD cwcLenSource, DWORD cwcStartSource, CHUNK_BREAKTYPE chunkBreakType);
extern "C" HRESULT MS_ABI impl__SetFileTimeValue_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey__U_FILETIME__W4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z(
    void* pThis, const PROPERTYKEY* pkey, unsigned long long dtVal, CHUNKSTATE chunkType, LCID locale,
    DWORD cwcLenSource, DWORD cwcStartSource, CHUNK_BREAKTYPE chunkBreakType);
extern "C" HRESULT MS_ABI impl__SetInt64Value_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey___JW4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z(
    void* pThis, const PROPERTYKEY* pkey, __int64 nVal, CHUNKSTATE chunkType, LCID locale,
    DWORD cwcLenSource, DWORD cwcStartSource, CHUNK_BREAKTYPE chunkBreakType);
extern "C" HRESULT MS_ABI impl__SetIntValue_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey__HW4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z(
    void* pThis, const PROPERTYKEY* pkey, int nVal, CHUNKSTATE chunkType, LCID locale,
    DWORD cwcLenSource, DWORD cwcStartSource, CHUNK_BREAKTYPE chunkBreakType);
extern "C" HRESULT MS_ABI impl__SetLongValue_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey__JW4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z(
    void* pThis, const PROPERTYKEY* pkey, long lVal, CHUNKSTATE chunkType, LCID locale,
    DWORD cwcLenSource, DWORD cwcStartSource, CHUNK_BREAKTYPE chunkBreakType);
extern "C" HRESULT MS_ABI impl__SetSystemTimeValue_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey__AEBU_SYSTEMTIME__W4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z(
    void* pThis, const PROPERTYKEY* pkey, const SYSTEMTIME* pSystemTime, CHUNKSTATE chunkType, LCID locale,
    DWORD cwcLenSource, DWORD cwcStartSource, CHUNK_BREAKTYPE chunkBreakType);
extern "C" HRESULT MS_ABI impl__SetTextValue_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey__PEB_WW4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z(
    void* pThis, const PROPERTYKEY* pkey, const wchar_t* pszValue, CHUNKSTATE chunkType, LCID locale,
    DWORD cwcLenSource, DWORD cwcStartSource, CHUNK_BREAKTYPE chunkBreakType);

namespace {

// The inline (header-defined) virtuals, which retail emits as out-of-line
// vftable targets (RVAs are mfc140u):
BOOL MS_ABI FCV_IsValid(const void* pThis) {                   // slot 2, 0x21e5c0: mov 0x8(%rcx),%eax
    return static_cast<const S_FilterChunkValue*>(pThis)->m_fIsValid;
}
PROPVARIANT* MS_ABI FCV_GetValueNoAlloc(void* pThis, PROPVARIANT* pRet) {  // slot 4, 0x21e5a0: 24-byte copy of +0x50 to *RDX, returns RDX
    std::memcpy(pRet, &Self(pThis)->m_propVariant, sizeof(PROPVARIANT));
    return pRet;
}
CString* MS_ABI FCV_GetString(void* pThis) {                   // slot 5, 0x21e590: lea 0x68(%rcx),%rax
    return &Self(pThis)->m_strValue;
}
CHUNKSTATE MS_ABI FCV_GetChunkType(const void* pThis) {        // slot 7, 0x1094e0: mov 0x18(%rcx),%eax
    return static_cast<const S_FilterChunkValue*>(pThis)->m_chunk.flags;
}
const GUID* MS_ABI FCV_GetChunkGUID(const void* pThis) {       // slot 8, 0x21e580: lea 0x20(%rcx),%rax (REFGUID)
    return &static_cast<const S_FilterChunkValue*>(pThis)->m_chunk.attribute.guidPropSet;
}
DWORD MS_ABI FCV_GetChunkPID(const void* pThis) {              // slot 9, 0x21e570: mov 0x38(%rcx),%eax
    return static_cast<const S_FilterChunkValue*>(pThis)->m_chunk.attribute.psProperty.propid;
}

// The scalar deleting destructor (slot 0, 0x21df50, mfc140u):
//     ~CMFCFilterChunkValueImpl();                   // call 0x21df90
//     if (flags & 1) operator delete(this, 0x70);    // call 0x2b77b0
//     return this;
// The retail sized delete is a jump into the CRT's free (the same 0x2b77b0
// that core/controlbar/CDockContext.cpp documents); OpenMFC's exported
// operator new (detail/MemcoreSupport.cpp) is std::malloc, so std::free pairs
// with a client `new CMFCFilterChunkValueImpl` that went through the DLL.
void* MS_ABI FCV_DeletingDtor(void* pThis, unsigned int flags) {
    impl___1CMFCFilterChunkValueImpl__UEAA_XZ(pThis);
    if (flags & 1u) std::free(pThis);
    return pThis;
}

void* const g_FilterChunkValueVtbl[20] = {
    reinterpret_cast<void*>(&FCV_DeletingDtor),                                                        //  0
    reinterpret_cast<void*>(&impl__Clear_CMFCFilterChunkValueImpl__UEAAXXZ),                           //  1
    reinterpret_cast<void*>(&FCV_IsValid),                                                             //  2
    reinterpret_cast<void*>(&impl__GetValue_CMFCFilterChunkValueImpl__UEAAJPEAPEAUtagPROPVARIANT___Z), //  3
    reinterpret_cast<void*>(&FCV_GetValueNoAlloc),                                                     //  4
    reinterpret_cast<void*>(&FCV_GetString),                                                           //  5
    reinterpret_cast<void*>(&impl__CopyChunk_CMFCFilterChunkValueImpl__UEAAJPEAUtagSTAT_CHUNK___Z),    //  6
    reinterpret_cast<void*>(&FCV_GetChunkType),                                                        //  7
    reinterpret_cast<void*>(&FCV_GetChunkGUID),                                                        //  8
    reinterpret_cast<void*>(&FCV_GetChunkPID),                                                         //  9
    reinterpret_cast<void*>(&impl__SetTextValue_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey__PEB_WW4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z),          // 10
    reinterpret_cast<void*>(&impl__SetBoolValue_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey__FW4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z),             // 11 (VARIANT_BOOL)
    reinterpret_cast<void*>(&impl__SetBoolValue_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey__HW4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z),             // 12 (BOOL)
    reinterpret_cast<void*>(&impl__SetIntValue_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey__HW4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z),              // 13
    reinterpret_cast<void*>(&impl__SetLongValue_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey__JW4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z),             // 14
    reinterpret_cast<void*>(&impl__SetDwordValue_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey__KW4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z),            // 15
    reinterpret_cast<void*>(&impl__SetInt64Value_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey___JW4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z),           // 16
    reinterpret_cast<void*>(&impl__SetSystemTimeValue_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey__AEBU_SYSTEMTIME__W4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z), // 17
    reinterpret_cast<void*>(&impl__SetFileTimeValue_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey__U_FILETIME__W4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z),    // 18
    reinterpret_cast<void*>(&impl__CopyFrom_CMFCFilterChunkValueImpl__UEAAXPEAUIFilterChunkValue_ATL___Z), // 19
};

// The value setters all share one shape (SetBoolValue(VARIANT_BOOL) 0x21e1b0,
// SetIntValue/SetLongValue 0x21e210, SetDwordValue 0x21e270, SetInt64Value
// 0x21e2d0, SetFileTimeValue 0x21e3b0, mfc140u):
//     HRESULT hr = SetChunk(pkey, chunkType, locale, cwcLenSource,
//                           cwcStartSource, chunkBreakType);   // direct call 0x21e410
//     if (SUCCEEDED(hr)) {
//         m_propVariant.vt = <VT>;          // movw $<VT>,0x50(%rbx)
//         <member at +0x58> = value;        // width of the value
//         m_fIsValid = TRUE;                // movl $1,0x8(%rbx)
//     }
//     return hr;
// (SetChunk always returns S_OK in retail, but the setters test it.)
template <typename T>
HRESULT SetScalar(void* pThis, const PROPERTYKEY* pkey, VARTYPE vt, T value, CHUNKSTATE chunkType,
                  LCID locale, DWORD cwcLenSource, DWORD cwcStartSource, CHUNK_BREAKTYPE chunkBreakType) {
    HRESULT hr = impl__SetChunk_CMFCFilterChunkValueImpl__IEAAJAEBU_tagpropertykey__W4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z(
        pThis, pkey, chunkType, locale, cwcLenSource, cwcStartSource, chunkBreakType);
    if (SUCCEEDED(hr)) {
        S_FilterChunkValue* s = Self(pThis);
        s->m_propVariant.vt = vt;
        std::memcpy(&s->m_propVariant.boolVal, &value, sizeof(T));   // the value union at +0x58
        s->m_fIsValid = TRUE;
    }
    return hr;
}

} // namespace

// Transcribed from retail ??0 RVA 0x21df00 (mfc140u):
//     vfptr = CMFCFilterChunkValueImpl vftable (0x180328b48);
//     m_strValue = <string manager>->GetNilString() + 1;   // vslot 3 of the manager at 0x1803b25e8, +0x18 -> +0x68
//     Clear();                                             // direct call 0x21dfe0
//     return this;
// OpenMFC stores g_FilterChunkValueVtbl (deviation (1)) and default-constructs
// the CString, which points it at OpenMFC's nil string data.
// Symbol: ??0CMFCFilterChunkValueImpl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCFilterChunkValueImpl__QEAA_XZ(void* pThis) {
    S_FilterChunkValue* s = Self(pThis);
    s->vfptr = const_cast<void**>(g_FilterChunkValueVtbl);
    new (&s->m_strValue) CString();
    impl__Clear_CMFCFilterChunkValueImpl__UEAAXXZ(pThis);
    return pThis;
}

// Transcribed from retail ??1 RVA 0x21df90 (mfc140u):
//     vfptr = CMFCFilterChunkValueImpl vftable;
//     Clear();                                  // direct call 0x21dfe0
//     m_strValue.~CString();                    // inline: lock xadd -1 on nRefs, pStringMgr->Free if <= 0
//     vfptr = ATL::IFilterChunkValue vftable;   // 0x180328c20 -- NOT reproduced, deviation (2)
// Symbol: ??1CMFCFilterChunkValueImpl@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCFilterChunkValueImpl__UEAA_XZ(void* pThis) {
    S_FilterChunkValue* s = Self(pThis);
    s->vfptr = const_cast<void**>(g_FilterChunkValueVtbl);
    impl__Clear_CMFCFilterChunkValueImpl__UEAAXXZ(pThis);
    s->m_strValue.~CString();
}

// Transcribed from retail RVA 0x21dfe0 (mfc140u):
//     m_fIsValid = FALSE;
//     memset(&m_chunk, 0, 0x40);          // IAT memset
//     memset(&m_propVariant, 0, 0x18);    // IAT memset -- no PropVariantClear
//     m_strValue.Empty();                 // tail jump to CSimpleStringT<wchar_t>::Empty (0x33b0)
// The PROPVARIANT is zeroed, not cleared.  The only pointer this class's own
// setters store in it is VT_LPWSTR aliasing m_strValue's buffer (SetTextValue),
// which the Empty() releases.  CopyFrom shallow-copies another object's
// PROPVARIANT, so after it any other pointer-bearing VARTYPE aliases memory
// owned by that source object; retail does not free that here either.
// Deviation (3): OpenMFC's inline CString::Empty releases and re-points at
// the nil string unconditionally; ATL's Empty is a no-op on an already-empty
// string and, for a locked (GetBuffer'ed) buffer, truncates it in place.
// Symbol: ?Clear@CMFCFilterChunkValueImpl@@UEAAXXZ
extern "C" void MS_ABI impl__Clear_CMFCFilterChunkValueImpl__UEAAXXZ(void* pThis) {
    S_FilterChunkValue* s = Self(pThis);
    s->m_fIsValid = FALSE;
    std::memset(&s->m_chunk, 0, sizeof(s->m_chunk));
    std::memset(&s->m_propVariant, 0, sizeof(s->m_propVariant));
    s->m_strValue.Empty();
}

// Transcribed from retail RVA 0x21e090 (mfc140u):
//     if (pStatChunk == NULL) return E_INVALIDARG;   // 0x80070057
//     *pStatChunk = m_chunk;                         // 4 x movups, +0x10..+0x4f
//     return S_OK;
// Symbol: ?CopyChunk@CMFCFilterChunkValueImpl@@UEAAJPEAUtagSTAT_CHUNK@@@Z
extern "C" HRESULT MS_ABI impl__CopyChunk_CMFCFilterChunkValueImpl__UEAAJPEAUtagSTAT_CHUNK___Z(
    void* pThis, STAT_CHUNK* pStatChunk) {
    if (pStatChunk == nullptr) return E_INVALIDARG;
    std::memcpy(pStatChunk, &Self(pThis)->m_chunk, sizeof(STAT_CHUNK));
    return S_OK;
}

// Transcribed from retail RVA 0x21e480 (mfc140u); every call is unconditional
// and pValue is not NULL-checked:
//     this->Clear();                                     // own vslot 1 (0x8)
//     pValue->CopyChunk(&m_chunk);                       // vslot 6 (0x30), result ignored
//     m_propVariant = pValue->GetValueNoAlloc();         // vslot 4 (0x20), 24-byte shallow copy
//     m_strValue = pValue->GetString();                  // vslot 5 (0x28), CSimpleStringT::operator= (0xde30)
//     if (m_propVariant.vt == VT_LPWSTR)                 // cmpw $0x1f,0x50(%rbx)
//         m_propVariant.pwszVal = m_strValue buffer;     // re-point at OUR copy of the text
//     m_fIsValid = pValue->IsValid();                    // vslot 2 (0x10)
// Deviation (3): OpenMFC's inline CString::operator= always shares the source
// buffer (AddRef, or alias without a reference if the source is locked); ATL's
// operator= (0xde30) deep-copies instead when the destination is locked or the
// two strings use different string managers, and its CloneData (0xdd40)
// deep-copies a locked source.  Note that OpenMFC's nil string is itself
// "locked" (nRefs == -1), so after this class's own Clear() the destination is
// locked from ATL's point of view.
// Symbol: ?CopyFrom@CMFCFilterChunkValueImpl@@UEAAXPEAUIFilterChunkValue@ATL@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCFilterChunkValueImpl__UEAAXPEAUIFilterChunkValue_ATL___Z(
    void* pThis, void* pValue) {
    S_FilterChunkValue* s = Self(pThis);
    VirtualClear(pThis);
    void** vt = Vtbl(pValue);
    reinterpret_cast<PFN_CopyChunk>(vt[6])(pValue, &s->m_chunk);
    PROPVARIANT tmp;
    PROPVARIANT* pv = reinterpret_cast<PFN_GetValueNoAlloc>(vt[4])(pValue, &tmp);
    std::memcpy(&s->m_propVariant, pv, sizeof(PROPVARIANT));
    CString* pStr = reinterpret_cast<PFN_GetString>(vt[5])(pValue);
    s->m_strValue = *pStr;
    if (s->m_propVariant.vt == VT_LPWSTR) {
        s->m_propVariant.pwszVal = const_cast<LPWSTR>(static_cast<const wchar_t*>(s->m_strValue));
    }
    s->m_fIsValid = reinterpret_cast<PFN_IsValid>(vt[2])(pValue);
}

// Transcribed from retail RVA 0x21e020 (mfc140u):
//     if (ppPropVariant == NULL) return E_INVALIDARG;
//     *ppPropVariant = NULL;
//     PROPVARIANT* p = CoTaskMemAlloc(0x18);            // IAT CoTaskMemAlloc
//     PROPVARIANT* pFree = p;
//     HRESULT hr;
//     if (p == NULL) hr = E_OUTOFMEMORY;                // 0x8007000e
//     else {
//         hr = PropVariantCopy(p, &m_propVariant);      // IAT PropVariantCopy
//         if (SUCCEEDED(hr)) { pFree = NULL; *ppPropVariant = p; }
//     }
//     CoTaskMemFree(pFree);                             // IAT CoTaskMemFree, always called
//     return hr;
// Symbol: ?GetValue@CMFCFilterChunkValueImpl@@UEAAJPEAPEAUtagPROPVARIANT@@@Z
extern "C" HRESULT MS_ABI impl__GetValue_CMFCFilterChunkValueImpl__UEAAJPEAPEAUtagPROPVARIANT___Z(
    void* pThis, PROPVARIANT** ppPropVariant) {
    if (ppPropVariant == nullptr) return E_INVALIDARG;
    *ppPropVariant = nullptr;
    PROPVARIANT* p = static_cast<PROPVARIANT*>(::CoTaskMemAlloc(sizeof(PROPVARIANT)));
    PROPVARIANT* pFree = p;
    HRESULT hr;
    if (p == nullptr) {
        hr = E_OUTOFMEMORY;
    } else {
        hr = ::PropVariantCopy(p, &Self(pThis)->m_propVariant);
        if (SUCCEEDED(hr)) {
            pFree = nullptr;
            *ppPropVariant = p;
        }
    }
    ::CoTaskMemFree(pFree);
    return hr;
}

// Transcribed from retail RVA 0x21e1b0 (mfc140u; vslot 11): the shared setter
// shape above with vt = VT_BOOL (0xb) and the 16-bit bVal stored at +0x58.
// Symbol: ?SetBoolValue@CMFCFilterChunkValueImpl@@UEAAJAEBU_tagpropertykey@@FW4tagCHUNKSTATE@@KKKW4tagCHUNK_BREAKTYPE@@@Z
extern "C" HRESULT MS_ABI impl__SetBoolValue_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey__FW4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z(
    void* pThis, const PROPERTYKEY* pkey, VARIANT_BOOL bVal, CHUNKSTATE chunkType, LCID locale,
    DWORD cwcLenSource, DWORD cwcStartSource, CHUNK_BREAKTYPE chunkBreakType) {
    return SetScalar<VARIANT_BOOL>(pThis, pkey, VT_BOOL, bVal, chunkType, locale,
                                   cwcLenSource, cwcStartSource, chunkBreakType);
}

// Transcribed from retail RVA 0x21e150 (mfc140u; vslot 12):
//     return this->SetBoolValue(pkey, bVal ? VARIANT_TRUE : VARIANT_FALSE,   // neg/sbb -> 0xffff or 0
//                               chunkType, locale, cwcLenSource,
//                               cwcStartSource, chunkBreakType);          // own vslot 11 (0x58)
// Symbol: ?SetBoolValue@CMFCFilterChunkValueImpl@@UEAAJAEBU_tagpropertykey@@HW4tagCHUNKSTATE@@KKKW4tagCHUNK_BREAKTYPE@@@Z
extern "C" HRESULT MS_ABI impl__SetBoolValue_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey__HW4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z(
    void* pThis, const PROPERTYKEY* pkey, BOOL bVal, CHUNKSTATE chunkType, LCID locale,
    DWORD cwcLenSource, DWORD cwcStartSource, CHUNK_BREAKTYPE chunkBreakType) {
    return reinterpret_cast<PFN_SetBoolValueVB>(Vtbl(pThis)[11])(
        pThis, pkey, bVal ? VARIANT_TRUE : VARIANT_FALSE, chunkType, locale,
        cwcLenSource, cwcStartSource, chunkBreakType);
}

// Transcribed from retail RVA 0x21e410 (mfc140u; protected, non-virtual):
//     this->Clear();                                      // own vslot 1 (0x8)
//     m_chunk.attribute.psProperty.ulKind = PRSPEC_PROPID; // movl $1,0x30(%rsi)
//     m_chunk.attribute.psProperty.propid = pkey.pid;     // +0x38
//     m_chunk.flags = chunkType;                          // +0x18
//     m_chunk.attribute.guidPropSet = pkey.fmtid;         // +0x20
//     if (locale == 0) locale = GetUserDefaultLCID();     // IAT GetUserDefaultLCID
//     m_chunk.cwcLenSource   = cwcLenSource;              // +0x48
//     m_chunk.cwcStartSource = cwcStartSource;            // +0x44
//     m_chunk.breakType      = chunkBreakType;            // +0x14
//     m_chunk.locale         = locale;                    // +0x1c
//     return S_OK;
// idChunk (+0x10) and idChunkSource (+0x40) are left as Clear() zeroed them.
// Symbol: ?SetChunk@CMFCFilterChunkValueImpl@@IEAAJAEBU_tagpropertykey@@W4tagCHUNKSTATE@@KKKW4tagCHUNK_BREAKTYPE@@@Z
extern "C" HRESULT MS_ABI impl__SetChunk_CMFCFilterChunkValueImpl__IEAAJAEBU_tagpropertykey__W4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z(
    void* pThis, const PROPERTYKEY* pkey, CHUNKSTATE chunkType, LCID locale,
    DWORD cwcLenSource, DWORD cwcStartSource, CHUNK_BREAKTYPE chunkBreakType) {
    S_FilterChunkValue* s = Self(pThis);
    VirtualClear(pThis);
    s->m_chunk.attribute.psProperty.ulKind = PRSPEC_PROPID;
    s->m_chunk.attribute.psProperty.propid = pkey->pid;
    s->m_chunk.flags = chunkType;
    s->m_chunk.attribute.guidPropSet = pkey->fmtid;
    if (locale == 0) locale = ::GetUserDefaultLCID();
    s->m_chunk.cwcLenSource = cwcLenSource;
    s->m_chunk.cwcStartSource = cwcStartSource;
    s->m_chunk.breakType = chunkBreakType;
    s->m_chunk.locale = locale;
    return S_OK;
}

// Transcribed from retail RVA 0x21e270 (mfc140u; vslot 15): the shared setter
// shape with vt = VT_UI4 (0x13) and the dword stored at +0x58.
// Symbol: ?SetDwordValue@CMFCFilterChunkValueImpl@@UEAAJAEBU_tagpropertykey@@KW4tagCHUNKSTATE@@KKKW4tagCHUNK_BREAKTYPE@@@Z
extern "C" HRESULT MS_ABI impl__SetDwordValue_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey__KW4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z(
    void* pThis, const PROPERTYKEY* pkey, DWORD dwVal, CHUNKSTATE chunkType, LCID locale,
    DWORD cwcLenSource, DWORD cwcStartSource, CHUNK_BREAKTYPE chunkBreakType) {
    return SetScalar<DWORD>(pThis, pkey, VT_UI4, dwVal, chunkType, locale,
                            cwcLenSource, cwcStartSource, chunkBreakType);
}

// Transcribed from retail RVA 0x21e3b0 (mfc140u; vslot 18): the shared setter
// shape with vt = VT_FILETIME (0x40) and the by-value FILETIME -- an 8-byte
// aggregate passed in R8 under the MS x64 ABI, hence `unsigned long long`
// here -- stored whole at +0x58.
// Symbol: ?SetFileTimeValue@CMFCFilterChunkValueImpl@@UEAAJAEBU_tagpropertykey@@U_FILETIME@@W4tagCHUNKSTATE@@KKKW4tagCHUNK_BREAKTYPE@@@Z
extern "C" HRESULT MS_ABI impl__SetFileTimeValue_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey__U_FILETIME__W4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z(
    void* pThis, const PROPERTYKEY* pkey, unsigned long long dtVal, CHUNKSTATE chunkType, LCID locale,
    DWORD cwcLenSource, DWORD cwcStartSource, CHUNK_BREAKTYPE chunkBreakType) {
    return SetScalar<unsigned long long>(pThis, pkey, VT_FILETIME, dtVal, chunkType, locale,
                                         cwcLenSource, cwcStartSource, chunkBreakType);
}

// Transcribed from retail RVA 0x21e2d0 (mfc140u; vslot 16): the shared setter
// shape with vt = VT_I8 (0x14) and the qword stored at +0x58.
// Symbol: ?SetInt64Value@CMFCFilterChunkValueImpl@@UEAAJAEBU_tagpropertykey@@_JW4tagCHUNKSTATE@@KKKW4tagCHUNK_BREAKTYPE@@@Z
extern "C" HRESULT MS_ABI impl__SetInt64Value_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey___JW4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z(
    void* pThis, const PROPERTYKEY* pkey, __int64 nVal, CHUNKSTATE chunkType, LCID locale,
    DWORD cwcLenSource, DWORD cwcStartSource, CHUNK_BREAKTYPE chunkBreakType) {
    return SetScalar<__int64>(pThis, pkey, VT_I8, nVal, chunkType, locale,
                              cwcLenSource, cwcStartSource, chunkBreakType);
}

// Transcribed from retail RVA 0x21e210 (mfc140u; vslot 13): the shared setter
// shape with vt = VT_I4 (3) and the dword stored at +0x58.  Retail folds
// SetIntValue and SetLongValue into this one body (both exports and both
// vslots 13/14 resolve to 0x21e210).
// Symbol: ?SetIntValue@CMFCFilterChunkValueImpl@@UEAAJAEBU_tagpropertykey@@HW4tagCHUNKSTATE@@KKKW4tagCHUNK_BREAKTYPE@@@Z
extern "C" HRESULT MS_ABI impl__SetIntValue_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey__HW4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z(
    void* pThis, const PROPERTYKEY* pkey, int nVal, CHUNKSTATE chunkType, LCID locale,
    DWORD cwcLenSource, DWORD cwcStartSource, CHUNK_BREAKTYPE chunkBreakType) {
    return SetScalar<int>(pThis, pkey, VT_I4, nVal, chunkType, locale,
                          cwcLenSource, cwcStartSource, chunkBreakType);
}

// Transcribed from retail RVA 0x21e210 (mfc140u; vslot 14) -- the same folded
// body as SetIntValue: VT_I4 (3), dword at +0x58.
// Symbol: ?SetLongValue@CMFCFilterChunkValueImpl@@UEAAJAEBU_tagpropertykey@@JW4tagCHUNKSTATE@@KKKW4tagCHUNK_BREAKTYPE@@@Z
extern "C" HRESULT MS_ABI impl__SetLongValue_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey__JW4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z(
    void* pThis, const PROPERTYKEY* pkey, long lVal, CHUNKSTATE chunkType, LCID locale,
    DWORD cwcLenSource, DWORD cwcStartSource, CHUNK_BREAKTYPE chunkBreakType) {
    return SetScalar<long>(pThis, pkey, VT_I4, lVal, chunkType, locale,
                           cwcLenSource, cwcStartSource, chunkBreakType);
}

// Transcribed from retail RVA 0x21e330 (mfc140u; vslot 17):
//     HRESULT hr = SetChunk(pkey, chunkType, locale, cwcLenSource,
//                           cwcStartSource, chunkBreakType);        // direct call 0x21e410
//     if (SUCCEEDED(hr)) {
//         m_propVariant.vt = VT_FILETIME;                           // 0x40
//         SystemTimeToFileTime(&systemTime, &m_propVariant.filetime); // IAT; result ignored
//         m_fIsValid = TRUE;
//     }
//     return hr;
// Symbol: ?SetSystemTimeValue@CMFCFilterChunkValueImpl@@UEAAJAEBU_tagpropertykey@@AEBU_SYSTEMTIME@@W4tagCHUNKSTATE@@KKKW4tagCHUNK_BREAKTYPE@@@Z
extern "C" HRESULT MS_ABI impl__SetSystemTimeValue_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey__AEBU_SYSTEMTIME__W4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z(
    void* pThis, const PROPERTYKEY* pkey, const SYSTEMTIME* pSystemTime, CHUNKSTATE chunkType, LCID locale,
    DWORD cwcLenSource, DWORD cwcStartSource, CHUNK_BREAKTYPE chunkBreakType) {
    HRESULT hr = impl__SetChunk_CMFCFilterChunkValueImpl__IEAAJAEBU_tagpropertykey__W4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z(
        pThis, pkey, chunkType, locale, cwcLenSource, cwcStartSource, chunkBreakType);
    if (SUCCEEDED(hr)) {
        S_FilterChunkValue* s = Self(pThis);
        s->m_propVariant.vt = VT_FILETIME;
        ::SystemTimeToFileTime(pSystemTime, &s->m_propVariant.filetime);
        s->m_fIsValid = TRUE;
    }
    return hr;
}

// Transcribed from retail RVA 0x21e0c0 (mfc140u; vslot 10):
//     if (pszValue == NULL) return E_INVALIDARG;         // before SetChunk: nothing is cleared
//     HRESULT hr = SetChunk(pkey, chunkType, locale, cwcLenSource,
//                           cwcStartSource, chunkBreakType);   // direct call 0x21e410
//     if (SUCCEEDED(hr)) {
//         m_strValue.SetString(pszValue, wcslen(pszValue));  // IAT wcslen; CSimpleStringT::SetString (0x2e30)
//         m_propVariant.pwszVal = m_strValue buffer;         // +0x58 aliases OUR copy of the text
//         m_propVariant.vt = VT_LPWSTR;                      // 0x1f
//         m_fIsValid = TRUE;
//     }
//     return hr;
// OpenMFC's CString::operator=(const wchar_t*) measures the string with
// wcslen and copies it (an empty string empties), which is what
// SetString(psz, wcslen(psz)) does.
// Symbol: ?SetTextValue@CMFCFilterChunkValueImpl@@UEAAJAEBU_tagpropertykey@@PEB_WW4tagCHUNKSTATE@@KKKW4tagCHUNK_BREAKTYPE@@@Z
extern "C" HRESULT MS_ABI impl__SetTextValue_CMFCFilterChunkValueImpl__UEAAJAEBU_tagpropertykey__PEB_WW4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z(
    void* pThis, const PROPERTYKEY* pkey, const wchar_t* pszValue, CHUNKSTATE chunkType, LCID locale,
    DWORD cwcLenSource, DWORD cwcStartSource, CHUNK_BREAKTYPE chunkBreakType) {
    if (pszValue == nullptr) return E_INVALIDARG;
    HRESULT hr = impl__SetChunk_CMFCFilterChunkValueImpl__IEAAJAEBU_tagpropertykey__W4tagCHUNKSTATE__KKKW4tagCHUNK_BREAKTYPE___Z(
        pThis, pkey, chunkType, locale, cwcLenSource, cwcStartSource, chunkBreakType);
    if (SUCCEEDED(hr)) {
        S_FilterChunkValue* s = Self(pThis);
        s->m_strValue = pszValue;
        s->m_propVariant.pwszVal = const_cast<LPWSTR>(static_cast<const wchar_t*>(s->m_strValue));
        s->m_propVariant.vt = VT_LPWSTR;
        s->m_fIsValid = TRUE;
    }
    return hr;
}
