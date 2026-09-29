// CMFCToolBarDropSource — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// Retail (afxtoolbardropsource.h): class CMFCToolBarDropSource : public
// COleDropSource { BOOL m_bDeleteOnDrop, m_bEscapePressed, m_bDragStarted;
// HCURSOR m_hcurDelete, m_hcurMove, m_hcurCopy; ... }.  Every body below is
// transcribed from the retail body, with the deviations spelled out at each
// one (the vptr handling in the ctor/dtor, and GiveFeedback's NULL-cursor
// path); RVAs are mfc140u.dll entries unless an mfc140.dll (ANSI twin) RVA is
// named explicitly.
//
// Layout (retail), read from the ctor (RVA 0x167120, mfc140u) and the bodies
// below: COleDropSource occupies 0x00..0x67, then
//   +0x68 BOOL    m_bDeleteOnDrop    (ctor: QWORD 1 at +0x68)
//   +0x6c BOOL    m_bEscapePressed   (QueryContinueDrag stores bEscapePressed)
//   +0x70 BOOL    m_bDragStarted     (OnBeginDrag stores 1)
//   +0x78 HCURSOR m_hcurDelete
//   +0x80 HCURSOR m_hcurMove
//   +0x88 HCURSOR m_hcurCopy
// sizeof == 0x90 (144; the harvested size of ?m_DropSource@CMFCToolBar@@, see
// detail/MfcFeature5ImplSupport.h, and the `mov $0x90,%edx` sized delete in
// the scalar deleting destructor at RVA 0x167160, mfc140u).  CMFCToolBarDropSource is not declared in
// include/openmfc, so pThis is a void* and the members are addressed through
// the accessors below.
//
// Retail's GiveFeedback also reads COleDropSource::m_bDragStarted at +0x58
// (the retail COleDropSource::GiveFeedback, RVA 0x25a440 mfc140u, is exactly
// that read).  OpenMFC's C++ COleDropSource (include/openmfc/afxole.h) does not
// have that member at +0x58 -- its layout differs from retail below +0x68 --
// so the base-class state is left to the COleDropSource impl__ thunks rather
// than read by offset here (see GiveFeedback).
//
// Base-class calls and the vptr -- same scheme as CMFCToolBarDropTarget.cpp.
// OpenMFC has no CMFCToolBarDropSource vftable.  The ctor runs the
// ??0COleDropSource thunk, which placement-constructs OpenMFC's C++
// COleDropSource and so installs its (GCC) vptr; the object keeps that vptr.
// The COleDropSource thunks called below (core/ole/Thunks.cpp,
// core/ole/COleDropSource.cpp) dispatch virtually through the object's vptr,
// so with that vptr they land on COleDropSource's own C++ methods.
// Known hazards, not fixed here:
//   * An object whose vptr is an MSVC vftable (a client class derived from
//     CMFCToolBarDropSource) would be dispatched at GCC slot indices.
//   * ?m_DropSource@CMFCToolBar@@ (CMFCToolBar.cpp) is a 144-byte blob whose
//     vptr is NULL (the ctor never runs on it); calling the base-forwarding
//     members below on it would fault in the base thunk.  Nothing in OpenMFC
//     hands that object to anything today.
//   * Because the object keeps COleDropSource's vptr, VIRTUAL calls never
//     reach the three overrides below.  OpenMFC's COleDataSource::DoDragDrop
//     (core/ole/COleDataSource.cpp) wraps the source in DropSourceAdapter
//     (detail/OlecoreSupport.h), whose IDropSource::QueryContinueDrag /
//     GiveFeedback call m_source->QueryContinueDrag / ->GiveFeedback
//     virtually, i.e. COleDropSource's own methods; it never calls
//     OnBeginDrag at all.  These bodies run only when their exports are
//     called directly.

#include "detail/ManualSmallStubImplementationsSupport.h"

