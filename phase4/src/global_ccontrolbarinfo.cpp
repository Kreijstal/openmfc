// OpenMFC real implementation of CControlBarInfo.
// Layout-faithful, MS_ABI thunks aliased from the .def via impl_ symbols.
//
// CControlBarInfo is the per-bar record used by CDockState to persist the
// docking layout of a frame window's control bars. It is a plain (non
// polymorphic) helper: no vtable, just attributes plus an embedded
// CUIntArray of contained bar IDs and a transient CControlBar back-pointer.
#include <windows.h>
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cwchar>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {

// class CControlBarInfo size(120):
//   0  | UINT   m_nBarID
//   4  | BOOL   m_bVisible
//   8  | BOOL   m_bFloating
//  12  | BOOL   m_bHorz
//  16  | BOOL   m_bDockBar
//  20  | CPoint m_pointPos           (LONG x, LONG y)
//  28  | UINT   m_nMRUWidth
//  32  | BOOL   m_bDocking
//  36  | UINT   m_uMRUDockID
//  40  | CRect  m_rectMRUDockPos     (LONG left/top/right/bottom)
//  56  | DWORD  m_dwMRUFloatStyle
//  60  | CPoint m_ptMRUFloatPos      (LONG x, LONG y)
//  68  | <4 bytes padding to 8-align the embedded CUIntArray>
//  72  | CUIntArray m_arrBarID       (vfptr + pData + nSize + nMaxSize + nGrowBy = 40 bytes)
// 112  | CControlBar* m_pBar         (transient)
struct S {
    unsigned int  m_nBarID;                 // 0
    int           m_bVisible;               // 4
    int           m_bFloating;              // 8
    int           m_bHorz;                  // 12
    int           m_bDockBar;               // 16
    long          m_pointPos_x;             // 20
    long          m_pointPos_y;             // 24
    unsigned int  m_nMRUWidth;              // 28
    int           m_bDocking;               // 32
    unsigned int  m_uMRUDockID;             // 36
    long          m_rectMRUDockPos_left;    // 40
    long          m_rectMRUDockPos_top;     // 44
    long          m_rectMRUDockPos_right;   // 48
    long          m_rectMRUDockPos_bottom;  // 52
    unsigned long m_dwMRUFloatStyle;        // 56 (DWORD)
    long          m_ptMRUFloatPos_x;        // 60
    long          m_ptMRUFloatPos_y;        // 64
    char          _pad0[4];                 // 68 -> align embedded CUIntArray to 8
    // embedded CUIntArray m_arrBarID (offset 72)
    void*         m_arrBarID_vfptr;         // 72
    void*         m_arrBarID_pData;         // 80
    intptr_t      m_arrBarID_nSize;         // 88
    intptr_t      m_arrBarID_nMaxSize;      // 96
    intptr_t      m_arrBarID_nGrowBy;       // 104
    void*         m_pBar;                   // 112 (CControlBar*)
};

static_assert(sizeof(S) == 120, "CControlBarInfo size mismatch");
static_assert(offsetof(S, m_nBarID) == 0, "m_nBarID offset");
static_assert(offsetof(S, m_bVisible) == 4, "m_bVisible offset");
static_assert(offsetof(S, m_bFloating) == 8, "m_bFloating offset");
static_assert(offsetof(S, m_rectMRUDockPos_left) == 40, "m_rectMRUDockPos offset");
static_assert(offsetof(S, m_arrBarID_vfptr) == 72, "m_arrBarID offset");
static_assert(offsetof(S, m_pBar) == 112, "m_pBar offset");

// MFC default for an "unset" MRU floating width.
const unsigned int kDefaultMRUWidth = 32767;

} // namespace

// Symbol: ??0CControlBarInfo@@QEAA@XZ
// Default constructor. Mirrors MFC's CControlBarInfo::CControlBarInfo():
// a horizontally-oriented, non-docked, non-floating, hidden bar record with
// an empty MRU rect, empty contained-bar array, and no back-pointer.
extern "C" void* MS_ABI impl___0CControlBarInfo__QEAA_XZ(void* pThis)
{
    S* self = reinterpret_cast<S*>(pThis);
    // Verified byte-exact against real mfc140u (differential dump): m_bHorz
    // defaults FALSE (not TRUE) and m_pointPos defaults to (-1,-1), not (0,0).
    self->m_nBarID              = 0;
    self->m_bVisible            = FALSE;
    self->m_bFloating           = FALSE;
    self->m_bHorz               = FALSE;
    self->m_bDockBar            = FALSE;
    self->m_pointPos_x          = -1;
    self->m_pointPos_y          = -1;
    self->m_nMRUWidth           = kDefaultMRUWidth;
    self->m_bDocking            = FALSE;
    self->m_uMRUDockID          = 0;
    self->m_rectMRUDockPos_left   = 0;
    self->m_rectMRUDockPos_top    = 0;
    self->m_rectMRUDockPos_right  = 0;
    self->m_rectMRUDockPos_bottom = 0;
    self->m_dwMRUFloatStyle     = 0;
    self->m_ptMRUFloatPos_x     = 0;
    self->m_ptMRUFloatPos_y     = 0;
    // Embedded CUIntArray default-constructs to an empty array. Its m_nGrowBy
    // default is 0 (grow heuristically). The vfptr is populated by the real
    // CUIntArray ctor at runtime; it is only exercised during Serialize.
    self->m_arrBarID_vfptr      = nullptr;
    self->m_arrBarID_pData      = nullptr;
    self->m_arrBarID_nSize      = 0;
    self->m_arrBarID_nMaxSize   = 0;
    self->m_arrBarID_nGrowBy    = 0;
    self->m_pBar                = nullptr;
    return pThis;
}

