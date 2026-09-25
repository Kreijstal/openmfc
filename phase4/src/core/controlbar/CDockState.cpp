// CDockState — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// Method: every body below was decoded from the retail export (disas.py / disas.py --u),
// the way core/ole/COleControl.cpp and core/controlbar/CDockBar.cpp do.  RVAs are cited
// in mfc140u.dll unless marked otherwise.  Function bodies are byte-identical between
// mfc140u.dll and its ANSI twin mfc140.dll; entries mfc140u's symbol map does not resolve
// (ScalePoint, ScaleRectPos, SetScreenSize, Serialize, SaveState) were located in mfc140u
// at the same distance from Clear as in mfc140.dll (+0x1ad0), and each located entry was
// checked to disassemble to the same instruction sequence apart from rip-relative
// displacements (SaveState also differs in its stack-frame size, 0x240 vs 0x140, because
// its section buffer is 256 TCHARs) -- SetScreenSize additionally is the callee of
// `call 0x1802215b0` inside LoadState, and Serialize is vftable slot 2.  (The delta is not
// uniform across the image: CControlBarInfo::LoadState sits at +0x1ac0, 0x2201f0 vs
// mfc140.dll 0x21e730.)
//
//   ctor 0x220bd0   dtor 0x220cb0   Serialize 0x220d50   LoadState 0x220fd0
//   SaveState 0x2211f0   Clear 0x2213b0   ScalePoint 0x221440   ScaleRectPos 0x2214c0
//   GetScreenSize 0x221590   SetScreenSize 0x2215b0   CreateObject 0x221fd0
//   GetVersion 0x23970   GetRuntimeClass 0x222000   {vector deleting dtor} 0x2b1a20
//
// ---- Object layout ------------------------------------------------------------------
// CDockState is not declared in OpenMFC's public headers (include/openmfc/afxwin.h only
// forward-declares it); MSVC clients build it with the layout of the shipping afxadv.h
// (read on this host: CObject base, CPtrArray m_arrBarInfo, BOOL m_bScaling, CRect
// m_rectDevice, CRect m_rectClip, CSize m_sizeLogical, DWORD m_dwVersion), so this file
// is the only place the layout lives.  Offsets as the retail code uses them:
//   +0x00 vfptr                 ctor `mov %rax,(%rcx)` (vftable 0x180328fc0)
//   +0x08 CPtrArray m_arrBarInfo  ctor stores its vfptr 0x180321190 at +0x08 and zeroes
//         +0x10 m_pData, +0x18 m_nSize, +0x20 m_nMaxSize, +0x28 m_nGrowBy; Clear/dtor
//         read +0x10 as the data pointer and +0x18 as the element count
//   +0x30 BOOL  m_bScaling      ScalePoint `cmpl $0x0,0x30(%rcx)`
//   +0x34 CRect m_rectDevice    GetScreenSize `0x3c - 0x34`, `0x40 - 0x38`
//   +0x44 CRect m_rectClip      ScalePoint clamps against +0x4c / +0x50 (right/bottom)
//   +0x54 CSize m_sizeLogical   ScalePoint MulDiv divisors +0x54 / +0x58
//   +0x5c DWORD m_dwVersion     GetVersion `mov 0x5c(%rcx),%eax`; ctor stores 2
//   sizeof 0x60                 CreateObject `mov $0x60,%ecx` before operator new; the
//                               retail CRuntimeClass (0x1803b18f8) has m_nObjectSize 0x60
//
// ---- m_arrBarInfo ---------------------------------------------------------------------
// Retail manipulates the array through the exported CPtrArray::SetSize / SetAtGrow and
// reads m_pData / m_nSize directly.  OpenMFC's CPtrArray (afx.h
// OPENMFC_DECLARE_ARRAY_WRAPPER) keeps its elements in a side table keyed by the object
// address and never touches +0x10 / +0x18, so its impl__ thunks would leave the fields MSVC
// clients read inline (GetSize / operator[] / GetData) permanently empty.  The bodies below
// therefore keep the retail fields themselves (DockStateArray, reproducing MFC array_p.cpp
// SetSize / SetAtGrow semantics), exactly as core/controlbar/CDockBar.cpp does for
// CDockBar::m_arrBars.  The buffer is malloc/free'd only inside this DLL.  DEVIATION: the
// CPtrArray vfptr at +0x08 is stored as nullptr, not the retail vftable (no MSVC-layout
// CPtrArray vftable exists in OpenMFC -- the same choice CDockBar.cpp makes); a client that
// grows m_arrBarInfo itself -- through the exported CPtrArray::SetSize/SetAtGrow, or the
// inline CPtrArray::Add (not exported; it reads m_nSize and calls the exported SetAtGrow) --
// updates OpenMFC's side table rather than these fields, so the element is not seen here.
//
// ---- vftable ----------------------------------------------------------------------------
// The retail vftable (0x180328fc0, mfc140u) has five slots, read from the image:
//   0 GetRuntimeClass 0x222000   1 {vector deleting dtor} 0x2b1a20   2 Serialize 0x220d50
//   3 AssertValid, 4 Dump: both 0x27d0, a bare `ret`
// kDockStateVtbl reproduces it and the constructor installs it (as featurepack/customize/
// CMouseManager.cpp does for its class), so an MSVC client's `delete pState`, virtual
// Serialize or GetRuntimeClass lands in these bodies.
// CAVEAT: this is the MSVC slot order.  OpenMFC's own mingw-compiled code that dispatches
// through CObject (include/openmfc/afx.h) uses the Itanium order -- GetRuntimeClass 0,
// ~CObject complete 1 / deleting 2, Serialize 3, AssertValid 4, Dump 5 -- so only slot 0
// agrees.  In particular CArchive::operator<<(const CObject*) / operator>>(CObject*&)
// (core/runtime/CArchive.cpp) call pOb->Serialize(*this) through slot 3, which here is the
// AssertValid no-op; they reach that call only when m_pfnCreateObject is non-null, and the
// CDockState descriptors (core/controlbar/ and featurepack/docking/RuntimeClasses.cpp)
// currently leave it null.  A mingw-side `delete` of a CObject* that is a CDockState would
// call slot 2 (Serialize) and must not happen.
//
// ---- CControlBarInfo elements -------------------------------------------------------
// The elements are CControlBarInfo* (sizeof 0x78; layout pinned in
// detail/CControlBarInfoSupport.h).  CControlBarInfo::Serialize / LoadState / SaveState
// (core/controlbar/CControlBarInfo.cpp) are currently NOT retail transcriptions: they do no
// archive or profile I/O.  The CDockState bodies here call them exactly where retail does,
// so the per-bar records are streamed/persisted once those siblings are implemented; until
// then Serialize writes/reads only the CDockState header and the bar count, and
// LoadState/SaveState only the "<profile>-Summary" values.

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/CControlBarInfoSupport.h"
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cwchar>