namespace {

// Base COleDropSource must fit below +0x68, where the derived members begin,
// or the base ctor thunk's placement-new would overwrite them.
static_assert(sizeof(COleDropSource) == 0x68,
              "OpenMFC COleDropSource must fit below m_bDeleteOnDrop (+0x68)");

constexpr std::size_t kOffDeleteOnDrop  = 0x68;   // BOOL
constexpr std::size_t kOffEscapePressed = 0x6c;   // BOOL
constexpr std::size_t kOffDragStarted   = 0x70;   // BOOL
constexpr std::size_t kOffHcurDelete    = 0x78;   // HCURSOR
constexpr std::size_t kOffHcurMove      = 0x80;   // HCURSOR
constexpr std::size_t kOffHcurCopy      = 0x88;   // HCURSOR
constexpr std::size_t kRetailSizeof     = 0x90;
static_assert(kOffHcurCopy + sizeof(HCURSOR) == kRetailSizeof,
              "retail sizeof(CMFCToolBarDropSource) == 0x90");

template <typename T>
inline T& Member(void* pThis, std::size_t off) {
    return *reinterpret_cast<T*>(static_cast<char*>(pThis) + off);
}

// Cursor resource ids, from the immediates in OnBeginDrag; names from retail
// afxribbonres.h (IDC_AFXBARRES_DELETE 16133, _MOVE 16146, _COPY 16004).
constexpr UINT kIdcAfxBarResDelete = 0x3f05;
constexpr UINT kIdcAfxBarResMove   = 0x3f12;
constexpr UINT kIdcAfxBarResCopy   = 0x3e84;

// The vptr ??0COleDropSource@@QEAA@XZ installs, recorded by our ctor; the dtor
// re-installs it (see there).
void* g_oleDropSourceVptr = nullptr;

} // namespace

// Base class, core/ole/Thunks.cpp / core/ole/COleDropSource.cpp.
extern "C" void* MS_ABI impl___0COleDropSource__QEAA_XZ(void* pThis);
extern "C" long  MS_ABI impl__GiveFeedback_COleDropSource__UEAAJK_Z(COleDropSource* pThis, unsigned long p0);
extern "C" long  MS_ABI impl__QueryContinueDrag_COleDropSource__UEAAJHK_Z(COleDropSource* pThis, int p0, unsigned long p1);
extern "C" int   MS_ABI impl__OnBeginDrag_COleDropSource__UEAAHPEAVCWnd___Z(COleDropSource* pThis, CWnd* p0);
// detail/CWinAppSupport.cpp.
extern "C" void  MS_ABI impl___1CCmdTarget__UEAA_XZ(CCmdTarget* pThis);
// core/runtime/Globals.cpp.  (impl__AfxGetModuleState is already declared by
// detail/ManualSmallStubImplementationsSupport.h.)
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
    const wchar_t* lpszName, const wchar_t* lpszType);

namespace {

// One cursor load as OnBeginDrag does it (three identical sequences):
//     AfxGetModuleState();                                  // result discarded
//     h = AfxFindResourceHandle(MAKEINTRESOURCE(id), RT_GROUP_CURSOR /*12*/);
//     return ::LoadCursorW(h, MAKEINTRESOURCE(id));         // IAT 0x1802c71a8 (mfc140u)
inline HCURSOR LoadBarResCursor(UINT id) {
    (void)impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
    HINSTANCE hInst = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
        MAKEINTRESOURCEW(id), RT_GROUP_CURSOR);
    return ::LoadCursorW(hInst, MAKEINTRESOURCEW(id));
}

} // namespace

// Retail (RVA 0x167120, mfc140u), complete:
//     COleDropSource::COleDropSource();          // call 0x25a350 (mfc140u)
//     vptr = CMFCToolBarDropSource::vftable;
//     *(QWORD*)(this+0x68) = 1;                  // m_bDeleteOnDrop = TRUE, m_bEscapePressed = FALSE
//     m_bDragStarted = FALSE;                    // DWORD +0x70
//     m_hcurDelete = m_hcurMove = m_hcurCopy = NULL;
//     return this;
// Deviation: OpenMFC has no CMFCToolBarDropSource vtable, so the object keeps
// the COleDropSource vptr the base ctor installed (see file header).
// Symbol: ??0CMFCToolBarDropSource@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarDropSource__QEAA_XZ(void* pThis) {
    impl___0COleDropSource__QEAA_XZ(pThis);
    g_oleDropSourceVptr = *static_cast<void**>(pThis);
    Member<BOOL>(pThis, kOffDeleteOnDrop)     = TRUE;
    Member<BOOL>(pThis, kOffEscapePressed)    = FALSE;
    Member<BOOL>(pThis, kOffDragStarted)      = FALSE;
    Member<HCURSOR>(pThis, kOffHcurDelete)    = nullptr;
    Member<HCURSOR>(pThis, kOffHcurMove)      = nullptr;
    Member<HCURSOR>(pThis, kOffHcurCopy)      = nullptr;
    return pThis;
}