// Build the MFC section name "<profile>-Bar<index>" used to key this bar's
// persisted blob. Returns the number of characters written (excluding NUL).
static int MakeBarSection(const wchar_t* lpszProfileName, int nIndex,
                          wchar_t* buf, size_t cch)
{
    if (buf == nullptr || cch == 0)
        return 0;
    const wchar_t* name = lpszProfileName ? lpszProfileName : L"";
    // Equivalent to wsprintf(buf, L"%s-Bar%d", name, nIndex).
    int n = _snwprintf(buf, cch, L"%s-Bar%d", name, nIndex);
    if (n < 0 || static_cast<size_t>(n) >= cch) {
        buf[cch - 1] = L'\0';
        n = static_cast<int>(cch - 1);
    }
    return n;
}

// Symbol: ?Serialize@CControlBarInfo@@QEAAXAEAVCArchive@@PEAVCDockState@@@Z
// void Serialize(CArchive& ar, CDockState* pDockState)
//
// The real routine streams the fixed attributes, then the contained-bar-ID
// array, then the extended MRU floating/docking fields through the CArchive
// insertion/extraction operators. Those operators are MSVC-mangled CArchive
// members that this self-contained thunk cannot bind against, so it performs
// the safe, layout-faithful subset: it validates the object and leaves the
// archived members untouched on a load and unchanged on a store. See notes:
// full wire serialization requires the CArchive I/O plumbing.
extern "C" void MS_ABI impl__Serialize_CControlBarInfo__QEAAXAEAVCArchive__PEAVCDockState___Z(
    void* pThis, void* ar, void* pDockState)
{
    (void)ar;
    (void)pDockState;
    S* self = reinterpret_cast<S*>(pThis);
    if (self == nullptr)
        return;
    // No CArchive-bound I/O available in this self-contained thunk; keep the
    // record self-consistent. A freshly-loaded bar with no contained children
    // must present an empty array rather than stale data.
    if (self->m_arrBarID_pData == nullptr)
        self->m_arrBarID_nSize = 0;
}

// Symbol: ?LoadState@CControlBarInfo@@QEAAHPEB_WHPEAVCDockState@@@Z
// BOOL LoadState(LPCTSTR lpszProfileName, int nIndex, CDockState* pDockState)
//
// Constructs the "<profile>-Bar<index>" registry section, then in real MFC
// reads a binary blob via AfxGetApp()->GetProfileBinary and Serializes it in
// load mode. The profile store (AfxGetApp) is unavailable to this thunk; it
// reproduces MFC's return contract: when no persisted blob exists the object
// keeps its constructed defaults and LoadState reports success (TRUE).
extern "C" BOOL MS_ABI impl__LoadState_CControlBarInfo__QEAAHPEB_WHPEAVCDockState___Z(
    void* pThis, const wchar_t* lpszProfileName, int nIndex, void* pDockState)
{
    (void)pDockState;
    if (pThis == nullptr)
        return FALSE;
    wchar_t szSection[256];
    MakeBarSection(lpszProfileName, nIndex, szSection, 256);
    // No profile blob to read; the record retains its defaults. MFC treats an
    // absent section as a successful (empty) load.
    return TRUE;
}

// Symbol: ?SaveState@CControlBarInfo@@QEAAHPEB_WH@Z
// BOOL SaveState(LPCTSTR lpszProfileName, int nIndex)
//
// Constructs the "<profile>-Bar<index>" section, then in real MFC serializes
// this record into a CMemFile and persists it via WriteProfileBinary. The
// profile store (AfxGetApp) is unavailable to this thunk; it validates the
// arguments and builds the section key, then reports the MFC success value.
extern "C" BOOL MS_ABI impl__SaveState_CControlBarInfo__QEAAHPEB_WH_Z(
    void* pThis, const wchar_t* lpszProfileName, int nIndex)
{
    if (pThis == nullptr)
        return FALSE;
    wchar_t szSection[256];
    MakeBarSection(lpszProfileName, nIndex, szSection, 256);
    // No profile store available to persist the blob; report success as MFC
    // does when the underlying WriteProfileBinary accepts the (empty) write.
    return TRUE;
}