// ---------------------------------------------------------------------------
// Sibling thunks, declared with the signature their mangled name describes; every
// definition was located with grep before use.
// ---------------------------------------------------------------------------
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDockState__UEBAPEAUCRuntimeClass__XZ(const void* pThis); // core/controlbar/RuntimeClasses.cpp
extern "C" void* MS_ABI impl___0CControlBarInfo__QEAA_XZ(void* pThis);                                      // core/controlbar/CControlBarInfo.cpp
extern "C" void MS_ABI impl__Serialize_CControlBarInfo__QEAAXAEAVCArchive__PEAVCDockState___Z(
    void* pThis, void* ar, void* pDockState);                                                              // core/controlbar/CControlBarInfo.cpp
extern "C" BOOL MS_ABI impl__LoadState_CControlBarInfo__QEAAHPEB_WHPEAVCDockState___Z(
    void* pThis, const wchar_t* lpszProfileName, int nIndex, void* pDockState);                            // core/controlbar/CControlBarInfo.cpp
extern "C" BOOL MS_ABI impl__SaveState_CControlBarInfo__QEAAHPEB_WH_Z(
    void* pThis, const wchar_t* lpszProfileName, int nIndex);                                              // core/controlbar/CControlBarInfo.cpp
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();                                           // detail/RegcoreSupport.cpp (internal; not an export)
extern "C" unsigned int MS_ABI impl__GetProfileIntW_CWinApp__UEAAIPEB_W0H_Z(
    CWinApp* pThis, const wchar_t* section, const wchar_t* entry, int defaultValue);                       // core/app/CWinApp.cpp
extern "C" int MS_ABI impl__WriteProfileInt_CWinApp__UEAAHPEB_W0H_Z(
    CWinApp* pThis, const wchar_t* section, const wchar_t* entry, int value);                              // core/app/CWinApp.cpp
extern "C" unsigned int MS_ABI impl__Read_CArchive__QEAAIPEAXI_Z(CArchive* pThis, void* lpBuf, unsigned int nMax);    // core/runtime/Thunks.cpp
extern "C" void MS_ABI impl__Write_CArchive__QEAAXPEBXI_Z(CArchive* pThis, const void* lpBuf, unsigned int nMax);     // core/runtime/Thunks.cpp
extern "C" void MS_ABI impl__AfxThrowArchiveException__YAXHPEB_W_Z(int cause, const wchar_t* lpszArchiveName);        // detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();                                            // detail/MfcExceptionsSupport.cpp
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);                                            // detail/MemcoreSupport.cpp (malloc)

// Thunks defined further down in this file that earlier bodies call.
extern "C" void MS_ABI impl___1CDockState__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__Clear_CDockState__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__SetScreenSize_CDockState__QEAAXAEAVCSize___Z(void* pThis, SIZE* pSize);
extern "C" void MS_ABI impl__Serialize_CDockState__UEAAXAEAVCArchive___Z(void* pThis, CArchive* ar);

