// COleControlContainer — OpenMFC implementation.
// Sources: ole_olectors_exports.cpp, olecore.cpp
//
//=============================================================================
// Retail-decoded implementations
//
// Everything from AttachControlSite down was decoded from the retail export
// (mangled name -> RVA -> objdump), the method this project uses elsewhere
// (see the header of core/ole/COleControl.cpp).  The class members above it
// (constructor, destructor, CreateSite, DeleteSite, FindItem) are OpenMFC's
// own pre-existing code and were not touched, apart from the one cleanup call
// added to the destructor for the side table introduced below.  Each function
// names the RVA it was transcribed from; those RVAs are mfc140.dll's, which is
// the MBCS build, so the ...W entry points appear there under their ...A
// names and their bodies are transcribed here with the Unicode CRT/Win32
// equivalents.
//
// The retail member layout is NOT OpenMFC's.  The offsets quoted below are
// the ones the shipping code actually dereferences; this is how they were
// mapped onto OpenMFC state (the container vftable is at RVA 0x328be8, and
// the slot numbers quoted below were read out of it):
//
//   retail +0x40  m_pWnd (CWnd*)            -> COleControlContainer::m_pWnd
//   retail +0x48  m_siteMap (CMapPtrToPtr,  -> no OpenMFC equivalent; the
//                 HWND -> COleControlSite*)    lookups it serves are done by
//                                              scanning m_listSites instead
//   retail +0x80  m_listSitesOrWnds         -> COleControlContainer::m_listSites
//                 A CPtrList.  The code reaches its head at +0x88 and its count
//                 at +0x98 directly; +0x80 is the CObject vptr in front of
//                 them.  Retail's elements are 24-byte COleControlSiteOrWnd
//                 records {CWnd*/HWND at +0x00, COleControlSite* at +0x08,
//                 DWORD at +0x10}, whereas OpenMFC's list holds
//                 COleControlSite* directly -- so every "entry->pSite" in the
//                 transcriptions below is simply the list element here.
//   retail +0xc0  m_pOleFont (IFont*)       -> ContainerExtra::pOleFont below
//   retail +0xb8  cached ambient back colour-> ContainerExtra::clrBack below
//   retail +0xbc  cached ambient fore colour-> ContainerExtra::clrFore below
//   retail +0xc8  UI-active site            -> ContainerExtra::pSiteUIActive
//   retail +0xd8  the site that took the focus -> no OpenMFC equivalent.
//                 HandleSetFocus (0x234ad0) returns whether it is non-null and
//                 OnUIDeactivate (0x2346a0) clears it, while the store comes
//                 from another translation unit: COleControlSite::SetFocus(MSG*)
//                 (0x23a560) puts the site there after a successful UIACTIVATE
//                 verb, as already recorded at
//                 core/ole/COleControlSite.cpp:1341.
//   retail +0xe8  nested XOleIPFrame        -> no OpenMFC equivalent
//   retail +0xf0  nested XOleContainer      -> no OpenMFC equivalent
//
// Site offsets referenced below use the map at the head of
// core/ole/COleControlSite.cpp: +0x48 m_hWnd, +0x80 IOleObject,
// +0x88 IOleInPlaceObject, +0xac m_dwStyle, +0xb0 cached misc status,
// +0xb4 windowless-active flag.
//
// The two nested-interface parts (XOleIPFrame at +0xe8, XOleContainer at
// +0xf0) do not exist in OpenMFC's COleControlContainer at all, so a nested
// method cannot recover its owning container from its own `this` the way the
// retail bodies do by subtracting 0xe8 / 0xf0 from it (visible as the
// `add $-0xe8,%rcx` in XOleIPFrame::QueryInterface at 0x235050, the
// `add $-0xf0,%rcx` in XOleContainer::QueryInterface at 0x235200, and the
// negative displacements in the bodies themselves).  The nested
// methods whose retail body is a constant are reproduced exactly; the three
// that read container state say so and are reported as stubs.
//=============================================================================

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#include <cstddef>
#include <cstdlib>
#include <olectl.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Ambient DISPIDs the shared support header does not already define.
#ifndef DISPID_AMBIENT_AUTOCLIP
#define DISPID_AMBIENT_AUTOCLIP        (-715)
#endif
#ifndef DISPID_AMBIENT_MESSAGEREFLECT
#define DISPID_AMBIENT_MESSAGEREFLECT  (-706)
#endif

//-----------------------------------------------------------------------------
// Thunks used from this file.  C++ methods of other translation units exist in
// this DLL only as their extern "C" impl__ aliases, so they are called through
// those; each name below was verified to have a definition in phase4/src.
//-----------------------------------------------------------------------------
extern "C" int MS_ABI impl__GetWindowTextW_CWnd__QEBAHPEA_WH_Z(
    const CWnd* pThis, wchar_t* lpszStringBuf, int nMaxCount);
extern "C" void MS_ABI impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(
    CWnd* pThis, const wchar_t* lpszString);
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" int MS_ABI impl__CreateControlSite_CWnd__MEAAHPEAVCOleControlContainer__PEAPEAVCOleControlSite__IAEBU_GUID___Z(
    CWnd* pThis, COleControlContainer* pContainer, COleControlSite** ppSite,
    unsigned int nID, const GUID* pClsid);
extern "C" void MS_ABI impl__FreezeEvents_COleControlSite__QEAAXH_Z(
    COleControlSite* pThis, int bFreeze);
extern "C" void MS_ABI impl__GetProperty_COleControlSite__UEBAXJGPEAX_Z(
    COleControlSite* pThis, long dispid, unsigned short vtProp, void* pvProp);
extern "C" int MS_ABI impl__SafeSetProperty_COleControlSite__UEAAHJGZZ(
    COleControlSite* pThis, long dispid, unsigned short vtProp, ...);
extern "C" long MS_ABI impl__CreateControl_COleControlSite__UEAAJPEAVCWnd__AEBU_GUID__PEB_WKAEBUtagRECT__IPEAVCFile__HPEA_W_Z(
    COleControlSite* pThis, CWnd* pWndCtrl, const _GUID* pClsid, const wchar_t* lpszWindowName,
    unsigned long dwStyle, const RECT* prect, unsigned int nID, CFile* pPersist,
    int bStorage, wchar_t* bstrLicKey);
// CCmdTarget::GetThisDispatchMap -- the base map COleControlContainer's own
// AFX_DISPMAP chains to (retail 0x328d38+0x00 holds 0x1dc780, which the RVA map
// names ?GetThisDispatchMap@CCmdTarget@@KAPEBUAFX_DISPMAP@@XZ).  Defined in
// core/runtime/CCmdTarget.cpp:998.
extern "C" const void* MS_ABI impl__GetThisDispatchMap_CCmdTarget__KAPEBUAFX_DISPMAP__XZ();

// Forward declarations of this file's own exports that other bodies here call,
// exactly as the retail bodies call them through the container vftable.
extern "C" void MS_ABI impl__CheckDlgButton_COleControlContainer__UEAAXHI_Z(
    COleControlContainer* pThis, int nIDButton, unsigned int nCheck);
extern "C" void MS_ABI impl__GetDlgItem_COleControlContainer__UEBAXHPEAPEAUHWND_____Z(
    const COleControlContainer* pThis, int nID, HWND* phWnd);
extern "C" CWnd* MS_ABI impl__GetDlgItem_COleControlContainer__UEBAPEAVCWnd__H_Z(
    const COleControlContainer* pThis, int nID);
extern "C" int MS_ABI impl__GetDlgItemTextW_COleControlContainer__UEBAHHPEA_WH_Z(
    const COleControlContainer* pThis, int nID, wchar_t* lpStr, int nMaxCount);
extern "C" void MS_ABI impl__SetDlgItemTextW_COleControlContainer__UEAAXHPEB_W_Z(
    COleControlContainer* pThis, int nID, const wchar_t* lpszString);
extern "C" void MS_ABI impl__CreateOleFont_COleControlContainer__QEAAXPEAVCFont___Z(
    COleControlContainer* pThis, CFont* pFont);
extern "C" int MS_ABI impl__CreateControl_COleControlContainer__QEAAHPEAVCWnd__AEBU_GUID__PEB_WKPEBUtagPOINT__PEBUtagSIZE__IPEAVCFile__HPEA_WPEAPEAVCOleControlSite___Z(
    COleControlContainer* pThis, CWnd* pWndCtrl, const GUID* pClsid, const wchar_t* lpszWindowName,
    unsigned long dwStyle, const POINT* ppt, const SIZE* psize, unsigned int nID,
    CFile* pPersist, int bStorage, wchar_t* bstrLicKey, COleControlSite** ppSite);
extern "C" int MS_ABI impl__CreateControlCommon_COleControlContainer__QEAAHPEAVCWnd__AEBU_GUID__AEBVCControlCreationInfo__PEB_WKPEBUtagPOINT__PEBUtagSIZE__IPEAVCFile__HPEA_WPEAPEAVCOleControlSite___Z(
    COleControlContainer* pThis, CWnd* pWndCtrl, const GUID* pClsid, const void* pInfo,
    const wchar_t* lpszWindowName, unsigned long dwStyle, const POINT* ppt, const SIZE* psize,
    unsigned int nID, CFile* pPersist, int bStorage, wchar_t* bstrLicKey,
    COleControlSite** ppSite);