// Retail (RVA 0x1671c0, mfc140u), complete:
//     vptr = CMFCToolBarDropSource::vftable;
//     if (m_hcurDelete) ::DeleteObject(m_hcurDelete);   // IAT 0x1802c6278 (mfc140u) = GDI32!DeleteObject
//     if (m_hcurMove)   ::DeleteObject(m_hcurMove);
//     if (m_hcurCopy)   ::DeleteObject(m_hcurCopy);
//     jmp CCmdTarget::~CCmdTarget                         // 0x1de430 (mfc140u)
// Retail really calls GDI32!DeleteObject on the HCURSORs (not DestroyCursor);
// that is transcribed as-is.  The handles are not cleared.  mfc140u exports
// no ??1COleDropSource, and this body tail-jumps straight to CCmdTarget's.
// The ??1CCmdTarget thunk runs `p->~CCmdTarget()`, which dispatches through
// the object's vptr; retail's vptr reset is reproduced with the only vptr
// OpenMFC has for this object -- the COleDropSource one recorded by the ctor --
// so that dispatch reaches OpenMFC's ~COleDropSource (which only zeroes its
// m_lRefCount) and then ~CCmdTarget.  If the ctor never ran in this process
// the store is skipped.
// Symbol: ??1CMFCToolBarDropSource@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCToolBarDropSource__UEAA_XZ(void* pThis) {
    if (g_oleDropSourceVptr != nullptr) *static_cast<void**>(pThis) = g_oleDropSourceVptr;
    if (HCURSOR h = Member<HCURSOR>(pThis, kOffHcurDelete)) ::DeleteObject(reinterpret_cast<HGDIOBJ>(h));
    if (HCURSOR h = Member<HCURSOR>(pThis, kOffHcurMove))   ::DeleteObject(reinterpret_cast<HGDIOBJ>(h));
    if (HCURSOR h = Member<HCURSOR>(pThis, kOffHcurCopy))   ::DeleteObject(reinterpret_cast<HGDIOBJ>(h));
    impl___1CCmdTarget__UEAA_XZ(static_cast<CCmdTarget*>(pThis));
}

// Retail (RVA 0x167230, mfc140u), complete:
//     HCURSOR h;
//     switch (dropEffect) {
//     case DROPEFFECT_COPY (1): h = m_hcurCopy;   break;   // +0x88
//     case DROPEFFECT_MOVE (2): h = m_hcurMove;   break;   // +0x80
//     default:                  h = m_hcurDelete; break;   // +0x78 (incl. NONE)
//     }
//     if (h == NULL)
//         return COleDropSource::m_bDragStarted ? DRAGDROP_S_USEDEFAULTCURSORS : S_OK;
//     ::SetCursor(h);                            // IAT 0x1802c71e0 (mfc140u)
//     return S_OK;
// Deviation: the NULL-cursor return is retail's inlined copy of
// COleDropSource::GiveFeedback (byte-identical to that export, RVA 0x25a440
// mfc140u: `mov 0x58(%rcx),%eax; neg; sbb; and $0x40102`).  OpenMFC's
// COleDropSource keeps no m_bDragStarted at +0x58 (see file header), so this
// calls the base thunk instead of reading +0x58.  The result is NOT always
// retail's: OpenMFC's COleDropSource::GiveFeedback (core/ole/COleDropSource.cpp)
// unconditionally returns DRAGDROP_S_USEDEFAULTCURSORS, while retail returns
// S_OK when the base m_bDragStarted is FALSE.  They agree once a drag has
// started (retail's base OnBeginDrag leaves m_bDragStarted TRUE on success).
// Symbol: ?GiveFeedback@CMFCToolBarDropSource@@UEAAJK@Z
extern "C" long MS_ABI impl__GiveFeedback_CMFCToolBarDropSource__UEAAJK_Z(void* pThis, unsigned long dropEffect) {
    HCURSOR h;
    switch (dropEffect) {
    case DROPEFFECT_COPY: h = Member<HCURSOR>(pThis, kOffHcurCopy);   break;
    case DROPEFFECT_MOVE: h = Member<HCURSOR>(pThis, kOffHcurMove);   break;
    default:              h = Member<HCURSOR>(pThis, kOffHcurDelete); break;
    }
    if (h == nullptr)
        return impl__GiveFeedback_COleDropSource__UEAAJK_Z(static_cast<COleDropSource*>(pThis), dropEffect);
    ::SetCursor(h);
    return S_OK;
}