namespace {

// ---------------------------------------------------------------------------
// Layout view (see the file header for where every offset comes from).
// ---------------------------------------------------------------------------
struct DockStateLayout {
    void*    vfptr;               // 0x00
    void*    arr_vfptr;           // 0x08  CPtrArray vfptr (retail 0x180321190; nullptr here)
    void**   arr_pData;           // 0x10  CPtrArray::m_pData
    INT_PTR  arr_nSize;           // 0x18  CPtrArray::m_nSize
    INT_PTR  arr_nMaxSize;        // 0x20  CPtrArray::m_nMaxSize
    INT_PTR  arr_nGrowBy;         // 0x28  CPtrArray::m_nGrowBy
    int      m_bScaling;          // 0x30
    RECT     m_rectDevice;        // 0x34
    RECT     m_rectClip;          // 0x44
    SIZE     m_sizeLogical;       // 0x54
    DWORD    m_dwVersion;         // 0x5c
};
static_assert(offsetof(DockStateLayout, arr_vfptr)     == 0x08, "CDockState::m_arrBarInfo");
static_assert(offsetof(DockStateLayout, arr_pData)     == 0x10, "CDockState::m_arrBarInfo.m_pData");
static_assert(offsetof(DockStateLayout, arr_nSize)     == 0x18, "CDockState::m_arrBarInfo.m_nSize");
static_assert(offsetof(DockStateLayout, arr_nMaxSize)  == 0x20, "CDockState::m_arrBarInfo.m_nMaxSize");
static_assert(offsetof(DockStateLayout, arr_nGrowBy)   == 0x28, "CDockState::m_arrBarInfo.m_nGrowBy");
static_assert(offsetof(DockStateLayout, m_bScaling)    == 0x30, "CDockState::m_bScaling");
static_assert(offsetof(DockStateLayout, m_rectDevice)  == 0x34, "CDockState::m_rectDevice");
static_assert(offsetof(DockStateLayout, m_rectClip)    == 0x44, "CDockState::m_rectClip");
static_assert(offsetof(DockStateLayout, m_sizeLogical) == 0x54, "CDockState::m_sizeLogical");
static_assert(offsetof(DockStateLayout, m_dwVersion)   == 0x5c, "CDockState::m_dwVersion");
static_assert(sizeof(DockStateLayout) == 0x60, "CDockState size (retail CreateObject allocates 0x60)");

constexpr std::size_t kDockStateSize = 0x60;
constexpr std::size_t kControlBarInfoSize = 0x78;   // retail `mov $0x78,%ecx` before operator new
static_assert(sizeof(S_Ccontrolbarinfo) == kControlBarInfoSize, "CControlBarInfo size");

inline DockStateLayout* DS(void* p) { return static_cast<DockStateLayout*>(p); }

// m_rectDevice.Size(), which retail computes inline wherever it needs it.
inline SIZE DeviceSize(const DockStateLayout* s) {
    SIZE size;
    size.cx = s->m_rectDevice.right - s->m_rectDevice.left;
    size.cy = s->m_rectDevice.bottom - s->m_rectDevice.top;
    return size;
}

// ---------------------------------------------------------------------------
// m_arrBarInfo helpers: MFC array_p.cpp CPtrArray::SetSize / SetAtGrow on the retail
// fields (grow-by heuristic max(4, min(1024, size/8)), zero-fill on growth).  Mirrors
// RetailArray in core/controlbar/CDockBar.cpp.
// ---------------------------------------------------------------------------
void ArraySetSize(DockStateLayout* s, INT_PTR nNewSize, INT_PTR nNewGrowBy) {
    if (nNewGrowBy >= 0) s->arr_nGrowBy = nNewGrowBy;
    if (nNewSize == 0) {
        std::free(s->arr_pData);
        s->arr_pData = nullptr;
        s->arr_nSize = s->arr_nMaxSize = 0;
    } else if (s->arr_pData == nullptr) {
        s->arr_pData = static_cast<void**>(std::malloc(static_cast<size_t>(nNewSize) * sizeof(void*)));
        if (s->arr_pData == nullptr) impl__AfxThrowMemoryException__YAXXZ();
        std::memset(s->arr_pData, 0, static_cast<size_t>(nNewSize) * sizeof(void*));
        s->arr_nSize = s->arr_nMaxSize = nNewSize;
    } else if (nNewSize <= s->arr_nMaxSize) {
        if (nNewSize > s->arr_nSize) {
            std::memset(s->arr_pData + s->arr_nSize, 0,
                        static_cast<size_t>(nNewSize - s->arr_nSize) * sizeof(void*));
        }
        s->arr_nSize = nNewSize;
    } else {
        INT_PTR grow = s->arr_nGrowBy;
        if (grow == 0) {
            grow = s->arr_nSize / 8;
            grow = (grow < 4) ? 4 : ((grow > 1024) ? 1024 : grow);
        }
        INT_PTR nNewMax = (nNewSize < s->arr_nMaxSize + grow) ? s->arr_nMaxSize + grow : nNewSize;
        void** pNewData = static_cast<void**>(std::malloc(static_cast<size_t>(nNewMax) * sizeof(void*)));
        if (pNewData == nullptr) impl__AfxThrowMemoryException__YAXXZ();
        std::memcpy(pNewData, s->arr_pData, static_cast<size_t>(s->arr_nSize) * sizeof(void*));
        std::memset(pNewData + s->arr_nSize, 0, static_cast<size_t>(nNewSize - s->arr_nSize) * sizeof(void*));
        std::free(s->arr_pData);
        s->arr_pData = pNewData;
        s->arr_nSize = nNewSize;
        s->arr_nMaxSize = nNewMax;
    }
}

void ArraySetAtGrow(DockStateLayout* s, INT_PTR nIndex, void* value) {
    if (nIndex >= s->arr_nSize) ArraySetSize(s, nIndex + 1, -1);
    s->arr_pData[nIndex] = value;
}

// `delete (CControlBarInfo*)p` as Clear and the destructor inline it: ~CControlBarInfo
// runs ~CUIntArray on m_arrBarID (+0x48: retail stores the CUIntArray vftable there, then
// free(m_pData at +0x50)), then operator delete(p, 0x78) -> ucrt free.  The vftable store
// into memory about to be freed is not reproduced.
void DeleteBarInfo(void* p) {
    if (p == nullptr) return;
    std::free(static_cast<S_Ccontrolbarinfo*>(p)->m_arrBarID_pData);
    std::free(p);
}

// `new CControlBarInfo`: retail calls ??2@YAPEAX_K@Z (0x27f0) with 0x78 and runs the
// constructor (0x21fb00) only when the allocation is non-null.
void* NewBarInfo() {
    void* p = impl___2_YAPEAX_K_Z(kControlBarInfoSize);
    return p ? impl___0CControlBarInfo__QEAA_XZ(p) : nullptr;
}

// "%Ts-Summary" (string at 0x18034cd28), formatted by LoadState/SaveState into a 256-TCHAR
// stack buffer through __stdio_common_vsnwprintf_s(buf, 0x100, 0xff, ...), i.e.
// _sntprintf_s(buf, 256, 255, ...).  DEVIATION: retail wraps the call in ATL's
// errno check (_errno saved/cleared, then ENOMEM -> AtlThrow(E_OUTOFMEMORY),
// EINVAL/ERANGE -> AtlThrow(E_INVALIDARG), anything but 0/STRUNCATE -> AtlThrow(E_FAIL));
// OpenMFC has no AtlThrow here, so a formatting failure just leaves the buffer as the
// CRT wrote it.
void FormatSummarySection(wchar_t (&buf)[256], const wchar_t* lpszProfileName) {
    buf[0] = L'\0';
    _snwprintf_s(buf, 256, 255, L"%ls-Summary", lpszProfileName);
}

// CArchive's inline 4-/2-byte operator<< / operator>> as retail inlines them into
// Serialize: storing -- if (m_lpBufCur + n > m_lpBufMax) Flush(); copy in;  loading -- if
// (m_lpBufCur + n > m_lpBufMax) FillBuffer(...); copy out.  OpenMFC's CArchive
// (include/openmfc/afx.h) has a different member layout, so these go through its
// Read/Write thunks; a short Read raises CArchiveException::endOfFile (3, NULL name) --
// the same helper shape featurepack/customize/CMouseManager.cpp uses.
template <class T>
T ArGet(CArchive* ar) {
    T value{};
    if (impl__Read_CArchive__QEAAIPEAXI_Z(ar, &value, sizeof(value)) != sizeof(value))
        impl__AfxThrowArchiveException__YAXHPEB_W_Z(3 /* CArchiveException::endOfFile */, nullptr);
    return value;
}
template <class T>
void ArPut(CArchive* ar, T value) {
    impl__Write_CArchive__QEAAXPEBXI_Z(ar, &value, sizeof(value));
}

// ---- vftable slots ----------------------------------------------------------------------

// Slot 1, the vector deleting destructor (0x2b1a20):
//   if (flags & 2) {                       // delete[]
//       base = this - 8; count = *(size_t*)base;
//       eh-vector-dtor(this, 0x60, count, ~CDockState)   // 0x2b7840: last element first
//       if (flags & 1) operator delete[](base, count*0x60 + 8)   // -> free
//       return base;
//   }
//   ~CDockState();                          // 0x220cb0
//   if (flags & 1) { if (flags & 4) call 0x27d0 (a bare `ret`, i.e. nothing) else free(this); }
//   return this;
void* MS_ABI DockStateDeletingDtor(void* pThis, unsigned int flags) {
    if ((flags & 2) != 0) {
        char* base = static_cast<char*>(pThis) - 8;
        std::size_t count = *reinterpret_cast<std::size_t*>(base);
        for (std::size_t i = count; i-- > 0;) {
            impl___1CDockState__UEAA_XZ(static_cast<char*>(pThis) + i * kDockStateSize);
        }
        if ((flags & 1) != 0) std::free(base);
        return base;
    }
    impl___1CDockState__UEAA_XZ(pThis);
    if ((flags & 1) != 0 && (flags & 4) == 0) std::free(pThis);
    return pThis;
}

// Slots 3 and 4 (AssertValid, Dump): both 0x27d0, a bare `ret`.
void MS_ABI DockStateNoOp(const void*, void*) {}

// The MSVC-layout vftable installed by the constructor (retail 0x180328fc0, mfc140u).
void* const kDockStateVtbl[5] = {
    reinterpret_cast<void*>(&impl__GetRuntimeClass_CDockState__UEBAPEAUCRuntimeClass__XZ), // 0 GetRuntimeClass (0x222000)
    reinterpret_cast<void*>(&DockStateDeletingDtor),                                       // 1 {vector deleting dtor} (0x2b1a20)
    reinterpret_cast<void*>(&impl__Serialize_CDockState__UEAAXAEAVCArchive___Z),           // 2 Serialize (0x220d50)
    reinterpret_cast<void*>(&DockStateNoOp),                                                // 3 AssertValid (0x27d0)
    reinterpret_cast<void*>(&DockStateNoOp),                                                // 4 Dump (0x27d0)
};

}  // namespace