namespace {

// Container state the retail class keeps in members OpenMFC's
// COleControlContainer does not declare (see the offset map above).  This is
// the same device core/ole/COleControlSite.cpp uses for the site's extra
// members; adding real members would change the class layout, which is
// ABI-load-bearing.
// (A hand-rolled list rather than a std:: container: the per-file link audit
// treats libstdc++'s out-of-line red-black-tree helpers as new undefined C++
// symbols.)
struct ContainerExtra {
    const COleControlContainer* owner         = nullptr;
    ContainerExtra*             next          = nullptr;
    COleControlSite*            pSiteUIActive = nullptr;                    // retail +0xc8
    IFont*                      pOleFont      = nullptr;                    // retail +0xc0
    // Never written: the only retail writer of site +0x90 is
    // XOleIPFrame::SetActiveObject (0x2350a0), which is a stub here because its
    // nested `this` cannot be turned back into a container.  Kept (and released
    // below) so the field is in place if that ever becomes reachable.
    IOleInPlaceActiveObject*    pActiveObject = nullptr;                    // retail: site +0x90
    COLORREF                    clrBack       = static_cast<COLORREF>(-1);  // retail +0xb8
    COLORREF                    clrFore       = static_cast<COLORREF>(-1);  // retail +0xbc
};

ContainerExtra* g_containerExtraHead = nullptr;

ContainerExtra& ExtraOf(const COleControlContainer* p) {
    for (ContainerExtra* e = g_containerExtraHead; e; e = e->next) {
        if (e->owner == p) {
            return *e;
        }
    }
    ContainerExtra* e = new ContainerExtra();
    e->owner = p;
    e->next = g_containerExtraHead;
    g_containerExtraHead = e;
    return *e;
}

void ReleaseContainerExtra(const COleControlContainer* p) {
    ContainerExtra** pp = &g_containerExtraHead;
    while (*pp) {
        if ((*pp)->owner == p) {
            ContainerExtra* e = *pp;
            *pp = e->next;
            if (e->pOleFont) {
                e->pOleFont->Release();
            }
            if (e->pActiveObject) {
                e->pActiveObject->Release();
            }
            delete e;
            return;
        }
        pp = &(*pp)->next;
    }
}

// The layout of CControlCreationInfo as pinned in
// phase4/src/detail/CControlCreationInfoSupport.h (HandleKind m_hk at 0,
// intptr_t m_nHandle at 8, CLSID m_clsid at 16; total size 32).
struct CreationInfoLayout {
    int      m_hk;
    int      _pad0;
    intptr_t m_nHandle;
    CLSID    m_clsid;
};
static_assert(sizeof(CreationInfoLayout) == 32, "CControlCreationInfo is 32 bytes");
static_assert(offsetof(CreationInfoLayout, m_nHandle) == 8, "m_nHandle at +0x08");
static_assert(offsetof(CreationInfoLayout, m_clsid) == 16, "m_clsid at +0x10");
const int kCreationInfoNullHandle = 2;   // HandleKind::NullHandle

//-----------------------------------------------------------------------------
// COleControlContainer's own AFX_DISPMAP, rebuilt from retail .rdata.
//
// The class's map lives at RVA 0x328d38 and its four qwords were read out of
// the image:
//     +0x00 pfnGetBaseMap  = 0x1dc780 = CCmdTarget::GetThisDispatchMap
//     +0x08 lpEntries      = 0x346ba0
//     +0x10 lpEntryCount   = 0x3ab728  (holds 0xffffffff, the "not counted
//                                       yet" sentinel BEGIN_DISPATCH_MAP seeds)
//     +0x18 lpStockPropMask= 0x3ab72c  (holds 0xffffffff, same sentinel)
// The entry array at 0x346ba0 is a single 0x40-byte terminator record whose 64
// bytes are, verbatim:
//     00..07 lpszName    = NULL
//     08..0f lDispID     = 0xffffffff (-1)
//     10..17 lpszParams  = NULL
//     18..1f vt          = 0x0018 = VT_VOID
//     20..2f pfn/pfnSet  = NULL
//     30..37 nPropOffset = (size_t)-1        <- the terminator test
//     38..3f flags       = 0
// i.e. COleControlContainer declares no dispatch entries of its own and simply
// chains to CCmdTarget.  The structure layout below mirrors the one pinned by
// static_asserts in core/runtime/CCmdTarget.cpp:100-120 (that translation unit
// declares the types privately, so they are re-declared here rather than
// shared); the same static_asserts are repeated so the two cannot drift.
struct DispMapEntryLayout {
    const wchar_t* lpszName;     // +0x00
    long           lDispID;      // +0x08
    const char*    lpszParams;   // +0x10
    unsigned short vt;           // +0x18
    void*          pfn;          // +0x20
    void*          pfnSet;       // +0x28
    size_t         nPropOffset;  // +0x30
    int            flags;        // +0x38
};
static_assert(offsetof(DispMapEntryLayout, lDispID) == 0x08, "lDispID at +0x08");
static_assert(offsetof(DispMapEntryLayout, vt) == 0x18, "vt at +0x18");
static_assert(offsetof(DispMapEntryLayout, nPropOffset) == 0x30, "nPropOffset at +0x30");
static_assert(sizeof(DispMapEntryLayout) == 0x40, "AFX_DISPMAP_ENTRY stride is 0x40");

typedef const void* (MS_ABI* PfnGetBaseDispMap)();

struct DispMapLayout {
    PfnGetBaseDispMap    pfnGetBaseMap;    // +0x00
    const DispMapEntryLayout* lpEntries;   // +0x08
    unsigned int*        lpEntryCount;     // +0x10
    unsigned long*       lpStockPropMask;  // +0x18
};
static_assert(sizeof(DispMapLayout) == 0x20, "AFX_DISPMAP is four pointers");

const DispMapEntryLayout g_dispEntries_COleControlContainer[] = {
    { nullptr, -1, nullptr, /*VT_VOID*/ 24, nullptr, nullptr, (size_t)-1, 0 }
};
unsigned int  g_dispEntryCount_COleControlContainer   = 0xffffffffu;
unsigned long g_dispStockPropMask_COleControlContainer = 0xffffffffu;
const DispMapLayout g_dispatchMap_COleControlContainer = {
    &impl__GetThisDispatchMap_CCmdTarget__KAPEBUAFX_DISPMAP__XZ,
    g_dispEntries_COleControlContainer,
    &g_dispEntryCount_COleControlContainer,
    &g_dispStockPropMask_COleControlContainer
};

HWND ContainerHwnd(const COleControlContainer* pThis) {
    return (pThis && pThis->m_pWnd) ? pThis->m_pWnd->m_hWnd : nullptr;
}

// Walks m_listSites.  Retail walks m_listSitesOrWnds and takes entry->pSite
// (+0x08) of each element, skipping the elements that carry only a window.
template <class F>
void ForEachSite(const COleControlContainer* pThis, F fn) {
    if (!pThis) {
        return;
    }
    CPtrList::POSITION pos = pThis->m_listSites.GetHeadPosition();
    while (pos != CPtrList::POSITION(nullptr)) {
        COleControlSite* pSite = static_cast<COleControlSite*>(pThis->m_listSites.GetNext(pos));
        if (pSite) {
            fn(pSite);
        }
    }
}

// Retail keeps the control's OLEMISC_* word cached in the site at +0xb0.
// OpenMFC's site has no such member (see the map in core/ole/COleControlSite.cpp),
// so it is queried from IOleObject on demand.
DWORD SiteMiscStatusOf(COleControlSite* pSite) {
    DWORD dwMisc = 0;
    if (pSite && pSite->m_lpObject) {
        if (FAILED(pSite->m_lpObject->GetMiscStatus(DVASPECT_CONTENT, &dwMisc))) {
            dwMisc = 0;
        }
    }
    return dwMisc;
}

}  // namespace

// Symbol: ??0COleControlContainer@@QEAA@PEAVCWnd@@@Z
// COleControlContainer::COleControlContainer(CWnd* pWnd)
extern "C" void* MS_ABI impl___0COleControlContainer__QEAA_PEAVCWnd___Z(void* p0) {
    return new COleControlContainer(static_cast<CWnd*>(p0));
}
COleControlContainer::COleControlContainer(CWnd* pWnd)
    : m_pWnd(pWnd) {
    memset(_olecontrolcontainer_padding, 0, sizeof(_olecontrolcontainer_padding));
}
COleControlContainer::~COleControlContainer() {
    // Cleanup sites without using POSITION
    while (!m_listSites.IsEmpty()) {
        COleControlSite* pSite = (COleControlSite*)m_listSites.RemoveHead();
        delete pSite;
    }
    // Release the interfaces this file parks in the ContainerExtra side table.
    // Retail's ~COleControlContainer (0x233a60) does the same for m_pOleFont:
    // at 0x233b0e it passes &m_pOleFont (+0xc0) to the helper at 0x26ba84,
    // which is `if (*pp) { (*pp)->Release(); *pp = NULL; }`.
    ReleaseContainerExtra(this);
}
COleControlSite* COleControlContainer::CreateSite(COleControlContainer* pCtrlCont) {
    COleControlSite* pSite = new COleControlSite(pCtrlCont);
    m_listSites.AddTail(pSite);
    return pSite;
}
BOOL COleControlContainer::DeleteSite(COleControlSite* pSite) {
    // Look for pSite in the list and remove it
    CPtrList::POSITION pos = m_listSites.Find(pSite);
    if (pos != CPtrList::POSITION(nullptr)) {
        m_listSites.RemoveAt(pos);
        delete pSite;
        return TRUE;
    }
    return FALSE;
}
COleControlSite* COleControlContainer::FindItem(UINT nID) const {
    CPtrList::POSITION pos = m_listSites.GetHeadPosition();
    while (pos != CPtrList::POSITION(nullptr)) {
        COleControlSite* pSite = static_cast<COleControlSite*>(m_listSites.GetNext(pos));
        if (!pSite) {
            continue;
        }

        UINT siteId = 0;
        if (TryGetControlSiteId(pSite, &siteId) && siteId == nID) {
            return pSite;
        }

        HWND hWnd = pSite->m_hWnd;
        if (hWnd && ::IsWindow(hWnd) && static_cast<UINT>(::GetDlgCtrlID(hWnd)) == nID) {
            return pSite;
        }
    }
    return nullptr;
}

// Symbol: ?AttachControlSite@COleControlContainer@@QEAAXPEAVCWnd@@I@Z
// COleControlContainer::AttachControlSite(CWnd*, UINT) — retail 0x2a8310:
//     COleControlSite* pSite = (nIDC == 0)
//         ? (COleControlSite*)m_siteMap(+0x48).GetValueAt(pWnd->m_hWnd)  // 0x2313b0
//         : FindItem(nIDC);                                              // vftable slot 0xb8
//     if (pSite != NULL) {
//         CWnd* pOld = pSite->m_pWndCtrl(+0x50);
//         if (pOld != NULL && pOld->m_pCtrlSite(+0xd0) == pSite)
//             pOld->m_pCtrlSite = NULL;
//         pWnd->m_pCtrlSite(+0xd0) = pSite;
//         pSite->m_pWndCtrl(+0x50) = pWnd;
//     }
// Left a stub: every store this function makes targets a member OpenMFC does
// not declare.  COleControlSite has no m_pWndCtrl and CWnd has no
// m_pCtrlSite (both recorded as missing in the layout map at the head of
// core/ole/COleControlSite.cpp), and the container has no m_siteMap, so the
// whole body would be a lookup whose result is then discarded.
extern "C" void MS_ABI impl__AttachControlSite_COleControlContainer__QEAAXPEAVCWnd__I_Z(
    COleControlContainer* pThis, CWnd* pWnd, unsigned int nIDC) {
    (void)pThis; (void)pWnd; (void)nIDC;
}

// Symbol: ?BroadcastAmbientPropertyChange@COleControlContainer@@UEAAXJ@Z
// COleControlContainer::BroadcastAmbientPropertyChange(DISPID) — retail
// 0x233bc0.  Walks m_listSitesOrWnds from its head (+0x88) and for every
// element whose site (+0x08) is non-null and whose site has an IOleObject
// (+0x80):
//     QueryInterface(IID_IOleControl, GUID at 0x2d7c38) on the IOleObject;
//     if it succeeds, OnAmbientPropertyChange(dispid) (IOleControl slot 5 =
//     0x28) then Release (slot 2 = 0x10).
// The GUID at 0x2d7c38 is identified as IID_IOleControl by
// COleControlSite::FreezeEvents (0x2399c0), which uses the same one.
extern "C" void MS_ABI impl__BroadcastAmbientPropertyChange_COleControlContainer__UEAAXJ_Z(
    COleControlContainer* pThis, long dispid) {
    ForEachSite(pThis, [dispid](COleControlSite* pSite) {
        if (!pSite->m_lpObject) {
            return;
        }
        IOleControl* pControl = nullptr;
        if (FAILED(pSite->m_lpObject->QueryInterface(
                IID_IOleControl, reinterpret_cast<void**>(&pControl))) || !pControl) {
            return;
        }
        pControl->OnAmbientPropertyChange(dispid);
        pControl->Release();
    });
}