// Retail (RVA 0x1672d0, mfc140u), complete:
//     if (m_hcurDelete == NULL) {                // only +0x78 is tested
//         m_hcurDelete = <load IDC_AFXBARRES_DELETE>;   // 0x3f05
//         m_hcurMove   = <load IDC_AFXBARRES_MOVE>;     // 0x3f12
//         m_hcurCopy   = <load IDC_AFXBARRES_COPY>;     // 0x3e84
//     }
//     m_bDragStarted = TRUE;                     // movl $1,0x70
//     return COleDropSource::OnBeginDrag(pWnd);  // tail jmp 0x25a450 (mfc140u)
// Each load is the LoadBarResCursor sequence above (AfxGetModuleState 0x133930,
// AfxFindResourceHandle 0x2aeb50, USER32!LoadCursorW).  The base call is made
// through the COleDropSource thunk (see file header).
// Symbol: ?OnBeginDrag@CMFCToolBarDropSource@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__OnBeginDrag_CMFCToolBarDropSource__UEAAHPEAVCWnd___Z(void* pThis, CWnd* pWnd) {
    if (Member<HCURSOR>(pThis, kOffHcurDelete) == nullptr) {
        Member<HCURSOR>(pThis, kOffHcurDelete) = LoadBarResCursor(kIdcAfxBarResDelete);
        Member<HCURSOR>(pThis, kOffHcurMove)   = LoadBarResCursor(kIdcAfxBarResMove);
        Member<HCURSOR>(pThis, kOffHcurCopy)   = LoadBarResCursor(kIdcAfxBarResCopy);
    }
    Member<BOOL>(pThis, kOffDragStarted) = TRUE;
    return impl__OnBeginDrag_COleDropSource__UEAAHPEAVCWnd___Z(static_cast<COleDropSource*>(pThis), pWnd);
}

// Retail (RVA 0x167280, mfc140u -- this entry is absent from
// mfc140u_rva_symbols.json; it is the mfc140.dll export at RVA 0x1658d0, whose
// instruction sequence appears at mfc140u 0x167280, between GiveFeedback and
// OnBeginDrag, identical except for rip-relative/branch targets; and slot 22
// of the CMFCToolBarDropSource vftable at 0x1803178d8 (mfc140u), the slot just
// before GiveFeedback (23) and OnBeginDrag (24), holds 0x167280), complete:
//     if (m_bDeleteOnDrop && m_hcurDelete != NULL)
//         ::SetCursor(m_hcurDelete);             // IAT 0x1802c71e0 (mfc140u)
//     m_bEscapePressed = bEscapePressed;         // mov %edi,0x6c(%rbx)
//     return COleDropSource::QueryContinueDrag(bEscapePressed, dwKeyState);
//                                                // tail jmp 0x25a410 (mfc140u; mfc140.dll
//                                                // export 0x259490, also absent from the u map)
// The base call is made through the COleDropSource thunk (see file header).
// Symbol: ?QueryContinueDrag@CMFCToolBarDropSource@@UEAAJHK@Z
extern "C" long MS_ABI impl__QueryContinueDrag_CMFCToolBarDropSource__UEAAJHK_Z(
    void* pThis, int bEscapePressed, unsigned long dwKeyState) {
    if (Member<BOOL>(pThis, kOffDeleteOnDrop) != 0) {
        if (HCURSOR h = Member<HCURSOR>(pThis, kOffHcurDelete)) ::SetCursor(h);
    }
    Member<BOOL>(pThis, kOffEscapePressed) = bEscapePressed;
    return impl__QueryContinueDrag_COleDropSource__UEAAJHK_Z(
        static_cast<COleDropSource*>(pThis), bEscapePressed, dwKeyState);
}