// Symbol: ??0CDockState@@QEAA@XZ
// Transcribed from retail 0x220bd0 (mfc140u):
//   vfptr = CDockState vftable; m_dwVersion = 2;
//   m_arrBarInfo: vfptr = CPtrArray vftable, m_pData/m_nSize/m_nMaxSize/m_nGrowBy = 0;
//   m_bScaling = 0 and m_rectDevice / m_rectClip / m_sizeLogical zeroed (qword stores
//   covering +0x30..+0x5b);
//   m_rectDevice.right  = GetSystemMetrics(SM_CXSCREEN)         (0)
//   m_rectDevice.bottom = GetSystemMetrics(SM_CYSCREEN)         (1)
//   m_rectClip.left     = GetSystemMetrics(SM_XVIRTUALSCREEN)   (0x4c)
//   m_rectClip.top      = GetSystemMetrics(SM_YVIRTUALSCREEN)   (0x4d)
//   m_rectClip.right    = GetSystemMetrics(SM_CXVIRTUALSCREEN)  (0x4e)
//   m_rectClip.bottom   = GetSystemMetrics(SM_CYVIRTUALSCREEN)  (0x4f)
//   m_rectClip.right   -= GetSystemMetrics(SM_CXICON)           (0xb)
//   m_rectClip.bottom  -= GetSystemMetrics(SM_CYSMCAPTION) + GetSystemMetrics(SM_CYICON)  (0x33, 0xc)
// (IAT 0x1802c6bd8 -> USER32!GetSystemMetrics.)  Retail really stores the virtual-screen
// WIDTH/HEIGHT as right/bottom, not left+width; that is transcribed as-is.
// DEVIATION: the embedded CPtrArray vfptr is nullptr (see the file header).  The
// OpenMfcResetDockState call is OpenMFC's own: it drops any CFrameWnd::GetDockState
// snapshot (detail/WincoreSupport.cpp) left keyed to this address by a previous object.
extern "C" void* MS_ABI impl___0CDockState__QEAA_XZ(void* pThis) {
    DockStateLayout* s = DS(pThis);
    s->vfptr = const_cast<void**>(kDockStateVtbl);
    s->m_dwVersion = 2;
    s->arr_vfptr = nullptr;
    s->arr_pData = nullptr;
    s->arr_nSize = 0;
    s->arr_nMaxSize = 0;
    s->arr_nGrowBy = 0;
    s->m_bScaling = 0;
    s->m_rectDevice = RECT{0, 0, 0, 0};
    s->m_rectClip = RECT{0, 0, 0, 0};
    s->m_sizeLogical = SIZE{0, 0};
    s->m_rectDevice.right  = ::GetSystemMetrics(SM_CXSCREEN);
    s->m_rectDevice.bottom = ::GetSystemMetrics(SM_CYSCREEN);
    s->m_rectClip.left     = ::GetSystemMetrics(SM_XVIRTUALSCREEN);
    s->m_rectClip.top      = ::GetSystemMetrics(SM_YVIRTUALSCREEN);
    s->m_rectClip.right    = ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
    s->m_rectClip.bottom   = ::GetSystemMetrics(SM_CYVIRTUALSCREEN);
    s->m_rectClip.right   -= ::GetSystemMetrics(SM_CXICON);
    const int cySmCaption  = ::GetSystemMetrics(SM_CYSMCAPTION);
    s->m_rectClip.bottom  -= cySmCaption + ::GetSystemMetrics(SM_CYICON);
    OpenMfcResetDockState(pThis);
    return pThis;
}