// Symbol: ?CheckDlgButton@COleControlContainer@@UEAAXHI@Z
// COleControlContainer::CheckDlgButton(int, UINT) — retail 0x2346d0:
//     CWnd* pWnd = GetDlgItem(nIDButton);            // vftable slot 0xf8
//     if (pWnd == NULL) return;
//     if (pWnd->m_pCtrlSite(+0xd0) == NULL)
//         ::SendMessage(pWnd->m_hWnd(+0x40), BM_SETCHECK /*0xf1*/, nCheck, 0);
//     else
//         pWnd->m_pCtrlSite->SafeSetProperty(DISPID_VALUE /*0*/, VT_I4 /*3*/, nCheck);
//                                                    // site vftable slot 0xf8
// Deviation: OpenMFC's CWnd has no m_pCtrlSite, so the "is this dialog item an
// OLE control site?" question is answered by FindItem(nIDButton) instead --
// which is the same site retail would find, because retail's own
// GetDlgItem(nID, &hWnd) falls back to FindItem(nID) for exactly these items.
// Retail's `if (pWnd == NULL) return;` gate is kept, in the equivalent form
// "GetDlgItem(nID, &hWnd) produced no window": retail reaches the CWnd through
// CWnd::FromHandle(hWnd), which is NULL exactly when hWnd is, so a site with no
// window (m_hWnd == 0) makes retail return without touching the control, and
// this does the same rather than setting the property anyway.
extern "C" void MS_ABI impl__CheckDlgButton_COleControlContainer__UEAAXHI_Z(
    COleControlContainer* pThis, int nIDButton, unsigned int nCheck) {
    if (!pThis) {
        return;
    }
    HWND hWnd = nullptr;
    impl__GetDlgItem_COleControlContainer__UEBAXHPEAPEAUHWND_____Z(pThis, nIDButton, &hWnd);
    if (!hWnd) {
        return;
    }
    COleControlSite* pSite = pThis->FindItem(static_cast<UINT>(nIDButton));
    if (pSite) {
        impl__SafeSetProperty_COleControlSite__UEAAHJGZZ(
            pSite, DISPID_VALUE, VT_I4, static_cast<long>(nCheck));
        return;
    }
    ::SendMessageW(hWnd, BM_SETCHECK, static_cast<WPARAM>(nCheck), 0);
}

// Symbol: ?CheckRadioButton@COleControlContainer@@UEAAXHHH@Z
// COleControlContainer::CheckRadioButton(int, int, int) — retail 0x234740:
//     if (nIDFirstButton > nIDLastButton) return;            // the leading cmp/jg
//     for (int i = nIDFirstButton; i <= nIDLastButton; ++i)
//         CheckDlgButton(i, i == nIDCheckButton);            // vftable slot 0xe0
// (The `sete` feeding r8d is the i == nIDCheckButton test.)
extern "C" void MS_ABI impl__CheckRadioButton_COleControlContainer__UEAAXHHH_Z(
    COleControlContainer* pThis, int nIDFirstButton, int nIDLastButton, int nIDCheckButton) {
    if (!pThis || nIDFirstButton > nIDLastButton) {
        return;
    }
    for (int nID = nIDFirstButton; nID <= nIDLastButton; ++nID) {
        impl__CheckDlgButton_COleControlContainer__UEAAXHI_Z(
            pThis, nID, static_cast<unsigned int>(nID == nIDCheckButton));
    }
}

// Symbol: ?CreateControl@COleControlContainer@@QEAAHPEAVCWnd@@AEBU_GUID@@PEB_WKAEBUtagRECT@@IPEAVCFile@@HPEA_WPEAPEAVCOleControlSite@@@Z
// COleControlContainer::CreateControl(..., const RECT&, ...) — retail 0x233c60
// (mfc140.dll's ...PEBD... overload; identical apart from the string type).
// It copies the rectangle to a local, splits it into a POINT (left, top) and a
// SIZE (right-left, bottom-top), and tail-calls the POINT/SIZE overload
// (0x233fe0) with every other argument unchanged.
extern "C" int MS_ABI impl__CreateControl_COleControlContainer__QEAAHPEAVCWnd__AEBU_GUID__PEB_WKAEBUtagRECT__IPEAVCFile__HPEA_WPEAPEAVCOleControlSite___Z(
    COleControlContainer* pThis, CWnd* pWndCtrl, const GUID* pClsid, const wchar_t* lpszWindowName,
    unsigned long dwStyle, const RECT* prect, unsigned int nID, CFile* pPersist,
    int bStorage, wchar_t* bstrLicKey, COleControlSite** ppSite) {
    if (!prect) {
        return FALSE;
    }
    POINT pt = { prect->left, prect->top };
    SIZE  sz = { prect->right - prect->left, prect->bottom - prect->top };
    return impl__CreateControl_COleControlContainer__QEAAHPEAVCWnd__AEBU_GUID__PEB_WKPEBUtagPOINT__PEBUtagSIZE__IPEAVCFile__HPEA_WPEAPEAVCOleControlSite___Z(
        pThis, pWndCtrl, pClsid, lpszWindowName, dwStyle, &pt, &sz, nID,
        pPersist, bStorage, bstrLicKey, ppSite);
}

// Symbol: ?CreateControl@COleControlContainer@@QEAAHPEAVCWnd@@AEBU_GUID@@PEB_WKPEBUtagPOINT@@PEBUtagSIZE@@IPEAVCFile@@HPEA_WPEAPEAVCOleControlSite@@@Z
// COleControlContainer::CreateControl(..., const POINT*, const SIZE*, ...) —
// retail 0x233fe0.  Builds a CControlCreationInfo on the stack --
// {m_hk = 2 (NullHandle), m_nHandle = 0, m_clsid = *pClsid} -- and tail-calls
// CreateControlCommon (0x233da0) with it plus all of its own arguments.
extern "C" int MS_ABI impl__CreateControl_COleControlContainer__QEAAHPEAVCWnd__AEBU_GUID__PEB_WKPEBUtagPOINT__PEBUtagSIZE__IPEAVCFile__HPEA_WPEAPEAVCOleControlSite___Z(
    COleControlContainer* pThis, CWnd* pWndCtrl, const GUID* pClsid, const wchar_t* lpszWindowName,
    unsigned long dwStyle, const POINT* ppt, const SIZE* psize, unsigned int nID,
    CFile* pPersist, int bStorage, wchar_t* bstrLicKey, COleControlSite** ppSite) {
    if (!pClsid) {
        return FALSE;
    }
    CreationInfoLayout info;
    info.m_hk = kCreationInfoNullHandle;
    info._pad0 = 0;
    info.m_nHandle = 0;
    info.m_clsid = *pClsid;
    return impl__CreateControlCommon_COleControlContainer__QEAAHPEAVCWnd__AEBU_GUID__AEBVCControlCreationInfo__PEB_WKPEBUtagPOINT__PEBUtagSIZE__IPEAVCFile__HPEA_WPEAPEAVCOleControlSite___Z(
        pThis, pWndCtrl, pClsid, &info, lpszWindowName, dwStyle, ppt, psize, nID,
        pPersist, bStorage, bstrLicKey, ppSite);
}

// Symbol: ?CreateControl@COleControlContainer@@QEAAHPEAVCWnd@@AEBVCControlCreationInfo@@KPEBUtagPOINT@@PEBUtagSIZE@@I@Z
// COleControlContainer::CreateControl(CWnd*, const CControlCreationInfo&, ...)
// — retail 0x233d40 tail-calls CreateControlCommon (0x233da0) passing
// &info.m_clsid (+0x10) as the CLSID and the info itself, with a NULL window
// name and zeroes for pPersist / bStorage / bstrLicKey / ppSite.
extern "C" int MS_ABI impl__CreateControl_COleControlContainer__QEAAHPEAVCWnd__AEBVCControlCreationInfo__KPEBUtagPOINT__PEBUtagSIZE__I_Z(
    COleControlContainer* pThis, CWnd* pWndCtrl, const void* pInfo, unsigned long dwStyle,
    const POINT* ppt, const SIZE* psize, unsigned int nID) {
    if (!pInfo) {
        return FALSE;
    }
    const CreationInfoLayout* info = static_cast<const CreationInfoLayout*>(pInfo);
    return impl__CreateControlCommon_COleControlContainer__QEAAHPEAVCWnd__AEBU_GUID__AEBVCControlCreationInfo__PEB_WKPEBUtagPOINT__PEBUtagSIZE__IPEAVCFile__HPEA_WPEAPEAVCOleControlSite___Z(
        pThis, pWndCtrl, &info->m_clsid, pInfo, nullptr, dwStyle, ppt, psize, nID,
        nullptr, 0, nullptr, nullptr);
}

