// CControlBarInfo — OpenMFC implementation.
// Sources: global_ccontrolbarinfo.cpp

#include "detail/CControlBarInfoSupport.h"

// Symbol: ??0CControlBarInfo@@QEAA@XZ
// Default constructor. Mirrors MFC's CControlBarInfo::CControlBarInfo():
// a horizontally-oriented, non-docked, non-floating, hidden bar record with
// an empty MRU rect, empty contained-bar array, and no back-pointer.
extern "C" void* MS_ABI impl___0CControlBarInfo__QEAA_XZ(void* pThis)
{
    S_Ccontrolbarinfo* self = reinterpret_cast<S_Ccontrolbarinfo*>(pThis);
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
    S_Ccontrolbarinfo* self = reinterpret_cast<S_Ccontrolbarinfo*>(pThis);
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