// Symbol: ??1CDockState@@UEAA@XZ
// Transcribed from retail 0x220cb0 (mfc140u):
//   vfptr = CDockState vftable;
//   for (i = 0; i < m_arrBarInfo.GetSize(); i++)
//       delete (CControlBarInfo*)m_arrBarInfo[i];      // null-checked; see DeleteBarInfo
//   ~CPtrArray(): vfptr(+0x08) = CPtrArray vftable; free(m_pData)  (tail `jmp *free`)
// Unlike Clear it does not call CPtrArray::SetSize and does not null the fields.  The
// operator[] bounds check (AfxThrowInvalidArgException) cannot fire inside this loop and is
// not reproduced.  DEVIATION: the CPtrArray vfptr store is omitted (nullptr layout, see the
// file header); OpenMfcResetDockState is OpenMFC's side-table cleanup.
extern "C" void MS_ABI impl___1CDockState__UEAA_XZ(void* pThis) {
    DockStateLayout* s = DS(pThis);
    s->vfptr = const_cast<void**>(kDockStateVtbl);
    for (INT_PTR i = 0; i < s->arr_nSize; ++i) {
        DeleteBarInfo(s->arr_pData[i]);
    }
    std::free(s->arr_pData);
    OpenMfcResetDockState(pThis);
}

// Symbol: ?Clear@CDockState@@QEAAXXZ
// Transcribed from retail 0x2213b0 (mfc140u):
//   for (i = 0; i < m_arrBarInfo.GetSize(); i++)
//       delete (CControlBarInfo*)m_arrBarInfo[i];      // null-checked; see DeleteBarInfo
//   m_arrBarInfo.SetSize(0);                           // tail jump to CPtrArray::SetSize(0, -1)
// The operator[] bounds check cannot fire inside this loop and is not reproduced.
extern "C" void MS_ABI impl__Clear_CDockState__QEAAXXZ(void* pThis) {
    DockStateLayout* s = DS(pThis);
    for (INT_PTR i = 0; i < s->arr_nSize; ++i) {
        DeleteBarInfo(s->arr_pData[i]);
    }
    ArraySetSize(s, 0, -1);
}

// Symbol: ?CreateObject@CDockState@@SAPEAVCObject@@XZ
// Transcribed from retail 0x221fd0 (mfc140u): p = operator new(0x60); return p ?
// CDockState::CDockState(p) (tail jump) : NULL.
extern "C" void* MS_ABI impl__CreateObject_CDockState__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(kDockStateSize);
    return p ? impl___0CDockState__QEAA_XZ(p) : nullptr;
}