// Symbol: ?CreateControlCommon@COleControlContainer@@QEAAHPEAVCWnd@@AEBU_GUID@@AEBVCControlCreationInfo@@PEB_WKPEBUtagPOINT@@PEBUtagSIZE@@IPEAVCFile@@HPEA_WPEAPEAVCOleControlSite@@@Z
// COleControlContainer::CreateControlCommon() — retail 0x233da0:
//     COleControlSite* pSite = NULL;
//     BOOL b = m_pWnd(+0x40)->CreateControlSite(this, &pSite, nID, clsid);  // CWnd slot 0x2c0
//     if (b && pSite == NULL) {
//         COccManager* pOcc = AfxGetModuleState()->m_pOccManager(+0xa0);    // 0x1345b0
//         if (pOcc == NULL) <throw>;                                        // 0x225b80, noreturn
//         pSite = pOcc->CreateSite(this, info);                             // slot 3
//     }
//     if (pSite == NULL) return FALSE;
//     long r = (info.m_hk(+0x00) != 2 /*NullHandle*/)
//         ? COleControlSite::CreateControl(pWndCtrl, info, dwStyle, ppt, psize, nID) // 0x238200
//         : pSite->CreateControl(pWndCtrl, clsid, lpszWindowName, dwStyle, ppt,
//                                psize, nID, pPersist, bStorage, bstrLicKey); // site slot 0xb0
//     if (r < 0) { delete pSite; return FALSE; }
//     m_listSitesOrWnds.AddTail(new COleControlSiteOrWnd{NULL, pSite, 0});   // 0x230490
//     if (pSite->m_hWnd(+0x48)) m_siteMap(+0x48)[pSite->m_hWnd] = pSite;     // 0x231460
//     if (ppSite) *ppSite = pSite;
//     return TRUE;
//
// Deviations, all forced by what OpenMFC has:
//  * The site factory.  OpenMFC's COccManager::CreateSite is itself a stub
//    that returns NULL (core/ole/COccManager.cpp), so when the CWnd override
//    yields no site this uses COleControlContainer::CreateSite(), which is the
//    same `new COleControlSite(this)` plus the list insertion retail does
//    separately.  That is also why there is no explicit AddTail below.
//  * Retail's CreateControlSite call is a virtual dispatch, so an application's
//    CWnd override participates; from inside the DLL only the base thunk is
//    reachable, and that base returns TRUE with *ppSite = NULL.
//  * Success test.  Retail tests `r >= 0` because its site CreateControl
//    returns an HRESULT; OpenMFC's site CreateControl returns a BOOL through
//    its thunk, so the test here is `!= 0`.  Using retail's test would treat
//    a failed creation (0) as success.
//  * The m_hk != NullHandle branch forwards, in retail, to
//    COleControlSite::CreateControl(CWnd*, const CControlCreationInfo&, ...);
//    OpenMFC's implementation of that overload is a stub, so this reports
//    failure for it instead of forwarding into a no-op.
//  * m_siteMap does not exist; FindItem scans m_listSites instead.
//  * Retail does NOT bail when CreateControlSite returns FALSE -- it falls
//    straight through to the `pSite == NULL` test (the `je 0x233e44` at
//    0x233e03 only skips the COccManager fallback).  The early return below is
//    behaviourally identical here because OpenMFC's base CreateControlSite
//    (core/window/CWnd.cpp:175) returns FALSE only when ppSite is NULL, which
//    this call site never passes.
//  * The site overload retail dispatches to through slot 0xb0 takes the
//    POINT*/SIZE* pair unchanged (0x233ebb passes ppt at 0x28(%rsp) and psize
//    at 0x30(%rsp)); OpenMFC's site exports that overload only as a stub
//    returning 0, so the RECT overload is called instead with the rectangle
//    rebuilt from ppt/psize.  A NULL ppt or psize therefore becomes a zero
//    edge here rather than being forwarded as NULL.
extern "C" int MS_ABI impl__CreateControlCommon_COleControlContainer__QEAAHPEAVCWnd__AEBU_GUID__AEBVCControlCreationInfo__PEB_WKPEBUtagPOINT__PEBUtagSIZE__IPEAVCFile__HPEA_WPEAPEAVCOleControlSite___Z(
    COleControlContainer* pThis, CWnd* pWndCtrl, const GUID* pClsid, const void* pInfo,
    const wchar_t* lpszWindowName, unsigned long dwStyle, const POINT* ppt, const SIZE* psize,
    unsigned int nID, CFile* pPersist, int bStorage, wchar_t* bstrLicKey,
    COleControlSite** ppSite) {
    if (!pThis || !pClsid || !pInfo) {
        return FALSE;
    }
    const CreationInfoLayout* info = static_cast<const CreationInfoLayout*>(pInfo);
    if (info->m_hk != kCreationInfoNullHandle) {
        return FALSE;   // see the fourth deviation above
    }

    COleControlSite* pSite = nullptr;
    if (!impl__CreateControlSite_CWnd__MEAAHPEAVCOleControlContainer__PEAPEAVCOleControlSite__IAEBU_GUID___Z(
            pThis->m_pWnd, pThis, &pSite, nID, pClsid)) {
        return FALSE;
    }
    if (!pSite) {
        pSite = pThis->CreateSite(pThis);
    }
    if (!pSite) {
        return FALSE;
    }

    RECT rc = { 0, 0, 0, 0 };
    if (ppt) {
        rc.left = ppt->x;
        rc.top  = ppt->y;
    }
    if (psize) {
        rc.right  = rc.left + psize->cx;
        rc.bottom = rc.top  + psize->cy;
    }

    long r = impl__CreateControl_COleControlSite__UEAAJPEAVCWnd__AEBU_GUID__PEB_WKAEBUtagRECT__IPEAVCFile__HPEA_W_Z(
        pSite, pWndCtrl, pClsid, lpszWindowName, dwStyle, &rc, nID, pPersist, bStorage, bstrLicKey);
    if (r == 0) {
        pThis->DeleteSite(pSite);
        return FALSE;
    }
    if (ppSite) {
        *ppSite = pSite;
    }
    return TRUE;
}

namespace {

// The body of CreateOleFont below, reached both from the export and from
// GetAmbientProp's DISPID_AMBIENT_FONT arm (which is how retail reaches it
// too: 0x234245 calls 0x2343f0).
void CreateOleFontFromHandle(COleControlContainer* pThis, HFONT hFont) {
    if (!pThis) {
        return;
    }
    ContainerExtra& extra = ExtraOf(pThis);

    // Retail: if the CFont argument is NULL or carries no handle (+0x08), it
    // attaches DEFAULT_GUI_FONT (0x11); if that is NULL too, SYSTEM_FONT
    // (0x0d); if that is NULL as well it stores NULL in m_pOleFont and gives up.
    if (!hFont) {
        hFont = static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
    }
    if (!hFont) {
        hFont = static_cast<HFONT>(::GetStockObject(SYSTEM_FONT));
    }
    if (!hFont) {
        if (extra.pOleFont) {
            extra.pOleFont->Release();
            extra.pOleFont = nullptr;
        }
        return;
    }

    LOGFONTW lf;
    memset(&lf, 0, sizeof(lf));
    ::GetObjectW(hFont, static_cast<int>(sizeof(lf)), &lf);

    // Retail (0x234481 onwards) fills a FONTDESC from the LOGFONT: face name,
    // lfWeight, lfCharSet, lfItalic, lfUnderline, lfStrikeOut, and a size of
    // abs(lfHeight) * 720000 / GetDeviceCaps(dc, LOGPIXELSY) in CY units,
    // where dc is a CWindowDC on m_pWnd.  (720000 = 72 points-per-inch scaled
    // by CY's 10000.)
    HDC hdc = ::GetWindowDC(ContainerHwnd(pThis));
    int nLogPixelsY = hdc ? ::GetDeviceCaps(hdc, LOGPIXELSY) : 0;
    if (hdc) {
        ::ReleaseDC(ContainerHwnd(pThis), hdc);
    }
    if (nLogPixelsY == 0) {
        nLogPixelsY = 96;
    }
    long nHeight = lf.lfHeight < 0 ? -lf.lfHeight : lf.lfHeight;

    FONTDESC fd;
    memset(&fd, 0, sizeof(fd));
    fd.cbSizeofstruct = sizeof(FONTDESC);
    fd.lpstrName      = lf.lfFaceName;
    // Retail does this arithmetic in 32 bits (imul/cltd/idiv at 0x2344eb) and
    // stores the quotient in cySize.Lo with cySize.Hi = 0.
    fd.cySize.int64   = static_cast<LONGLONG>(
                            static_cast<long>(nHeight * 720000L / nLogPixelsY));
    fd.sWeight        = static_cast<SHORT>(lf.lfWeight);
    fd.sCharset       = static_cast<SHORT>(lf.lfCharSet);
    fd.fItalic        = lf.lfItalic;
    fd.fUnderline     = lf.lfUnderline;
    fd.fStrikethrough = lf.lfStrikeOut;

    // Retail releases the previous m_pOleFont (+0xc0) through IUnknown slot 2
    // and NULLs it before the create, and NULLs it again if the create fails.
    // (Deviation: on the "no stock font at all" path above, retail 0x23445e
    // simply stores NULL over m_pOleFont without releasing what was there;
    // this releases it first.  Deviation: retail divides by
    // GetDeviceCaps(LOGPIXELSY) unguarded, so a DC it could not open would
    // fault; the 96 dpi fallback above is OpenMFC's.)
    if (extra.pOleFont) {
        extra.pOleFont->Release();
        extra.pOleFont = nullptr;
    }
    IFont* pFont = nullptr;
    if (FAILED(::OleCreateFontIndirect(&fd, IID_IFont, reinterpret_cast<void**>(&pFont)))) {
        pFont = nullptr;
    }
    extra.pOleFont = pFont;
}

}  // namespace

// Symbol: ?CreateOleFont@COleControlContainer@@QEAAXPEAVCFont@@@Z
// COleControlContainer::CreateOleFont(CFont*) — retail 0x2343f0; the body is
// transcribed in CreateOleFontFromHandle above.  Retail reads the font handle
// out of the CFont at +0x08 (CGdiObject::m_hObject), which is where OpenMFC's
// CGdiObject keeps it too.
extern "C" void MS_ABI impl__CreateOleFont_COleControlContainer__QEAAXPEAVCFont___Z(
    COleControlContainer* pThis, CFont* pFont) {
    CreateOleFontFromHandle(pThis, pFont ? static_cast<HFONT>(pFont->m_hObject) : nullptr);
}

// Symbol: ?FillListSitesOrWnds@COleControlContainer@@UEAAHPEAU_AFX_OCC_DIALOG_INFO@@@Z
// COleControlContainer::FillListSitesOrWnds(_AFX_OCC_DIALOG_INFO*) — retail
// 0x234ea0.  It walks the dialog-info's array of dword pairs (array pointer
// at pInfo+0x18, count at pInfo+0x10; the first dword of each pair is a
// control id) in lock-step with m_listSitesOrWnds.  For an entry that does not
// match the site at the current list position it walks the container window's
// children -- ::GetWindow(GW_CHILD) for the first, ::GetWindow(GW_HWNDNEXT)
// afterwards -- resolves the id with the helper AfxGetDlgItemStartFromHWND
// (0x234e50) and falls back to ::GetDlgItem, then allocates a 24-byte
// COleControlSiteOrWnd {that HWND, NULL site, the pair's second dword},
// checks ::IsWindow on it and splices it into the list at that position.
// It returns FALSE only when pOccDialogInfo is NULL, TRUE otherwise.
// Left a stub: _AFX_OCC_DIALOG_INFO is only a forward declaration in
// include/openmfc/afxwin.h (no fields), and OpenMFC's m_listSites holds bare
// COleControlSite* rather than site-or-window records, so neither the input
// nor the output of this function is representable here.
extern "C" int MS_ABI impl__FillListSitesOrWnds_COleControlContainer__UEAAHPEAU_AFX_OCC_DIALOG_INFO___Z(
    COleControlContainer* pThis, void* pOccDialogInfo) {
    (void)pThis; (void)pOccDialogInfo;
    return 0;   // retail returns FALSE when pOccDialogInfo is NULL (0x234ec1)
}

// Symbol: ?FreezeAllEvents@COleControlContainer@@QEAAXH@Z
// COleControlContainer::FreezeAllEvents(BOOL) — retail 0x2345a0: walks
// m_listSitesOrWnds from its head (+0x88) and calls
// COleControlSite::FreezeEvents(bFreeze) (0x2399c0) on every element whose
// site (+0x08) is non-null.
extern "C" void MS_ABI impl__FreezeAllEvents_COleControlContainer__QEAAXH_Z(
    COleControlContainer* pThis, int bFreeze) {
    ForEachSite(pThis, [bFreeze](COleControlSite* pSite) {
        impl__FreezeEvents_COleControlSite__QEAAXH_Z(pSite, bFreeze);
    });
}