// Symbol: ?GetScreenSize@CDockState@@QEAA?AVCSize@@XZ
// Transcribed from retail 0x221590 (mfc140u).  The CSize comes back through the hidden
// return pointer, which MSVC x64 passes in RDX after `this` in RCX (retail writes
// (%rdx) / 4(%rdx) and returns %rdx):
//   return CSize(m_rectDevice.right - m_rectDevice.left, m_rectDevice.bottom - m_rectDevice.top);
extern "C" void* MS_ABI impl__GetScreenSize_CDockState__QEAA_AVCSize__XZ(void* pThis, SIZE* pRet) {
    *pRet = DeviceSize(DS(pThis));
    return pRet;
}

// Symbol: ?GetVersion@CDockState@@QEAAKXZ
// Transcribed from retail 0x23970 (mfc140u): `mov 0x5c(%rcx),%eax; ret` -- m_dwVersion.
extern "C" unsigned long MS_ABI impl__GetVersion_CDockState__QEAAKXZ(void* pThis) {
    return DS(pThis)->m_dwVersion;
}

// Symbol: ?LoadState@CDockState@@QEAAXPEB_W@Z
// Transcribed from retail 0x220fd0 (mfc140u):
//   CWinApp* pApp = AfxGetApp();                  // AfxGetModuleState()->m_pCurrentWinApp
//   TCHAR szSection[256]; _sntprintf_s(szSection, 256, 255, _T("%Ts-Summary"), lpszProfileName);
//   int nBars = pApp->GetProfileInt(szSection, _T("Bars"), 0);         // vslot 0x108/8 = 33
//   CSize size;
//   size.cx = pApp->GetProfileInt(szSection, _T("ScreenCX"), 0);
//   size.cy = pApp->GetProfileInt(szSection, _T("ScreenCY"), 0);
//   SetScreenSize(size);
//   for (i = 0; i < m_arrBarInfo.GetSize(); i++)
//       delete m_arrBarInfo[i];      // plain operator delete (0x2b77b0 -> free), NO
//                                    // ~CControlBarInfo and no null test -- unlike Clear
//   m_arrBarInfo.SetSize(0);
//   for (i = 0; i < nBars; i++) {
//       CControlBarInfo* pInfo = new CControlBarInfo;
//       m_arrBarInfo.Add(pInfo);     // CPtrArray::SetAtGrow(GetSize(), pInfo)
//       pInfo->LoadState(lpszProfileName, i, this);    // result ignored
//   }
// Strings read from the image: "%Ts-Summary" 0x18034cd28, "Bars" 0x18034cb98,
// "ScreenCX" 0x18034cb80, "ScreenCY" 0x18034cb68.  DEVIATIONS: GetProfileInt is called
// through its impl__ thunk rather than the app's vtable (OpenMFC's CWinApp has a mingw
// vtable), so a client override of GetProfileInt is not honoured; a NULL AfxGetApp() is
// passed on (the thunk returns the default) where retail would fault; the section
// formatting's ATL error throws are not reproduced (FormatSummarySection).
extern "C" void MS_ABI impl__LoadState_CDockState__QEAAXPEB_W_Z(void* pThis, const wchar_t* lpszProfileName) {
    DockStateLayout* s = DS(pThis);
    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    wchar_t szSection[256];
    FormatSummarySection(szSection, lpszProfileName);
    const int nBars = static_cast<int>(impl__GetProfileIntW_CWinApp__UEAAIPEB_W0H_Z(pApp, szSection, L"Bars", 0));
    SIZE size;
    size.cx = static_cast<LONG>(impl__GetProfileIntW_CWinApp__UEAAIPEB_W0H_Z(pApp, szSection, L"ScreenCX", 0));
    size.cy = static_cast<LONG>(impl__GetProfileIntW_CWinApp__UEAAIPEB_W0H_Z(pApp, szSection, L"ScreenCY", 0));
    impl__SetScreenSize_CDockState__QEAAXAEAVCSize___Z(pThis, &size);
    for (INT_PTR i = 0; i < s->arr_nSize; ++i) {
        std::free(s->arr_pData[i]);
    }
    ArraySetSize(s, 0, -1);
    for (int i = 0; i < nBars; ++i) {
        void* pInfo = NewBarInfo();
        ArraySetAtGrow(s, s->arr_nSize, pInfo);
        impl__LoadState_CControlBarInfo__QEAAHPEB_WHPEAVCDockState___Z(pInfo, lpszProfileName, i, pThis);
    }
}

// Symbol: ?SaveState@CDockState@@QEAAXPEB_W@Z
// Transcribed from retail 0x2211f0 (mfc140u):
//   int nIndex = 0;
//   for (i = 0; i < m_arrBarInfo.GetSize(); i++)
//       if (((CControlBarInfo*)m_arrBarInfo[i])->SaveState(lpszProfileName, nIndex))  // 0x2207a0
//           nIndex++;
//   CWinApp* pApp = AfxGetApp();
//   TCHAR szSection[256]; _sntprintf_s(szSection, 256, 255, _T("%Ts-Summary"), lpszProfileName);
//   pApp->WriteProfileInt(szSection, _T("Bars"), nIndex);                   // vslot 0x110/8 = 34
//   CSize size = GetScreenSize();                  // inlined: m_rectDevice.right-left, bottom-top
//   pApp->WriteProfileInt(szSection, _T("ScreenCX"), size.cx);
//   pApp->WriteProfileInt(szSection, _T("ScreenCY"), size.cy);
// (0x2207a0 (mfc140u) is not in the symbol map; it is CControlBarInfo::SaveState by its
// content: it formats "%Ts-Bar%d" (string at 0x18034cd40, mfc140u) with (lpszProfileName,
// nIndex), clears that section through app vslot 0x120/8 = 36 (WriteProfileString with a
// NULL entry) and then -- unless the record still holds default values, in which case it
// returns FALSE -- writes the per-bar values.  It is the Unicode counterpart of mfc140.dll's
// ?SaveState@CControlBarInfo@@QEAAHPEBDH@Z (0x21ecd0) -- NOT byte-identical to it: the
// frame and instruction count differ with the TCHAR buffer.)  DEVIATIONS as in LoadState:
// WriteProfileInt goes through its impl__ thunk, not the app's vtable; a NULL AfxGetApp()
// is passed on (the thunk returns FALSE); the formatting's ATL error throws are not
// reproduced.
extern "C" void MS_ABI impl__SaveState_CDockState__QEAAXPEB_W_Z(void* pThis, const wchar_t* lpszProfileName) {
    DockStateLayout* s = DS(pThis);
    int nIndex = 0;
    for (INT_PTR i = 0; i < s->arr_nSize; ++i) {
        if (impl__SaveState_CControlBarInfo__QEAAHPEB_WH_Z(s->arr_pData[i], lpszProfileName, nIndex))
            ++nIndex;
    }
    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    wchar_t szSection[256];
    FormatSummarySection(szSection, lpszProfileName);
    impl__WriteProfileInt_CWinApp__UEAAHPEB_W0H_Z(pApp, szSection, L"Bars", nIndex);
    const SIZE size = DeviceSize(s);
    impl__WriteProfileInt_CWinApp__UEAAHPEB_W0H_Z(pApp, szSection, L"ScreenCX", size.cx);
    impl__WriteProfileInt_CWinApp__UEAAHPEB_W0H_Z(pApp, szSection, L"ScreenCY", size.cy);
}

// Symbol: ?ScalePoint@CDockState@@QEAAXAEAVCPoint@@@Z
// Transcribed from retail 0x221440 (mfc140u):
//   if (m_bScaling) {
//       CSize sizeDevice = m_rectDevice.Size();
//       pt.x = MulDiv(pt.x, sizeDevice.cx, m_sizeLogical.cx);
//       pt.y = MulDiv(pt.y, sizeDevice.cy, m_sizeLogical.cy);
//   }
//   if (pt.x > m_rectClip.right)  pt.x = m_rectClip.right;
//   if (pt.y > m_rectClip.bottom) pt.y = m_rectClip.bottom;
// (IAT 0x1802c6658 -> KERNEL32!MulDiv.)  Only the right/bottom edges are clamped.
extern "C" void MS_ABI impl__ScalePoint_CDockState__QEAAXAEAVCPoint___Z(void* pThis, POINT* pPt) {
    DockStateLayout* s = DS(pThis);
    if (s->m_bScaling != 0) {
        const SIZE sizeDevice = DeviceSize(s);
        pPt->x = ::MulDiv(pPt->x, sizeDevice.cx, s->m_sizeLogical.cx);
        pPt->y = ::MulDiv(pPt->y, sizeDevice.cy, s->m_sizeLogical.cy);
    }
    if (pPt->x > s->m_rectClip.right) pPt->x = s->m_rectClip.right;
    if (pPt->y > s->m_rectClip.bottom) pPt->y = s->m_rectClip.bottom;
}

// Symbol: ?ScaleRectPos@CDockState@@QEAAXAEAVCRect@@@Z
// Transcribed from retail 0x2214c0 (mfc140u):
//   if (m_bScaling) {
//       CSize sizeDevice = m_rectDevice.Size();
//       int dx = MulDiv(rect.left, sizeDevice.cx, m_sizeLogical.cx) - rect.left;
//       int dy = MulDiv(rect.top,  sizeDevice.cy, m_sizeLogical.cy) - rect.top;
//       ::OffsetRect(&rect, dx, dy);                   // unconditional inside the branch
//   }
//   // then, on the (possibly moved) rect:
//   dx = rect.left > m_rectClip.right ? m_rectClip.right - rect.left
//      : rect.left < m_rectClip.left  ? m_rectClip.left  - rect.left : 0;
//   dy = rect.top  > m_rectClip.bottom ? m_rectClip.bottom - rect.top
//      : rect.top  < m_rectClip.top    ? m_rectClip.top    - rect.top : 0;
//   if (dx != 0 || dy != 0) ::OffsetRect(&rect, dx, dy);
// (IAT 0x1802c6658 -> KERNEL32!MulDiv, 0x1802c72f0 -> USER32!OffsetRect.)
extern "C" void MS_ABI impl__ScaleRectPos_CDockState__QEAAXAEAVCRect___Z(void* pThis, RECT* pRect) {
    DockStateLayout* s = DS(pThis);
    if (s->m_bScaling != 0) {
        const SIZE sizeDevice = DeviceSize(s);
        const LONG left = pRect->left;
        const LONG top = pRect->top;
        const int dx = ::MulDiv(left, sizeDevice.cx, s->m_sizeLogical.cx) - left;
        const int dy = ::MulDiv(top, sizeDevice.cy, s->m_sizeLogical.cy) - top;
        ::OffsetRect(pRect, dx, dy);
    }
    int dx;
    if (pRect->left > s->m_rectClip.right)
        dx = s->m_rectClip.right - pRect->left;
    else
        dx = (pRect->left >= s->m_rectClip.left) ? 0 : s->m_rectClip.left - pRect->left;
    int dy;
    if (pRect->top > s->m_rectClip.bottom)
        dy = s->m_rectClip.bottom - pRect->top;
    else
        dy = (pRect->top >= s->m_rectClip.top) ? 0 : s->m_rectClip.top - pRect->top;
    if (dx != 0 || dy != 0) ::OffsetRect(pRect, dx, dy);
}