// Symbol: ?GetAmbientProp@COleControlContainer@@UEAAHPEAVCOleControlSite@@JPEAUtagVARIANT@@@Z
// COleControlContainer::GetAmbientProp(COleControlSite*, DISPID, VARIANT*) —
// retail 0x234120.  The body is a jump table over (dispid + 0x2cc), i.e. the
// 16 DISPIDs -716..-701; the table itself is at RVA 0x2343a4 and was read out
// of the image.  Anything outside that range, and DISPID_AMBIENT_TEXTALIGN,
// falls to 0x23438b: `xor %eax,%eax; ret` -- return FALSE, VARIANT untouched.
// Per-arm bodies:
//   -716 APPEARANCE        (0x23417f) vt = VT_I2,   iVal = 1
//   -715 AUTOCLIP          (0x23415f) vt = VT_BOOL, VARIANT_TRUE
//   -714 SUPPORTSMNEMONICS (0x23415f) vt = VT_BOOL, VARIANT_TRUE
//   -713 DISPLAYASDEFAULT  (0x234280) vt = VT_BOOL, TRUE iff the *site*'s
//                                     cached misc status (+0xb0) has
//                                     OLEMISC_ACTSLIKEBUTTON (0x1000) and its
//                                     m_dwStyle (+0xac) has bit 0
//                                     (BS_DEFPUSHBUTTON)
//   -712 SHOWHATCHING      (0x234176) vt = VT_BOOL, VARIANT_FALSE
//   -711 SHOWGRABHANDLES   (0x234176) vt = VT_BOOL, VARIANT_FALSE
//   -710 UIDEAD            (0x234176) vt = VT_BOOL, VARIANT_FALSE
//   -709 USERMODE          (0x23415f) vt = VT_BOOL, VARIANT_TRUE
//   -708 TEXTALIGN         (0x23438b) return FALSE
//   -707 SCALEUNITS        (0x2342ff) see below
//   -706 MESSAGEREFLECT    (0x23415f) vt = VT_BOOL, VARIANT_TRUE
//   -705 LOCALEID          (0x2342b4) vt = VT_I4,   lVal = ::GetThreadLocale()
//   -704 FORECOLOR         (0x234196) vt = VT_I4,   lVal = cached +0xbc
//   -703 FONT              (0x234215) see below
//   -702 DISPLAYNAME       (0x2342c5) see below
//   -701 BACKCOLOR         (0x234196) vt = VT_I4,   lVal = cached +0xb8
// The colour arm (0x234196) recomputes the pair whenever the cached back
// colour (+0xb8) is -1: it opens a CWindowDC on m_pWnd, sends
// WM_CTLCOLORSTATIC (0x138) to m_pWnd->m_hWnd with the DC as wParam and the
// same HWND as lParam, then caches ::GetBkColor into +0xb8 and
// ::GetTextColor into +0xbc.  (Verified by resolving the two IAT slots:
// 0x2c4178 = GDI32!GetBkColor, 0x2c4228 = GDI32!GetTextColor.)
// The font arm (0x234215) creates m_pOleFont (+0xc0) on first use from
// ::SendMessage(m_pWnd->m_hWnd, WM_GETFONT, 0, 0) via CreateOleFont, returns
// FALSE if that still leaves it NULL, and otherwise AddRefs it and stores the
// IFont* straight into the VT_DISPATCH slot without a QueryInterface.
//
// The SCALEUNITS (0x2342ff) and DISPLAYNAME (0x2342c5) arms both start by
// loading the qword at the process-global 0x3ab440 and calling its vtable slot
// 3 with 0x3ab440 itself as `this`.  That global is MFC's string manager: its
// vptr is 0x32f700, whose slot 3 is ?GetNilString@CAfxStringMgr@@, so the call
// is nothing more exotic than a default-constructed, EMPTY CString --
// `lea 0x18(%rax)` is the CStringData -> character-data step, and the
// `lock xadd %eax,0x10(%rdx)` at the end of both arms is that CString's
// destructor releasing the nil CStringData again.  So:
//   DISPLAYNAME (0x2342c5) is `CString str; pvar->vt = VT_BSTR;
//                              pvar->bstrVal = str.AllocSysString();
//                              return TRUE;` -- an empty BSTR, not a failure.
//   SCALEUNITS  (0x2342ff) is the same CString loaded from string resource
//                              0xF1C0 (= AFX_IDS_OCC_SCALEUNITS_PIXELS,
//                              afxres.h:525) through
//                              AfxFindStringResourceHandle (0x2accf0) and
//                              CStringA::LoadString (0xdc00), then returned as
//                              a BSTR; a missing handle or a zero-length load
//                              throws (0x225b80 = AfxThrowInvalidArgException).
//                              That resource is in mfc140.dll's own string
//                              table (RT_STRING block 3869, index 0) and its
//                              value there is the literal "pixels", extracted
//                              from the image.  OpenMFC ships no MFC string
//                              table, so the literal is used directly; an
//                              application that overrides 0xF1C0 -- which
//                              retail would honour through the module state's
//                              resource chain -- is not followed here.
extern "C" int MS_ABI impl__GetAmbientProp_COleControlContainer__UEAAHPEAVCOleControlSite__JPEAUtagVARIANT___Z(
    COleControlContainer* pThis, COleControlSite* pSite, long dispid, VARIANT* pVar) {
    if (!pThis || !pVar) {
        return FALSE;
    }
    switch (dispid) {
    case DISPID_AMBIENT_APPEARANCE:
        pVar->vt = VT_I2;
        pVar->iVal = 1;
        return TRUE;

    case DISPID_AMBIENT_AUTOCLIP:
    case DISPID_AMBIENT_SUPPORTSMNEMONICS:
    case DISPID_AMBIENT_USERMODE:
    case DISPID_AMBIENT_MESSAGEREFLECT:
        pVar->vt = VT_BOOL;
        pVar->boolVal = VARIANT_TRUE;
        return TRUE;

    case DISPID_AMBIENT_SHOWHATCHING:
    case DISPID_AMBIENT_SHOWGRABHANDLES:
    case DISPID_AMBIENT_UIDEAD:
        pVar->vt = VT_BOOL;
        pVar->boolVal = VARIANT_FALSE;
        return TRUE;

    case DISPID_AMBIENT_DISPLAYASDEFAULT: {
        // Retail dereferences pSite unconditionally here; the null check is
        // OpenMFC's own hardening.
        BOOL bDefault = FALSE;
        if (pSite) {
            bDefault = (SiteMiscStatusOf(pSite) & OLEMISC_ACTSLIKEBUTTON) &&
                       (pSite->m_dwStyle & 0x1);
        }
        pVar->vt = VT_BOOL;
        pVar->boolVal = bDefault ? VARIANT_TRUE : VARIANT_FALSE;
        return TRUE;
    }

    case DISPID_AMBIENT_LOCALEID:
        pVar->vt = VT_I4;
        pVar->lVal = static_cast<long>(::GetThreadLocale());
        return TRUE;

    case DISPID_AMBIENT_BACKCOLOR:
    case DISPID_AMBIENT_FORECOLOR: {
        ContainerExtra& extra = ExtraOf(pThis);
        if (extra.clrBack == static_cast<COLORREF>(-1)) {
            HWND hWnd = ContainerHwnd(pThis);
            HDC hdc = ::GetWindowDC(hWnd);      // retail: CWindowDC dc(m_pWnd)
            if (hdc) {
                ::SendMessageW(hWnd, WM_CTLCOLORSTATIC,
                               reinterpret_cast<WPARAM>(hdc),
                               reinterpret_cast<LPARAM>(hWnd));
                extra.clrBack = ::GetBkColor(hdc);
                extra.clrFore = ::GetTextColor(hdc);
                ::ReleaseDC(hWnd, hdc);
            }
        }
        pVar->vt = VT_I4;
        pVar->lVal = static_cast<long>(dispid == DISPID_AMBIENT_BACKCOLOR
                                           ? extra.clrBack : extra.clrFore);
        return TRUE;
    }

    case DISPID_AMBIENT_FONT: {
        ContainerExtra& extra = ExtraOf(pThis);
        if (!extra.pOleFont) {
            HWND hWnd = ContainerHwnd(pThis);
            HFONT hFont = reinterpret_cast<HFONT>(::SendMessageW(hWnd, WM_GETFONT, 0, 0));
            CreateOleFontFromHandle(pThis, hFont);
            if (!extra.pOleFont) {
                return FALSE;
            }
        }
        pVar->vt = VT_DISPATCH;
        extra.pOleFont->AddRef();
        // Retail stores the IFont* into the VT_DISPATCH slot verbatim (no QI).
        pVar->pdispVal = reinterpret_cast<IDispatch*>(extra.pOleFont);
        return TRUE;
    }

    case DISPID_AMBIENT_SCALEUNITS:
        // Retail: CString::LoadString(AFX_IDS_OCC_SCALEUNITS_PIXELS) then
        // AllocSysString.  mfc140.dll's own copy of that resource is "pixels".
        pVar->vt = VT_BSTR;
        pVar->bstrVal = ::SysAllocString(L"pixels");
        return TRUE;

    case DISPID_AMBIENT_DISPLAYNAME:
        // Retail: an empty CString's AllocSysString (see the note above) --
        // the container reports a display name that is present but blank.
        pVar->vt = VT_BSTR;
        pVar->bstrVal = ::SysAllocString(L"");
        return TRUE;

    case DISPID_AMBIENT_TEXTALIGN:
    default:
        return FALSE;   // retail's default arm, 0x23438b
    }
}

// Symbol: ?GetDispatchMap@COleControlContainer@@MEBAPEBUAFX_DISPMAP@@XZ
// COleControlContainer::GetDispatchMap() — this override has no RVA of its
// own because MSVC folded it with the static GetThisDispatchMap: container
// vftable slot 14 (RVA 0x328be8 + 0x70) holds 0x233890, which is
// ?GetThisDispatchMap@COleControlContainer@@KAPEBUAFX_DISPMAP@@XZ, whose whole
// body is `lea 0x328d38(%rip),%rax; ret`.  Both therefore return the address
// of COleControlContainer's AFX_DISPMAP at RVA 0x328d38.
// That map is reproduced above (see g_dispatchMap_COleControlContainer): its
// four fields and its single terminator entry were read straight out of retail
// .rdata, and the base-map slot is the exported
// CCmdTarget::GetThisDispatchMap, which OpenMFC defines at
// core/runtime/CCmdTarget.cpp:998 and which returns CCmdTarget's own (equally
// entry-less) map.
extern "C" void* MS_ABI impl__GetDispatchMap_COleControlContainer__MEBAPEBUAFX_DISPMAP__XZ(
    const COleControlContainer* pThis) {
    (void)pThis;
    return const_cast<DispMapLayout*>(&g_dispatchMap_COleControlContainer);
}

// Symbol: ?GetDlgItem@COleControlContainer@@UEBAPEAVCWnd@@H@Z
// COleControlContainer::GetDlgItem(int) — retail 0x234790:
//     HWND hWnd; GetDlgItem(nID, &hWnd);      // vftable slot 0xf0
//     return CWnd::FromHandle(hWnd);          // 0x289180
extern "C" CWnd* MS_ABI impl__GetDlgItem_COleControlContainer__UEBAPEAVCWnd__H_Z(
    const COleControlContainer* pThis, int nID) {
    HWND hWnd = nullptr;
    impl__GetDlgItem_COleControlContainer__UEBAXHPEAPEAUHWND_____Z(pThis, nID, &hWnd);
    return impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(hWnd);
}