// Symbol: ?Serialize@CDockState@@UEAAXAEAVCArchive@@@Z
// Transcribed from retail 0x220d50 (mfc140u; vftable slot 2):
//   if (ar.IsStoring()) {
//       ar << m_dwVersion;                                  // DWORD
//       if (m_dwVersion > 1) {
//           CSize size = GetScreenSize();
//           ar.Write(&size, sizeof(CSize));
//       }
//       ar << (WORD)m_arrBarInfo.GetSize();
//       for (i = 0; i < m_arrBarInfo.GetSize(); i++)
//           ((CControlBarInfo*)m_arrBarInfo[i])->Serialize(ar, this);
//   } else {
//       Clear();
//       ar >> m_dwVersion;
//       if (m_dwVersion > 1) {
//           CSize size(0, 0);
//           if (ar.Read(&size, sizeof(CSize)) != sizeof(CSize))
//               AfxThrowArchiveException(CArchiveException::endOfFile, NULL);
//           SetScreenSize(size);
//       }
//       WORD nOldSize; ar >> nOldSize;
//       m_arrBarInfo.SetSize(nOldSize);
//       for (i = 0; i < m_arrBarInfo.GetSize(); i++) {
//           m_arrBarInfo[i] = new CControlBarInfo;
//           ((CControlBarInfo*)m_arrBarInfo[i])->Serialize(ar, this);
//       }
//       m_dwVersion = 2;
//   }
// Retail's inline CArchive operators re-test the archive mode (the WORD store and both loads
// do; the first DWORD store's test is merged with the branch), throwing readOnly (2) /
// writeOnly (4) with ar.m_strFileName -- unreachable inside the branch that already tested it,
// and not reproduced; nor are the operator[] bounds checks, which cannot fire in these loops.
// The primitives go through ArGet/ArPut (see above) because OpenMFC's CArchive layout
// differs from retail's.  NOTE: the per-bar CControlBarInfo::Serialize is currently a no-op
// in OpenMFC (see the file header), so no bar records are streamed yet.
extern "C" void MS_ABI impl__Serialize_CDockState__UEAAXAEAVCArchive___Z(void* pThis, CArchive* ar) {
    DockStateLayout* s = DS(pThis);
    if (!ar->IsLoading()) {
        ArPut<DWORD>(ar, s->m_dwVersion);
        if (s->m_dwVersion > 1) {
            const SIZE size = DeviceSize(s);
            impl__Write_CArchive__QEAAXPEBXI_Z(ar, &size, sizeof(size));
        }
        ArPut<WORD>(ar, static_cast<WORD>(s->arr_nSize));
        for (INT_PTR i = 0; i < s->arr_nSize; ++i) {
            impl__Serialize_CControlBarInfo__QEAAXAEAVCArchive__PEAVCDockState___Z(s->arr_pData[i], ar, pThis);
        }
        return;
    }
    impl__Clear_CDockState__QEAAXXZ(pThis);
    s->m_dwVersion = ArGet<DWORD>(ar);
    if (s->m_dwVersion > 1) {
        SIZE size = {0, 0};
        if (impl__Read_CArchive__QEAAIPEAXI_Z(ar, &size, sizeof(size)) != sizeof(size))
            impl__AfxThrowArchiveException__YAXHPEB_W_Z(3 /* CArchiveException::endOfFile */, nullptr);
        impl__SetScreenSize_CDockState__QEAAXAEAVCSize___Z(pThis, &size);
    }
    const WORD nOldSize = ArGet<WORD>(ar);
    ArraySetSize(s, nOldSize, -1);
    for (INT_PTR i = 0; i < s->arr_nSize; ++i) {
        s->arr_pData[i] = NewBarInfo();
        impl__Serialize_CControlBarInfo__QEAAXAEAVCArchive__PEAVCDockState___Z(s->arr_pData[i], ar, pThis);
    }
    s->m_dwVersion = 2;
}

// Symbol: ?SetScreenSize@CDockState@@QEAAXAEAVCSize@@@Z
// Transcribed from retail 0x2215b0 (mfc140u):
//   m_sizeLogical = size;                                  // one 8-byte copy
//   m_bScaling = (size != m_rectDevice.Size());
extern "C" void MS_ABI impl__SetScreenSize_CDockState__QEAAXAEAVCSize___Z(void* pThis, SIZE* pSize) {
    DockStateLayout* s = DS(pThis);
    s->m_sizeLogical = *pSize;
    const SIZE sizeDevice = DeviceSize(s);
    s->m_bScaling = (pSize->cx == sizeDevice.cx && pSize->cy == sizeDevice.cy) ? 0 : 1;
}