// Symbol: ?GetDlgItem@COleControlContainer@@UEBAXHPEAPEAUHWND__@@@Z
// COleControlContainer::GetDlgItem(int, HWND*) — retail 0x2347c0:
//     HWND h = ::GetDlgItem(m_pWnd ? m_pWnd->m_hWnd : NULL, nID);
//     if (h == NULL) {
//         COleControlSite* pSite = FindItem(nID);   // vftable slot 0xb8
//         if (pSite != NULL) h = pSite->m_hWnd;     // site +0x48
//     }
//     *phWnd = h;
extern "C" void MS_ABI impl__GetDlgItem_COleControlContainer__UEBAXHPEAPEAUHWND_____Z(
    const COleControlContainer* pThis, int nID, HWND* phWnd) {
    if (!phWnd) {
        return;
    }
    HWND hWnd = ::GetDlgItem(ContainerHwnd(pThis), nID);
    if (!hWnd && pThis) {
        COleControlSite* pSite = pThis->FindItem(static_cast<UINT>(nID));
        if (pSite) {
            hWnd = pSite->m_hWnd;
        }
    }
    *phWnd = hWnd;
}

// Symbol: ?GetDlgItemInt@COleControlContainer@@UEBAIHPEAHH@Z
// COleControlContainer::GetDlgItemInt(int, BOOL*, BOOL) — retail 0x234820:
//     TCHAR sz[256];
//     if (!GetDlgItemText(nID, sz, 256)) {          // vftable slot 0x108
//         if (lpTrans) *lpTrans = 0;
//         return 0;
//     }
//     p = sz;
//     while (isspace(*p)) p = CharNext(p);
//     if (*p == '+' || *p == '-') p = CharNext(p);  // the (c-'+') & 0xfd test
//     int bOK = isdigit(*p);
//     if (lpTrans) *lpTrans = bOK;
//     if (!bOK) return 0;
//     return bSigned ? strtol(sz, NULL, 10) : strtoul(sz, NULL, 10);
// Note it hands the *original* buffer to strtol, not the scanned pointer.
extern "C" unsigned int MS_ABI impl__GetDlgItemInt_COleControlContainer__UEBAIHPEAHH_Z(
    const COleControlContainer* pThis, int nID, int* lpTrans, int bSigned) {
    wchar_t sz[256];
    sz[0] = L'\0';
    if (!impl__GetDlgItemTextW_COleControlContainer__UEBAHHPEA_WH_Z(
            pThis, nID, sz, 256)) {
        if (lpTrans) {
            *lpTrans = 0;
        }
        return 0;
    }
    const wchar_t* p = sz;
    while (*p && iswspace(*p)) {
        ++p;
    }
    if (*p == L'+' || *p == L'-') {
        ++p;
    }
    int bOK = iswdigit(*p) ? 1 : 0;
    if (lpTrans) {
        *lpTrans = bOK;
    }
    if (!bOK) {
        return 0;
    }
    return bSigned ? static_cast<unsigned int>(wcstol(sz, nullptr, 10))
                   : static_cast<unsigned int>(wcstoul(sz, nullptr, 10));
}

// Symbol: ?GetDlgItemTextW@COleControlContainer@@UEBAHHPEA_WH@Z
// COleControlContainer::GetDlgItemText(int, LPTSTR, int) — retail 0x234900
// (mfc140.dll names it ...A; this is the same body):
//     CWnd* pWnd = GetDlgItem(nID);            // vftable slot 0xf8
//     if (pWnd == NULL) return 0;
//     return pWnd->GetWindowText(lpStr, nMaxCount);
extern "C" int MS_ABI impl__GetDlgItemTextW_COleControlContainer__UEBAHHPEA_WH_Z(
    const COleControlContainer* pThis, int nID, wchar_t* lpStr, int nMaxCount) {
    CWnd* pWnd = impl__GetDlgItem_COleControlContainer__UEBAPEAVCWnd__H_Z(pThis, nID);
    if (!pWnd) {
        return 0;
    }
    return impl__GetWindowTextW_CWnd__QEBAHPEA_WH_Z(pWnd, lpStr, nMaxCount);
}

// Symbol: ?GetThisDispatchMap@COleControlContainer@@KAPEBUAFX_DISPMAP@@XZ
// COleControlContainer::GetThisDispatchMap() — retail 0x233890 is exactly
// `lea 0x328d38(%rip),%rax; ret`: the address of the class's AFX_DISPMAP.
// Returns the same reconstructed map as GetDispatchMap above, which is what
// the folding in retail means.
extern "C" void* MS_ABI impl__GetThisDispatchMap_COleControlContainer__KAPEBUAFX_DISPMAP__XZ() {
    return const_cast<DispMapLayout*>(&g_dispatchMap_COleControlContainer);
}

// Symbol: ?HandleSetFocus@COleControlContainer@@UEAAHXZ
// COleControlContainer::HandleSetFocus() — retail 0x234ad0 is exactly
// `xor %eax,%eax; cmp %rax,0xd8(%rcx); setne %al; ret`, i.e.
// `return m_pSiteFocus(+0xd8) != NULL;`.
// Left a stub: +0xd8 is written by COleControlSite (its SetFocus stores the
// site there, see the note at core/ole/COleControlSite.cpp:1341) and cleared
// by OnUIDeactivate below.  It is therefore state shared with another
// translation unit, and OpenMFC's COleControlContainer has no member for it,
// so this file cannot make the answer true.  Returning FALSE keeps the
// "nothing has the focus" answer rather than claiming a focus site exists.
extern "C" int MS_ABI impl__HandleSetFocus_COleControlContainer__UEAAHXZ(
    COleControlContainer* pThis) {
    (void)pThis;
    return 0;
}

// Symbol: ?HandleWindowlessMessage@COleControlContainer@@UEAAHI_K_JPEA_J@Z
// COleControlContainer::HandleWindowlessMessage(UINT, WPARAM, LPARAM, LRESULT*)
// — retail 0x234ae0.  It stores 0 through plResult first (0x234b05) and then
// routes on the message id.  Two disjoint sets are handled; every other
// message falls to 0x234ba4, `xor %eax,%eax`, i.e. "not handled":
//   * 0x200-0x209 (the mouse messages) go to 0x234bf3, which first consults
//     the container's capture site (+0xd0) and otherwise walks
//     m_listSitesOrWnds from its head (+0x88);
//   * 0x1f, 0x53, 0x100-0x105, 0x107, 0x10d-0x10f, 0x281 and 0x282 go to
//     0x234cc0.
// Those are the ids the compare chain at 0x234b0c-0x234bf1 selects; ids above
// 0x282 are dispatched by a further chain at 0x234c9a (0x283, 0x284, ... also
// reach 0x234cc0) which was NOT enumerated here, so the lists above are a lower
// bound on what retail handles, not the complete set.  The bodies at 0x234bf3 /
// 0x234cc0 were not transcribed further either.
// The head comment of core/ole/COleControlSite.cpp already records, from the
// same function, that the site loop visits a site only when its
// windowless-active flag (+0xb4) is set, hit-tests the site's stored position
// rect (+0x5c) and dispatches through the site's IOleInPlaceObjectWindowless
// (+0x98) slot 9 (0x48) = OnWindowMessage.
// Left a stub: OpenMFC's COleControlSite has none of the three members this
// needs -- the windowless-active flag, the stored position rect, and the
// IOleInPlaceObjectWindowless pointer are all recorded as having no OpenMFC
// equivalent in the map at the head of core/ole/COleControlSite.cpp -- and the
// container has no capture/focus site either.  Returning FALSE means "not
// handled", which leaves the caller's default processing in place.
extern "C" int MS_ABI impl__HandleWindowlessMessage_COleControlContainer__UEAAHI_K_JPEA_J_Z(
    COleControlContainer* pThis, unsigned int message, unsigned __int64 wParam,
    __int64 lParam, __int64* plResult) {
    (void)pThis; (void)message; (void)wParam; (void)lParam;
    if (plResult) {
        *plResult = 0;   // retail stores 0 through the out pointer first (0x234b05)
    }
    return 0;
}

// Symbol: ?IsDlgButtonChecked@COleControlContainer@@UEBAIH@Z
// COleControlContainer::IsDlgButtonChecked(int) — retail 0x234a60:
//     CWnd* pWnd = GetDlgItem(nIDButton);              // vftable slot 0xf8
//     if (pWnd == NULL) return 0;
//     if (pWnd->m_pCtrlSite(+0xd0) == NULL)
//         return ::SendMessage(pWnd->m_hWnd, BM_GETCHECK /*0xf0*/, 0, 0);
//     long v; pWnd->GetProperty(DISPID_VALUE /*0*/, VT_I4 /*3*/, &v);   // 0x2a7b70
//     return (v == 0xffff) ? 1 : v;
// The final compare is literally `cmp $0xffff,%eax` -- a VB-style TRUE stored
// as an unsigned short -- and anything else is returned unchanged.
// Deviation, as in CheckDlgButton above: OpenMFC's CWnd has no m_pCtrlSite, so
// the site is found with FindItem(nIDButton), and the "no window -> return 0"
// gate is expressed as "GetDlgItem(nID, &hWnd) produced no window".  The
// property is read through COleControlSite::GetProperty directly rather than
// through CWnd::GetProperty; retail's CWnd::GetProperty (0x2a7b70) is a
// two-hop forwarder to the very same site -- it tail-calls CWnd::InvokeHelper
// (0x2a7b10), which calls m_pCtrlSite(+0xd0)'s vftable slot 0xd0
// (InvokeHelperV) with DISPATCH_PROPERTYGET, which is what the site's own
// GetProperty does too.
extern "C" unsigned int MS_ABI impl__IsDlgButtonChecked_COleControlContainer__UEBAIH_Z(
    const COleControlContainer* pThis, int nIDButton) {
    if (!pThis) {
        return 0;
    }
    HWND hWnd = nullptr;
    impl__GetDlgItem_COleControlContainer__UEBAXHPEAPEAUHWND_____Z(pThis, nIDButton, &hWnd);
    if (!hWnd) {
        return 0;
    }
    COleControlSite* pSite = pThis->FindItem(static_cast<UINT>(nIDButton));
    if (pSite) {
        long lValue = 0;
        impl__GetProperty_COleControlSite__UEBAXJGPEAX_Z(pSite, DISPID_VALUE, VT_I4, &lValue);
        return (lValue == 0xffff) ? 1u : static_cast<unsigned int>(lValue);
    }
    return static_cast<unsigned int>(::SendMessageW(hWnd, BM_GETCHECK, 0, 0));
}

// Symbol: ?OnPaint@COleControlContainer@@UEAAHPEAVCDC@@@Z
// COleControlContainer::OnPaint(CDC*) — retail 0x234d10.  Walks
// m_listSitesOrWnds and, for each element whose site (+0x08) is non-null, is
// windowless-active (+0xb4 != 0) and does NOT have OLEMISC_INVISIBLEATRUNTIME
// (0x400) in its cached misc status (+0xb0):
//     QueryInterface(IID_IViewObject, GUID at 0x2d7b78) on the site's
//     IOleObject (+0x80); on success call Draw (IViewObject slot 3 = 0x18)
//     with dwAspect = DVASPECT_CONTENT, lindex = -1 and every other pointer
//     argument NULL except hdcDraw = pDC->m_hDC (+0x08); if it returns >= 0,
//     remember that something was painted; then Release.
// Returns whether at least one site was drawn.
// Deviation: OpenMFC's site has no windowless-active flag; as the map at the
// head of core/ole/COleControlSite.cpp records, that flag is approximated by
// m_hWnd == NULL, and the implication only runs one way (a site that has not
// been activated yet also has a null m_hWnd).  The cached misc status is
// likewise queried from IOleObject on demand.
extern "C" int MS_ABI impl__OnPaint_COleControlContainer__UEAAHPEAVCDC___Z(
    COleControlContainer* pThis, CDC* pDC) {
    HDC hdc = pDC ? pDC->m_hDC : nullptr;
    int bPainted = 0;
    ForEachSite(pThis, [hdc, &bPainted](COleControlSite* pSite) {
        if (pSite->m_hWnd != nullptr) {
            return;   // approximates retail's "windowless-active" test on +0xb4
        }
        if (!pSite->m_lpObject) {
            return;
        }
        if (SiteMiscStatusOf(pSite) & OLEMISC_INVISIBLEATRUNTIME) {
            return;
        }
        IViewObject* pView = nullptr;
        if (FAILED(pSite->m_lpObject->QueryInterface(
                IID_IViewObject, reinterpret_cast<void**>(&pView))) || !pView) {
            return;
        }
        HRESULT hr = pView->Draw(DVASPECT_CONTENT, -1, nullptr, nullptr, nullptr,
                                 hdc, nullptr, nullptr, nullptr, 0);
        if (SUCCEEDED(hr)) {
            bPainted = 1;
        }
        pView->Release();
    });
    return bPainted;
}

// Symbol: ?OnUIActivate@COleControlContainer@@UEAAXPEAVCOleControlSite@@@Z
// COleControlContainer::OnUIActivate(COleControlSite*) — retail 0x234650:
//     COleControlSite* pOld = m_pSiteUIActive(+0xc8);
//     if (pOld != NULL && pOld != pSite)
//         pOld->m_lpInPlaceObject(+0x88)->UIDeactivate();  // IOleInPlaceObject slot 6
//     m_pSiteUIActive = pSite;
// Retail does not null-check the old site's in-place object before the virtual
// call; the check below is OpenMFC's own hardening.
extern "C" void MS_ABI impl__OnUIActivate_COleControlContainer__UEAAXPEAVCOleControlSite___Z(
    COleControlContainer* pThis, COleControlSite* pSite) {
    if (!pThis) {
        return;
    }
    ContainerExtra& extra = ExtraOf(pThis);
    COleControlSite* pOld = extra.pSiteUIActive;
    if (pOld && pOld != pSite && pOld->m_lpInPlaceObject) {
        pOld->m_lpInPlaceObject->UIDeactivate();
    }
    extra.pSiteUIActive = pSite;
}

// Symbol: ?OnUIDeactivate@COleControlContainer@@UEAAXPEAVCOleControlSite@@@Z
// COleControlContainer::OnUIDeactivate(COleControlSite*) — retail 0x2346a0:
//     if (m_pSiteUIActive(+0xc8) == pSite) m_pSiteUIActive = NULL;
//     if (m_pSiteFocus(+0xd8)    == pSite) m_pSiteFocus    = NULL;
// Only the first half is reproduced: OpenMFC's container has no focus-site
// member (see HandleSetFocus above), so there is nothing to clear for +0xd8.
extern "C" void MS_ABI impl__OnUIDeactivate_COleControlContainer__UEAAXPEAVCOleControlSite___Z(
    COleControlContainer* pThis, COleControlSite* pSite) {
    if (!pThis) {
        return;
    }
    ContainerExtra& extra = ExtraOf(pThis);
    if (extra.pSiteUIActive == pSite) {
        extra.pSiteUIActive = nullptr;
    }
}

// Symbol: ?ScrollChildren@COleControlContainer@@UEAAXHH@Z
// COleControlContainer::ScrollChildren(int, int) — retail 0x2345e0.  For every
// element of m_listSitesOrWnds whose site (+0x08) is non-null:
//     ::OffsetRect(&pSite->m_rect(+0x5c), dx, dy);
//     pSite->m_lpInPlaceObject(+0x88)->SetObjectRects(&pSite->m_rect,
//                                                     &pSite->m_rect);
//                                        // IOleInPlaceObject slot 7 = 0x38
// Left a stub: the whole function is an in-place edit of the site's *stored*
// position rectangle at +0x5c, and OpenMFC's COleControlSite has no such
// member -- detail::olecore::GetSitePositionRect derives the rectangle from
// the live window instead, so offsetting what it returns and pushing it back
// would apply the scroll a second time on top of a window that has already
// moved.  Doing nothing is the honest result until the site owns a rect.
extern "C" void MS_ABI impl__ScrollChildren_COleControlContainer__UEAAXHH_Z(
    COleControlContainer* pThis, int dx, int dy) {
    (void)pThis; (void)dx; (void)dy;
}

// Symbol: ?SendDlgItemMessageW@COleControlContainer@@UEAA_JHI_K_J@Z
// COleControlContainer::SendDlgItemMessage(int, UINT, WPARAM, LPARAM) —
// retail 0x234940 (mfc140.dll names it ...A; same body):
//     CWnd* pWnd = GetDlgItem(nID);          // vftable slot 0xf8
//     if (pWnd == NULL) return 0;
//     return ::SendMessage(pWnd->m_hWnd(+0x40), message, wParam, lParam);
// Retail does not check m_hWnd before the send; the extra guard below only
// short-circuits a ::SendMessage to a NULL window, which returns 0 anyway.
extern "C" __int64 MS_ABI impl__SendDlgItemMessageW_COleControlContainer__UEAA_JHI_K_J_Z(
    COleControlContainer* pThis, int nID, unsigned int message,
    unsigned __int64 wParam, __int64 lParam) {
    CWnd* pWnd = impl__GetDlgItem_COleControlContainer__UEBAPEAVCWnd__H_Z(pThis, nID);
    if (!pWnd || !pWnd->m_hWnd) {
        return 0;
    }
    return static_cast<__int64>(::SendMessageW(pWnd->m_hWnd, message,
                                               static_cast<WPARAM>(wParam),
                                               static_cast<LPARAM>(lParam)));
}

// Symbol: ?SetDlgItemInt@COleControlContainer@@UEAAXHIH@Z
// COleControlContainer::SetDlgItemInt(int, UINT, BOOL) — retail 0x234990:
//     TCHAR sz[34];
//     bSigned ? _ltoa_s((long)nValue, sz, 34, 10) : _ultoa_s(nValue, sz, 34, 10);
//     SetDlgItemText(nID, sz);            // vftable slot 0x120
// (The errno checks after the conversion are the CRT's invalid-parameter
// handling; only 0 continues, everything else throws.)
extern "C" void MS_ABI impl__SetDlgItemInt_COleControlContainer__UEAAXHIH_Z(
    COleControlContainer* pThis, int nID, unsigned int nValue, int bSigned) {
    wchar_t sz[34];
    if (bSigned) {
        _ltow_s(static_cast<long>(nValue), sz, 34, 10);
    } else {
        _ultow_s(static_cast<unsigned long>(nValue), sz, 34, 10);
    }
    impl__SetDlgItemTextW_COleControlContainer__UEAAXHPEB_W_Z(pThis, nID, sz);
}

// Symbol: ?SetDlgItemTextW@COleControlContainer@@UEAAXHPEB_W@Z
// COleControlContainer::SetDlgItemText(int, LPCTSTR) — retail 0x234a30
// (mfc140.dll names it ...A; same body):
//     CWnd* pWnd = GetDlgItem(nID);        // vftable slot 0xf8
//     if (pWnd != NULL) pWnd->SetWindowText(lpszString);
extern "C" void MS_ABI impl__SetDlgItemTextW_COleControlContainer__UEAAXHPEB_W_Z(
    COleControlContainer* pThis, int nID, const wchar_t* lpszString) {
    CWnd* pWnd = impl__GetDlgItem_COleControlContainer__UEBAPEAVCWnd__H_Z(pThis, nID);
    if (pWnd) {
        impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(pWnd, lpszString);
    }
}

//=============================================================================
// COleControlContainer::XOleIPFrame  (retail nested part at container +0xe8,
// vftable RVA 0x328b30)  and  ::XOleContainer  (nested part at +0xf0, vftable
// RVA 0x328bb0).
//
// Ten of these entry points have no RVA of their own in the export map because
// their bodies were folded by the linker with other two-instruction functions.
// They were recovered by reading the two vftables out of the image and
// disassembling the slots:
//   XOleIPFrame slot 4 ContextSensitiveHelp -> 0xf540
//               slot 5 GetBorder            -> 0xf540
//               slot 6 RequestBorderSpace   -> 0xf540
//               slot 7 SetBorderSpace       -> 0xf540
//               slot 9 InsertMenus          -> 0xf540
//               slot 10 SetMenu             -> 0xf540
//               slot 11 RemoveMenus         -> 0xf540
//               slot 12 SetStatusText       -> 0xf540
//               slot 13 EnableModeless      -> 0x7260
//               slot 14 TranslateAccelerator-> 0xf540
//   XOleContainer slot 3 ParseDisplayName   -> 0xf540
//                 slot 5 LockContainer      -> 0xf540
// RVA 0xf540 is `mov $0x80004001,%eax; ret` (E_NOTIMPL) and RVA 0x7260 is
// `xor %eax,%eax; ret` (S_OK).  Neither touches any argument.  (Those two RVAs
// reverse-map to ?UpdateUI@CDHtmlDialog@@ and ?Update@XOleObject@COleControl@@
// respectively, which is just the folding; the vftable slot is the reliable
// direction.)
//=============================================================================

// Symbol: ?ContextSensitiveHelp@XOleIPFrame@COleControlContainer@@UEAAJH@Z
extern "C" long MS_ABI impl__ContextSensitiveHelp_XOleIPFrame_COleControlContainer__UEAAJH_Z(
    void* pThis, int fEnterMode) {
    (void)pThis; (void)fEnterMode;
    return E_NOTIMPL;
}

// Symbol: ?EnableModeless@XOleIPFrame@COleControlContainer@@UEAAJH@Z
// The one slot in this group whose folded body is S_OK rather than E_NOTIMPL.
extern "C" long MS_ABI impl__EnableModeless_XOleIPFrame_COleControlContainer__UEAAJH_Z(
    void* pThis, int fEnable) {
    (void)pThis; (void)fEnable;
    return S_OK;
}

namespace {

// An IEnumUnknown over a fixed (here: always empty) set.  See EnumObjects.
class CEmptyEnumUnknown : public IEnumUnknown {
public:
    CEmptyEnumUnknown() : m_refs(1) {}

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv) override {
        if (!ppv) {
            return E_POINTER;
        }
        if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IEnumUnknown)) {
            *ppv = static_cast<IEnumUnknown*>(this);
            AddRef();
            return S_OK;
        }
        *ppv = nullptr;
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() override {
        return static_cast<ULONG>(InterlockedIncrement(&m_refs));
    }
    ULONG STDMETHODCALLTYPE Release() override {
        LONG n = InterlockedDecrement(&m_refs);
        if (n == 0) {
            delete this;
        }
        return static_cast<ULONG>(n);
    }
    HRESULT STDMETHODCALLTYPE Next(ULONG celt, IUnknown** rgelt, ULONG* pceltFetched) override {
        if (!rgelt && celt != 0) {
            return E_POINTER;
        }
        if (pceltFetched) {
            *pceltFetched = 0;
        }
        return celt == 0 ? S_OK : S_FALSE;
    }
    HRESULT STDMETHODCALLTYPE Skip(ULONG celt) override { return celt == 0 ? S_OK : S_FALSE; }
    HRESULT STDMETHODCALLTYPE Reset() override { return S_OK; }
    HRESULT STDMETHODCALLTYPE Clone(IEnumUnknown** ppenum) override {
        if (!ppenum) {
            return E_POINTER;
        }
        *ppenum = new CEmptyEnumUnknown();
        return S_OK;
    }

private:
    virtual ~CEmptyEnumUnknown() = default;
    volatile LONG m_refs;
};

}  // namespace

// Symbol: ?EnumObjects@XOleContainer@COleControlContainer@@UEAAJKPEAPEAUIEnumUnknown@@@Z
// COleControlContainer::XOleContainer::EnumObjects(DWORD, IEnumUnknown**) —
// retail 0x235230:
//     *ppenum = NULL;
//     IUnknown** rgUnk = NULL; ULONG n = 0;
//     if (grfFlags & OLECONTF_EMBEDDINGS /*1*/) {
//         rgUnk = new IUnknown*[m_listSitesOrWnds.GetCount()];      // -0x58(this)
//         for (node = list head (-0x68(this)); node; node = node->next) {
//             COleControlSite* pSite = node->data->pSite(+0x08);
//             if (pSite == NULL) continue;
//             pSite->m_lpObject(+0x80)->AddRef();
//             rgUnk[n++] = pSite->m_lpObject;
//         }
//     }
//     CEnumUnknown* pEnum = new CEnumUnknown;                        // 0x70 bytes
//     if (pEnum) CEnumArray::CEnumArray(pEnum, 8, rgUnk, n, TRUE);   // 0x25a6e0
//     *ppenum = (IEnumUnknown*)((BYTE*)pEnum + 0x68);
//     return S_OK;
// The `-0x68`/`-0x58` displacements are the METHOD_PROLOGUE for a nested part
// at container +0xf0, so they name m_listSitesOrWnds' head (+0x88) and count
// (+0x98).
// Left effectively a stub: OpenMFC's COleControlContainer has no nested
// XOleContainer subobject, so `pThis` here cannot be turned back into a
// container and the site list is unreachable.  Rather than return S_OK with an
// unwritten out-parameter, this hands back a valid but permanently empty
// IEnumUnknown; the enumeration is empty because the sites cannot be reached,
// not because the container has none.
extern "C" long MS_ABI impl__EnumObjects_XOleContainer_COleControlContainer__UEAAJKPEAPEAUIEnumUnknown___Z(
    void* pThis, unsigned long grfFlags, IEnumUnknown** ppenum) {
    (void)pThis; (void)grfFlags;
    if (!ppenum) {
        return E_POINTER;
    }
    *ppenum = new CEmptyEnumUnknown();
    return S_OK;
}

// Symbol: ?GetBorder@XOleIPFrame@COleControlContainer@@UEAAJPEAUtagRECT@@@Z
extern "C" long MS_ABI impl__GetBorder_XOleIPFrame_COleControlContainer__UEAAJPEAUtagRECT___Z(
    void* pThis, RECT* lprectBorder) {
    (void)pThis; (void)lprectBorder;
    return E_NOTIMPL;
}

// Symbol: ?GetWindow@XOleIPFrame@COleControlContainer@@UEAAJPEAPEAUHWND__@@@Z
// COleControlContainer::XOleIPFrame::GetWindow(HWND*) — retail 0x235080 is
// `mov -0xa8(%rcx),%rax; mov 0x40(%rax),%rcx; xor %eax,%eax; mov %rcx,(%rdx);
// ret`.  With the nested part at container
// +0xe8 that is `*phWnd = pContainer->m_pWnd(+0x40)->m_hWnd(+0x40);
// return S_OK;` -- with no null check on either pointer.
// Left a stub: OpenMFC's container has no nested XOleIPFrame subobject, so
// `pThis` cannot be converted back into the container and m_pWnd is
// unreachable.  The out parameter is cleared so a caller that trusts the S_OK
// sees "no window" rather than an uninitialised handle.
extern "C" long MS_ABI impl__GetWindow_XOleIPFrame_COleControlContainer__UEAAJPEAPEAUHWND_____Z(
    void* pThis, HWND* phWnd) {
    (void)pThis;
    if (!phWnd) {
        return E_POINTER;
    }
    *phWnd = nullptr;
    return S_OK;
}

// Symbol: ?InsertMenus@XOleIPFrame@COleControlContainer@@UEAAJPEAUHMENU__@@PEAUtagOleMenuGroupWidths@@@Z
extern "C" long MS_ABI impl__InsertMenus_XOleIPFrame_COleControlContainer__UEAAJPEAUHMENU____PEAUtagOleMenuGroupWidths___Z(
    void* pThis, HMENU hmenuShared, OLEMENUGROUPWIDTHS* lpMenuWidths) {
    (void)pThis; (void)hmenuShared; (void)lpMenuWidths;
    return E_NOTIMPL;
}

// Symbol: ?LockContainer@XOleContainer@COleControlContainer@@UEAAJH@Z
extern "C" long MS_ABI impl__LockContainer_XOleContainer_COleControlContainer__UEAAJH_Z(
    void* pThis, int fLock) {
    (void)pThis; (void)fLock;
    return E_NOTIMPL;
}

// Symbol: ?ParseDisplayName@XOleContainer@COleControlContainer@@UEAAJPEAUIBindCtx@@PEA_WPEAKPEAPEAUIMoniker@@@Z
extern "C" long MS_ABI impl__ParseDisplayName_XOleContainer_COleControlContainer__UEAAJPEAUIBindCtx__PEA_WPEAKPEAPEAUIMoniker___Z(
    void* pThis, IBindCtx* pbc, wchar_t* pszDisplayName, unsigned long* pchEaten,
    IMoniker** ppmkOut) {
    (void)pThis; (void)pbc; (void)pszDisplayName; (void)pchEaten; (void)ppmkOut;
    return E_NOTIMPL;
}

// Symbol: ?RemoveMenus@XOleIPFrame@COleControlContainer@@UEAAJPEAUHMENU__@@@Z
extern "C" long MS_ABI impl__RemoveMenus_XOleIPFrame_COleControlContainer__UEAAJPEAUHMENU_____Z(
    void* pThis, HMENU hmenuShared) {
    (void)pThis; (void)hmenuShared;
    return E_NOTIMPL;
}

// Symbol: ?RequestBorderSpace@XOleIPFrame@COleControlContainer@@UEAAJPEBUtagRECT@@@Z
extern "C" long MS_ABI impl__RequestBorderSpace_XOleIPFrame_COleControlContainer__UEAAJPEBUtagRECT___Z(
    void* pThis, const RECT* pborderwidths) {
    (void)pThis; (void)pborderwidths;
    return E_NOTIMPL;
}

// Symbol: ?SetActiveObject@XOleIPFrame@COleControlContainer@@UEAAJPEAUIOleInPlaceActiveObject@@PEB_W@Z
// COleControlContainer::XOleIPFrame::SetActiveObject() — retail 0x2350a0:
//     COleControlSite* pSite = pContainer->m_pSiteUIActive(+0xc8);
//     if (pSite == NULL) return S_OK;                 // the early exit
//     IOleInPlaceActiveObject* pOld = pSite->(+0x90);
//     if (pActiveObject) pActiveObject->AddRef();
//     pContainer->m_pSiteUIActive->(+0x90) = pActiveObject;
//     if (pOld) pOld->Release();
//     return S_OK;
// (The container is re-read from `this` for the store, not cached.)
// Left a stub: OpenMFC's container has no nested XOleIPFrame subobject, so
// `pThis` cannot be converted back into the container, and its site has no
// +0x90 member to store the active object in either.  Doing nothing and
// returning S_OK is what retail does whenever no site is UI-active, which is
// the only branch this file can justify.
extern "C" long MS_ABI impl__SetActiveObject_XOleIPFrame_COleControlContainer__UEAAJPEAUIOleInPlaceActiveObject__PEB_W_Z(
    void* pThis, IOleInPlaceActiveObject* pActiveObject, const wchar_t* pszObjName) {
    (void)pThis; (void)pActiveObject; (void)pszObjName;
    return S_OK;
}

// Symbol: ?SetBorderSpace@XOleIPFrame@COleControlContainer@@UEAAJPEBUtagRECT@@@Z
extern "C" long MS_ABI impl__SetBorderSpace_XOleIPFrame_COleControlContainer__UEAAJPEBUtagRECT___Z(
    void* pThis, const RECT* pborderwidths) {
    (void)pThis; (void)pborderwidths;
    return E_NOTIMPL;
}

// Symbol: ?SetMenu@XOleIPFrame@COleControlContainer@@UEAAJPEAUHMENU__@@PEAXPEAUHWND__@@@Z
extern "C" long MS_ABI impl__SetMenu_XOleIPFrame_COleControlContainer__UEAAJPEAUHMENU____PEAXPEAUHWND_____Z(
    void* pThis, HMENU hmenuShared, void* holemenu, HWND hwndActiveObject) {
    (void)pThis; (void)hmenuShared; (void)holemenu; (void)hwndActiveObject;
    return E_NOTIMPL;
}

// Symbol: ?SetStatusText@XOleIPFrame@COleControlContainer@@UEAAJPEB_W@Z
extern "C" long MS_ABI impl__SetStatusText_XOleIPFrame_COleControlContainer__UEAAJPEB_W_Z(
    void* pThis, const wchar_t* pszStatusText) {
    (void)pThis; (void)pszStatusText;
    return E_NOTIMPL;
}

// Symbol: ?TranslateAcceleratorW@XOleIPFrame@COleControlContainer@@UEAAJPEAUtagMSG@@G@Z
extern "C" long MS_ABI impl__TranslateAcceleratorW_XOleIPFrame_COleControlContainer__UEAAJPEAUtagMSG__G_Z(
    void* pThis, MSG* lpmsg, unsigned short wID) {
    (void)pThis; (void)lpmsg; (void)wID;
    return E_NOTIMPL;
}
