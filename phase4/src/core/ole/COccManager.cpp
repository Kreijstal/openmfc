// COccManager — OpenMFC implementation.
//
// =============================================================================
// COccManager, decoded from the retail export bodies (mfc140u.dll).
//
// Every RVA quoted in this file is an mfc140u.dll RVA, read with
// `disas.py --u` / the ordinal join described in the header of
// core/ole/COleControl.cpp (eight of these exports fold in the RVA map and
// were resolved through the export address table by ordinal; the five of
// those eight that are virtual cross-check against the COccManager vftable
// at mfc140u 0x32af30, which holds the same addresses -- SetDefaultButton,
// UIActivateControl and UIDeactivateIfNecessary are statics and are not in
// it).  IAT slots were resolved with iatu.py; the names below are the resolved imports, written as
// the UNICODE-neutral macro where the ANSI/Unicode pair exists.
//
// The class.  Retail's COccManager (atlmfc/include/afxocc.h:503, read on this
// host) is `class COccManager : public CNoTrackObject` with NO data members:
// the object is a lone vfptr, and the retail default instance is created by
// the 8-byte allocation at 0x2384d0 that stores the vftable at 0x32af30.  Its
// slots, read out of that vftable (mfc140u):
//
//    0  0x238500  scalar deleting destructor
//    1  0x2373d0  OnEvent
//    2  0x2373f0  CreateContainer
//    3  0x237420  CreateSite(COleControlContainer*, const CControlCreationInfo&)
//    4  0x0071e0  CreateSite(COleControlContainer*)   -- `xor eax,eax; ret`
//    5  0x237490  PreCreateDialog
//    6  0x237600  PostCreateDialog
//    7  0x237660  SplitDialogTemplate
//    8  0x237950  CreateDlgControls(CWnd*, void*, _AFX_OCC_DIALOG_INFO*)
//    9  0x237890  CreateDlgControls(CWnd*, LPCTSTR, _AFX_OCC_DIALOG_INFO*)
//   10  0x241fa0  IsDialogMessage
//
// That order is NOT the afxocc.h declaration order.  afxocc.h:513/514 and
// :522/524 declare CreateSite(COleControlContainer*) BEFORE
// CreateSite(COleControlContainer*, const CControlCreationInfo&), and
// CreateDlgControls(LPCTSTR) before CreateDlgControls(void*); MSVC emits
// same-name overloads in REVERSE declaration order, so each of those two
// pairs is swapped in the vftable above (slots 3/4 and 8/9).  Every slot
// number used below was therefore read off that vftable, not counted off the
// header.  An MSVC-compiled client deriving from COccManager gets the same
// layout, because retail's vftable IS MSVC's own compilation of this header,
// so the three retail bodies that dispatch through `this` (CreateSite ->
// slot 4, PreCreateDialog -> slot 7, CreateDlgControls(LPCTSTR) -> slot 8)
// do the same here through the slot helpers below.  OpenMFC never constructs a COccManager
// of its own -- AfxEnableControlContainer (featurepack/CMFC_misc_stubs.cpp:1193)
// only records the pointer it is handed -- so every live `this` reaching these
// exports was built by client code against the real header, and its vtable has
// exactly that layout.
//
// The two classes these bodies create.  OpenMFC declares COleControlContainer
// and COleControlSite in include/openmfc/afxole.h with its OWN member layout
// (mapped onto the retail offsets in the headers of core/ole/
// COleControlContainer.cpp and core/ole/COleControlSite.cpp), so every retail
// member access below is written against the OpenMFC member or the sibling
// thunk that models it, never against a raw retail offset.  Retail members
// that OpenMFC does not model at all are named at the function that needs
// them, and the function is left a stub when the body cannot be reproduced
// without them.
//
// The helper structs retail reads through raw offsets (_AFX_OCC_DIALOG_INFO,
// CControlCreationInfo, DLGTEMPLATEEX, DLGITEMTEMPLATEEX) are forward
// declarations only in OpenMFC's public headers, so their layouts are pinned
// below from the real afxocc.h / atlwin.h on this host and static_asserted;
// the offsets are the ones the disassembly dereferences.
//
// The one retail CWnd member this file keeps needing is m_pCtrlSite (CWnd
// +0xd0, the COleControlSite that owns a control's window) and its sibling
// m_pCtrlCont (CWnd +0xc8, the container a dialog window owns).  OpenMFC's
// CWnd (include/openmfc/afxwin.h) declares neither; the container is kept in
// a side map read through CWnd::GetControlContainer (core/window/CWnd.cpp:2228,
// exported as the thunk used below), and the site link is recovered by
// SiteOfWnd() below, whose comment says exactly what it does and does not
// reproduce.
// =============================================================================

#include "openmfc/afxole.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>

#ifdef __GNUC__
#  define MS_ABI __attribute__((ms_abi))
#else
#  define MS_ABI
#endif

// ---- sibling impl__ exports called by the bodies in this file ----
// (C++ methods of other classes exist in this DLL only as their extern "C"
// impl__ thunks.  Every declaration below matches a definition that exists
// today; the file and line of each is given.)

// core/window/CWnd.cpp:602 / :926 / :786; core/window/Thunks.cpp:1278, :1428,
// :1688 (in that order).
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" int MS_ABI impl__IsWindowEnabled_CWnd__QEBAHXZ(const CWnd* pThis);
extern "C" COleControlContainer* MS_ABI
impl__GetControlContainer_CWnd__QEAAPEAVCOleControlContainer__XZ(CWnd* pThis);
extern "C" int MS_ABI impl__InitControlContainer_CWnd__IEAAHH_Z(CWnd* pThis, int bCreateFromResource);
extern "C" void* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);

// core/ole/COleControlContainer.cpp:313 -- NOTE the placeholder signature.
// The mangled name describes (this, CWnd* pWnd), but the definition takes the
// window as its ONLY parameter and allocates the object itself
// (`return new COleControlContainer(pWnd)`), i.e. it behaves as a factory.
// It is declared here exactly as it is defined, and CreateContainer below
// relies on that factory behaviour; see the headerRequest in the report.
extern "C" void* MS_ABI impl___0COleControlContainer__QEAA_PEAVCWnd___Z(void* pWnd);
// core/ole/COleControlSite.cpp:292 -- same shape: the definition takes the
// container as its only parameter and returns `new COleControlSite(pCtrlCont)`.
extern "C" void* MS_ABI impl___0COleControlSite__QEAA_PEAVCOleControlContainer___Z(void* pCtrlCont);

// core/ole/COleControlContainer.cpp:476 / :744 / :733.
extern "C" int MS_ABI impl__CreateControl_COleControlContainer__QEAAHPEAVCWnd__AEBU_GUID__PEB_WKAEBUtagRECT__IPEAVCFile__HPEA_WPEAPEAVCOleControlSite___Z(
    COleControlContainer* pThis, CWnd* pWndCtrl, const GUID* pClsid, const wchar_t* lpszWindowName,
    unsigned long dwStyle, const RECT* prect, unsigned int nID, CFile* pPersist,
    int bStorage, wchar_t* bstrLicKey, COleControlSite** ppSite);
extern "C" void MS_ABI impl__FreezeAllEvents_COleControlContainer__QEAAXH_Z(
    COleControlContainer* pThis, int bFreeze);
extern "C" int MS_ABI impl__FillListSitesOrWnds_COleControlContainer__UEAAHPEAU_AFX_OCC_DIALOG_INFO___Z(
    COleControlContainer* pThis, void* pOccDialogInfo);

// core/ole/COleControlSite.cpp:846 / :891 / :995 / :1058 / :1263 / :1363.
extern "C" void MS_ABI impl__FreezeEvents_COleControlSite__QEAAXH_Z(COleControlSite* pThis, int bFreeze);
extern "C" unsigned long MS_ABI impl__GetDefBtnCode_COleControlSite__QEAAKXZ(COleControlSite* pThis);
extern "C" unsigned long MS_ABI impl__GetStyle_COleControlSite__UEBAKXZ(COleControlSite* pThis);
extern "C" int MS_ABI impl__IsMatchingMnemonic_COleControlSite__QEAAHPEAUtagMSG___Z(
    COleControlSite* pThis, MSG* pMsg);
extern "C" void MS_ABI impl__SetDefaultButton_COleControlSite__QEAAXH_Z(COleControlSite* pThis, int bDefault);
extern "C" CWnd* MS_ABI impl__SetFocus_COleControlSite__UEAAPEAVCWnd__XZ(COleControlSite* pThis);

// core/runtime/CCmdTarget.cpp:1211.  The signature below is the one the
// mangled name describes (this, idCtrl, pEvent, pHandlerInfo); the definition
// there still carries the generated placeholder list `(unsigned int, void**,
// void**)` and returns 0 -- see OnEvent below and the headerRequest.
extern "C" int MS_ABI impl__OnEvent_CCmdTarget__QEAAHIPEAUAFX_EVENT__PEAUAFX_CMDHANDLERINFO___Z(
    CCmdTarget* pThis, unsigned int idCtrl, void* pEvent, void* pHandlerInfo);

// core/file/Thunks.cpp:1143 / :1168 -- both take the object storage first and
// placement-construct / destroy in it.
extern "C" void* MS_ABI impl___0CMemFile__QEAA_PEAEII_Z(
    void* pThis, unsigned char* lpBuffer, unsigned int nBufferSize, unsigned int nGrowBytes);
extern "C" void MS_ABI impl___1CMemFile__UEAA_XZ(void* pThis);

// core/runtime/Globals.cpp:341; detail/MemcoreSupport.cpp:5 (std::malloc);
// detail/MfcExceptionsSupport.cpp:35 / :603 (both throw).
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
    const wchar_t* lpszResource, const wchar_t* lpszType);
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();

// This file's own exports that other bodies here call directly, as retail
// does (CreateDlgControls(void*) calls CreateDlgControl at 0x237ce0 and
// BindControls at 0x237ba0 non-virtually).
extern "C" int MS_ABI impl__CreateDlgControl_COccManager__IEAAHPEAVCWnd__PEAUHWND____HPEAUDLGITEMTEMPLATE__GPEAEKPEAPEAU3__Z(
    void* pThis, CWnd* pWndParent, HWND hwAfter, int bDialogEx, DLGITEMTEMPLATE* pDlgItem,
    unsigned short nMsg, unsigned char* lpData, unsigned long cb, HWND* phWnd);
extern "C" void MS_ABI impl__BindControls_COccManager__IEAAXPEAVCWnd___Z(void* pThis, CWnd* pWndParent);

namespace {

// ---------------------------------------------------------------------------
// Layouts retail reads through raw offsets.
// ---------------------------------------------------------------------------

// _AFX_OCC_DIALOG_INFO, verbatim from atlmfc/include/afxocc.h:550 on this
// host.  The retail bodies read +0x00, +0x08, +0x10 and +0x18 of it.
struct OccDialogInfoLayout {
    DLGTEMPLATE*     m_pNewTemplate;    // +0x00
    DLGITEMTEMPLATE** m_ppOleDlgItems;  // +0x08
    unsigned         m_cItems;          // +0x10
    struct ItemInfo {
        unsigned nId;                   // +0x00
        BOOL     bAutoRadioButton;      // +0x04
    };
    ItemInfo*        m_pItemInfo;       // +0x18
};
static_assert(offsetof(OccDialogInfoLayout, m_ppOleDlgItems) == 0x08, "m_ppOleDlgItems at +0x08");
static_assert(offsetof(OccDialogInfoLayout, m_cItems) == 0x10, "m_cItems at +0x10");
static_assert(offsetof(OccDialogInfoLayout, m_pItemInfo) == 0x18, "m_pItemInfo at +0x18");
static_assert(sizeof(OccDialogInfoLayout::ItemInfo) == 8, "ItemInfo is two dwords");
static_assert(sizeof(OccDialogInfoLayout) == 0x20, "_AFX_OCC_DIALOG_INFO is 32 bytes");

// CControlCreationInfo (afxocc.h:77): HandleKind m_hk at +0, intptr_t
// m_nHandle at +8, CLSID m_clsid at +16 -- the same layout
// core/ole/COleControlContainer.cpp pins.  CreateSite reads only m_hk.
struct CreationInfoLayout {
    int      m_hk;
    int      _pad0;
    intptr_t m_nHandle;
    CLSID    m_clsid;
};
static_assert(sizeof(CreationInfoLayout) == 32, "CControlCreationInfo is 32 bytes");
const int kCreationInfoNullHandle = 2;   // CControlCreationInfo::NullHandle

// DLGTEMPLATEEX / DLGITEMTEMPLATEEX, verbatim from atlmfc/include/atlwin.h:412
// and :436 (pack(1)).  The retail template walkers use sizeof == 0x1a / 0x18
// and the field offsets asserted here.
#pragma pack(push, 1)
struct DlgTemplateExLayout {
    WORD  dlgVer;      // +0x00
    WORD  signature;   // +0x02  0xFFFF marks the EX form
    DWORD helpID;      // +0x04
    DWORD exStyle;     // +0x08
    DWORD style;       // +0x0c
    WORD  cDlgItems;   // +0x10
    short x, y, cx, cy;
};
struct DlgItemTemplateExLayout {
    DWORD helpID;      // +0x00
    DWORD exStyle;     // +0x04
    DWORD style;       // +0x08
    short x;           // +0x0c
    short y;           // +0x0e
    short cx;          // +0x10
    short cy;          // +0x12
    DWORD id;          // +0x14
};
#pragma pack(pop)
static_assert(sizeof(DlgTemplateExLayout) == 0x1a, "DLGTEMPLATEEX is 26 bytes");
static_assert(offsetof(DlgTemplateExLayout, style) == 0x0c, "DLGTEMPLATEEX::style at +0x0c");
static_assert(offsetof(DlgTemplateExLayout, cDlgItems) == 0x10, "DLGTEMPLATEEX::cDlgItems at +0x10");
static_assert(sizeof(DlgItemTemplateExLayout) == 0x18, "DLGITEMTEMPLATEEX is 24 bytes");
static_assert(offsetof(DlgItemTemplateExLayout, id) == 0x14, "DLGITEMTEMPLATEEX::id at +0x14");
static_assert(sizeof(DLGTEMPLATE) == 0x12, "DLGTEMPLATE is 18 bytes");
static_assert(sizeof(DLGITEMTEMPLATE) == 0x12, "DLGITEMTEMPLATE is 18 bytes");
static_assert(offsetof(DLGTEMPLATE, cdit) == 0x08, "DLGTEMPLATE::cdit at +0x08");
static_assert(offsetof(DLGITEMTEMPLATE, id) == 0x10, "DLGITEMTEMPLATE::id at +0x10");

// The OCC DLGINIT message numbers the retail bodies compare against
// (0x376..0x378 and 0x37a..0x37b at 0x1802379f5-0x180237a21 and
// 0x180237e16-0x180237e32).
const WORD kOccLoadFromStream    = 0x376;   // WM_OCC_LOADFROMSTREAM
const WORD kOccLoadFromStorage   = 0x377;   // WM_OCC_LOADFROMSTORAGE
const WORD kOccInitNew           = 0x378;   // WM_OCC_INITNEW
const WORD kOccLoadFromStreamEx  = 0x37a;   // WM_OCC_LOADFROMSTREAM_EX
const WORD kOccLoadFromStorageEx = 0x37b;   // WM_OCC_LOADFROMSTORAGE_EX
const DWORD kDispidDataSource    = 0x80010001;   // the default-binding record tag (0x180237e6a)
const WORD kRtDlgInit            = 240;          // RT_DLGINIT (0x1802378b0)

// Unaligned reads out of resource data (the retail bodies read these as
// UNALIGNED WORD/DWORD).
inline WORD  RdW(const void* p)  { WORD  v; std::memcpy(&v, p, sizeof(v)); return v; }
inline DWORD RdDW(const void* p) { DWORD v; std::memcpy(&v, p, sizeof(v)); return v; }

// ---------------------------------------------------------------------------
// COccManager vtable dispatch.  `this` is a client-built object whose vtable
// is in afxocc.h declaration order (see the file header); the slot numbers
// are the ones the retail call sites use.
// ---------------------------------------------------------------------------
typedef COleControlSite* (MS_ABI* PfnCreateSiteCont)(void* pThis, COleControlContainer* pCtrlCont);
typedef DLGTEMPLATE* (MS_ABI* PfnSplitDialogTemplate)(void* pThis, const DLGTEMPLATE* pTemplate,
                                                      DLGITEMTEMPLATE** ppOleDlgItems);
typedef int (MS_ABI* PfnCreateDlgControlsRes)(void* pThis, CWnd* pWndParent, void* lpResource,
                                              void* pOccDialogInfo);

inline void* OccManagerSlot(void* pThis, int slot) {
    return (*static_cast<void***>(pThis))[slot];
}
const int kSlot_CreateSiteCont       = 4;   // 0x20 at 0x180237433
const int kSlot_SplitDialogTemplate  = 7;   // 0x38 at 0x1802374f2
const int kSlot_CreateDlgControlsRes = 8;   // 0x40 at 0x18023790a

// ---------------------------------------------------------------------------
// Dialog-template walkers.  Retail keeps these as two unexported helpers that
// every template body here calls: 0x2372a4 (first item) and 0x237354 (next
// item).  MFC's published sources call them _AfxFindFirstDlgItem /
// _AfxFindNextDlgItem; neither name is in this host's headers or in the
// export table, so the naming is unverified -- the RVAs are what matter and
// the bodies below are transcribed from them.
// ---------------------------------------------------------------------------
inline bool IsDialogExTemplate(const void* pTemplate) {
    return RdW(static_cast<const BYTE*>(pTemplate) + 2) == 0xFFFF;
}
inline WORD DlgTemplateItemCount(const void* pTemplate) {
    return RdW(static_cast<const BYTE*>(pTemplate) + (IsDialogExTemplate(pTemplate) ? 0x10 : 0x08));
}
inline void SetDlgTemplateItemCount(void* pTemplate, WORD n) {
    std::memcpy(static_cast<BYTE*>(pTemplate) + (IsDialogExTemplate(pTemplate) ? 0x10 : 0x08), &n, sizeof(n));
}
// sz_Or_Ord: 0xFFFF followed by one ordinal word, else a NUL-terminated string.
inline const WORD* SkipOrdinalOrString(const WORD* pw) {
    if (*pw == 0xFFFF) {
        return pw + 2;
    }
    while (*pw++) {
    }
    return pw;
}

// 0x2372a4: a NULL template throws AfxThrowInvalidArgException (0x227720);
// the header is 0x1a bytes (EX) or 0x12 (classic) with the style at +0x0c /
// +0x00; skip menu, class and title; under DS_SETFONT skip 6 bytes (EX:
// pointsize, weight, italic+charset) or 2 (classic: pointsize) plus the face
// name; round up to a DWORD boundary.
DLGITEMTEMPLATE* FindFirstDlgItem(const DLGTEMPLATE* pTemplate) {
    if (!pTemplate) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    const bool bDialogEx = IsDialogExTemplate(pTemplate);
    const BYTE* pb = reinterpret_cast<const BYTE*>(pTemplate);
    const DWORD dwStyle = RdDW(pb + (bDialogEx ? 0x0c : 0x00));
    const WORD* pw = reinterpret_cast<const WORD*>(pb + (bDialogEx ? 0x1a : 0x12));
    pw = SkipOrdinalOrString(pw);   // menu
    pw = SkipOrdinalOrString(pw);   // class
    while (*pw++) {                 // title
    }
    if (dwStyle & DS_SETFONT) {
        pw = reinterpret_cast<const WORD*>(reinterpret_cast<const BYTE*>(pw) + (bDialogEx ? 6 : 2));
        while (*pw++) {             // face name
        }
    }
    return reinterpret_cast<DLGITEMTEMPLATE*>((reinterpret_cast<ULONG_PTR>(pw) + 3) & ~static_cast<ULONG_PTR>(3));
}

// 0x237354: the item header is 0x18 bytes (EX) or 0x12 (classic); skip class
// and title; then the creation-data word: an extra-data count of zero stays
// zero, otherwise the classic form's count includes its own word and is
// reduced by 2 while the EX form's is taken as is; round up to a DWORD
// boundary past the count word and the data.
DLGITEMTEMPLATE* FindNextDlgItem(const DLGITEMTEMPLATE* pItem, int bDialogEx) {
    const WORD* pw = reinterpret_cast<const WORD*>(
        reinterpret_cast<const BYTE*>(pItem) + (bDialogEx ? 0x18 : 0x12));
    pw = SkipOrdinalOrString(pw);   // class
    pw = SkipOrdinalOrString(pw);   // title
    WORD cbExtra = *pw;
    if (cbExtra != 0 && !bDialogEx) {
        cbExtra = static_cast<WORD>(cbExtra - 2);
    }
    return reinterpret_cast<DLGITEMTEMPLATE*>(
        (reinterpret_cast<ULONG_PTR>(pw) + 2 + cbExtra + 3) & ~static_cast<ULONG_PTR>(3));
}

// The item's window-class string: EX items carry it at +0x18, classic at +0x12.
inline const WORD* DlgItemClassName(const DLGITEMTEMPLATE* pItem, int bDialogEx) {
    return reinterpret_cast<const WORD*>(reinterpret_cast<const BYTE*>(pItem) + (bDialogEx ? 0x18 : 0x12));
}

// ---------------------------------------------------------------------------
// The retail CWnd::m_pCtrlSite link (CWnd +0xd0).
//
// Retail stores the COleControlSite that owns a control window into that
// window's CWnd in COleControlSite::AttachWindow, at the moment the site's
// m_hWnd (+0x48) becomes that window's handle (see the AttachWindow
// transcription at core/ole/COleControlSite.cpp:654).  OpenMFC's CWnd has no
// m_pCtrlSite, so the link is recovered from the same fact: the site in the
// PARENT window's container whose m_hWnd equals this window's handle.  The
// parent's container is OpenMFC's stand-in for the retail CWnd::m_pCtrlCont
// (+0xc8) and is reachable only for a permanent CWnd, so a control whose
// parent is not a permanent OpenMFC window resolves to NULL, exactly as a
// retail CWnd that was never attached to a site does.  Sites that have not
// captured a window yet (m_hWnd NULL) never match.
// ---------------------------------------------------------------------------
COleControlSite* SiteOfWnd(const CWnd* pWnd) {
    if (!pWnd || !pWnd->m_hWnd) {
        return nullptr;
    }
    HWND hParent = ::GetParent(pWnd->m_hWnd);
    if (!hParent) {
        return nullptr;
    }
    CWnd* pParent = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hParent);
    if (!pParent) {
        return nullptr;
    }
    COleControlContainer* pCont = impl__GetControlContainer_CWnd__QEAAPEAVCOleControlContainer__XZ(pParent);
    if (!pCont) {
        return nullptr;
    }
    CPtrList::POSITION pos = pCont->m_listSites.GetHeadPosition();
    while (pos != CPtrList::POSITION(nullptr)) {
        COleControlSite* pSite = static_cast<COleControlSite*>(pCont->m_listSites.GetNext(pos));
        if (pSite && pSite->m_hWnd == pWnd->m_hWnd) {
            return pSite;
        }
    }
    return nullptr;
}

// The retail site keeps the control's OLEMISC_* word cached at +0xb0 (filled
// from IOleObject::GetMiscStatus when the control is created).  OpenMFC's site
// has no such member (core/ole/COleControlSite.cpp header), so, as that file
// and core/ole/COleControlContainer.cpp do, it is queried from the object.
DWORD SiteMiscStatusOf(COleControlSite* pSite) {
    DWORD dwMisc = 0;
    if (pSite && pSite->m_lpObject) {
        if (FAILED(pSite->m_lpObject->GetMiscStatus(DVASPECT_CONTENT, &dwMisc))) {
            dwMisc = 0;
        }
    }
    return dwMisc;
}

// Stack CMemFile built and torn down through the exported thunks (the C++
// class is declared in openmfc/afx.h, so its size is known here, but its
// members exist in this DLL only as thunks).
struct StackMemFile {
    alignas(16) unsigned char storage[sizeof(CMemFile)];
    StackMemFile(unsigned char* lpBuffer, unsigned int nBufferSize) {
        impl___0CMemFile__QEAA_PEAEII_Z(storage, lpBuffer, nBufferSize, 0);
    }
    ~StackMemFile() { impl___1CMemFile__UEAA_XZ(storage); }
    CFile* file() { return reinterpret_cast<CFile*>(storage); }
};

}  // namespace

// Symbol: ?BindControls@COccManager@@IEAAXPEAVCWnd@@@Z
// Not implemented.  Retail 0x237ba0 walks pWndParent->m_pCtrlCont(+0xc8)->
// m_listSitesOrWnds (head at +0x88) and, for every entry with a site (+0x08):
//   * walks the site's bound-property list at site+0xe0 (CDataBoundProperty
//     nodes: control id at +0x08, DSC site at +0x10, next at +0x20), points
//     each node at pWndParent->GetDlgItem(id)->m_pCtrlSite(+0xd0) and calls
//     CDataBoundProperty::Notify (0x2401a0) when that changed;
//   * if the WORD at site+0xe8 (the default-binding DSC control id written by
//     CreateDlgControl) is non-zero, resolves that control's site the same
//     way, calls EnableDSC on it (site vtable slot 0x178), overwrites +0xe8
//     with the DSC site pointer and calls slot 3 of the DSC site's data-source
//     object at +0xd8 with (site, TRUE);
// then makes a second pass calling slot 4 of every site's +0xd8 object.
// None of +0xd8 / +0xe0 / +0xe8 exists on OpenMFC's COleControlSite, and its
// BindProperty / BindDefaultProperty / EnableDSC exports are themselves stubs
// for that reason (core/ole/COleControlSite.cpp:675-701 and :831), so there is
// nothing here to walk or bind; the export is a no-op.
extern "C" void MS_ABI impl__BindControls_COccManager__IEAAXPEAVCWnd___Z(void* pThis, CWnd* pWndParent) {
    (void)pThis;
    (void)pWndParent;
}

// Symbol: ?CreateContainer@COccManager@@UEAAPEAVCOleControlContainer@@PEAVCWnd@@@Z
// COccManager::CreateContainer(CWnd*) -- retail 0x2373f0 (mfc140u):
//     void* p = operator new(0xf8);                 // 0x27f0
//     return p ? COleControlContainer::COleControlContainer(p, pWnd)  // 0x235300
//              : NULL;
// i.e. `return new COleControlContainer(pWnd)`, 0xf8 being retail's
// sizeof(COleControlContainer).  In this DLL the object is OpenMFC's own
// COleControlContainer (include/openmfc/afxole.h), and the exported
// constructor thunk is the factory that allocates and constructs it (see its
// declaration above), so the whole body is that one call.  Retail does not
// register the container anywhere -- CWnd::InitControlContainer is what
// stores it -- and neither does this.
extern "C" COleControlContainer* MS_ABI impl__CreateContainer_COccManager__UEAAPEAVCOleControlContainer__PEAVCWnd___Z(
    void* pThis, CWnd* pWnd) {
    (void)pThis;
    return static_cast<COleControlContainer*>(impl___0COleControlContainer__QEAA_PEAVCWnd___Z(pWnd));
}

// Symbol: ?CreateDlgControl@COccManager@@IEAAHPEAVCWnd@@PEAUHWND__@@HPEAUDLGITEMTEMPLATE@@GPEAEKPEAPEAU3@@Z
// COccManager::CreateDlgControl(...) -- retail 0x237ce0 (mfc140u).  Retail:
//   1. If bDialogEx, copy the DLGITEMTEMPLATEEX fields (style +0x08,
//      exStyle +0x04, x/y/cx/cy +0x0c..+0x12, LOWORD(id +0x14)) into a local
//      DLGITEMTEMPLATE and take the class name at +0x18; otherwise use the
//      item as is and the class name at +0x12.
//   2. rect = {x, y, x+cx, y+cy}; ::MapDialogRect(pWndParent->m_hWnd(+0x40), &rect).
//   3. License key: if cb >= 4, read a DWORD character count and, when it is
//      non-zero, bstrLicKey = ::SysAllocStringLen(lpData, cch) (OLEAUT32 #4);
//      advance lpData/cb past the count and the characters.
//   4. Data binding (nMsg == 0x37a or 0x37b only): a block {DWORD dwLen;
//      8 bytes; entries...} of dwLen bytes.  Each entry is {DWORD dispid;
//      WORD ctlid}; a dispid of 0x80010001 is the default-binding record and
//      is followed by {DWORD dispidDefault; DWORD vtDefault; 8 bytes; char
//      szField[]} (the field name is ANSI -- retail assigns it to a CString
//      through CStringT::operator=(PCSTR) at 0x1cd480 and advances by the
//      resulting length + 1); any other entry becomes a 0x28-byte
//      CDataBoundProperty node {+0x08 ctlid, +0x0c dispid, +0x20 next}
//      pushed on a local list.  Then cb -= dwLen and nMsg -= 4 (the _EX
//      message becomes its plain form).
//   5. clsid = (pszClassName[0] == L'{') ? ::CLSIDFromString : ::CLSIDFromProgID.
//   6. CMemFile memFile(lpData, cb, 0) (0x22aec0); pFile = (nMsg == 0x378
//      INITNEW) ? NULL : &memFile.
//   7. If SUCCEEDED(hr) && pWndParent->InitControlContainer(TRUE) (0x235130):
//      pWndParent->m_pCtrlCont(+0xc8)->CreateControl(NULL, clsid, NULL,
//      item.style, rect, item.id, pFile, nMsg == 0x377, bstrLicKey, &pSite)
//      (0x235640, the RECT overload); on success pSite->FreezeEvents(TRUE)
//      (0x23b3c0), ::SetWindowPos(pSite->m_hWnd(+0x48), hwAfter, 0,0,0,0,
//      SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE) and the binding stores:
//      site+0xe0 = node list, site+0xf8 = field-name CString, site+0xe8 =
//      ctlid of the default-binding record, site+0xf0/+0xf4 =
//      dispidDefault/vtDefault.
//   8. ::SysFreeString(bstrLicKey) (OLEAUT32 #6) if any; *phWnd = pSite ?
//      pSite->m_hWnd : NULL; ~CMemFile; return pSite != NULL.
// Deviations, all forced by what OpenMFC has:
//   * The container is reached through CWnd::GetControlContainer (OpenMFC's
//     side map) instead of CWnd+0xc8, after the same InitControlContainer
//     call.  OpenMFC's InitControlContainer ignores its argument.
//   * Step 4 is parsed exactly as retail parses it, because the CMemFile in
//     step 6 must start where retail's does, but nothing is stored: OpenMFC's
//     COleControlSite has none of +0xe0/+0xe8/+0xf0/+0xf4/+0xf8 (its
//     BindProperty/BindDefaultProperty are stubs for the same reason), so the
//     binding records are dropped and no CDataBoundProperty nodes are made.
//     The ANSI field name is skipped with strlen, which equals retail's
//     converted-length for single-byte text.
//   * MapDialogRect and the container access are guarded against a NULL
//     pWndParent; retail dereferences it unconditionally.
extern "C" int MS_ABI impl__CreateDlgControl_COccManager__IEAAHPEAVCWnd__PEAUHWND____HPEAUDLGITEMTEMPLATE__GPEAEKPEAPEAU3__Z(
    void* pThis, CWnd* pWndParent, HWND hwAfter, int bDialogEx, DLGITEMTEMPLATE* pDlgItem,
    unsigned short nMsg, unsigned char* lpData, unsigned long cb, HWND* phWnd) {
    (void)pThis;

    // 1. normalise the item
    DLGITEMTEMPLATE item;
    const DLGITEMTEMPLATE* pItem = pDlgItem;
    if (bDialogEx) {
        const DlgItemTemplateExLayout* pEx = reinterpret_cast<const DlgItemTemplateExLayout*>(pDlgItem);
        item.style = pEx->style;
        item.dwExtendedStyle = pEx->exStyle;
        item.x = pEx->x;
        item.y = pEx->y;
        item.cx = pEx->cx;
        item.cy = pEx->cy;
        item.id = static_cast<WORD>(pEx->id);
        pItem = &item;
    }
    const wchar_t* pszClassName = reinterpret_cast<const wchar_t*>(DlgItemClassName(pDlgItem, bDialogEx));

    // 2. position, in dialog units -> pixels
    RECT rect;
    rect.left = pItem->x;
    rect.top = pItem->y;
    rect.right = pItem->x + pItem->cx;
    rect.bottom = pItem->y + pItem->cy;
    ::MapDialogRect(pWndParent ? pWndParent->m_hWnd : nullptr, &rect);

    // 3. license key
    BSTR bstrLicKey = nullptr;
    if (cb >= 4) {
        const DWORD cchLicKey = RdDW(lpData);
        lpData += 4;
        cb -= 4;
        if (cchLicKey != 0) {
            bstrLicKey = ::SysAllocStringLen(reinterpret_cast<const OLECHAR*>(lpData), cchLicKey);
            lpData += cchLicKey * 2;
            cb -= cchLicKey * 2;
        }
    }

    // 4. data-binding block (parsed past; contents dropped -- see above)
    if (static_cast<WORD>(nMsg - kOccLoadFromStreamEx) <= 1) {
        const DWORD dwLen = RdDW(lpData);
        int remaining = static_cast<int>(dwLen) - 12;
        lpData += 12;
        while (remaining != 0) {
            const DWORD dispid = RdDW(lpData);
            lpData += 6;
            remaining -= 6;
            if (dispid == kDispidDataSource) {
                const char* szField = reinterpret_cast<const char*>(lpData + 16);
                const int nField = static_cast<int>(std::strlen(szField));
                lpData += 16 + nField + 1;
                remaining -= nField + 17;
            }
        }
        cb -= dwLen;
        nMsg = static_cast<WORD>(nMsg - 4);
    }

    // 5. class id
    CLSID clsid;
    HRESULT hr;
    if (pszClassName[0] == L'{') {
        hr = ::CLSIDFromString(const_cast<wchar_t*>(pszClassName), &clsid);
    } else {
        hr = ::CLSIDFromProgID(pszClassName, &clsid);
    }

    // 6. persistence stream over the remaining data
    StackMemFile memFile(lpData, static_cast<unsigned int>(cb));
    CFile* pFile = (nMsg == kOccInitNew) ? nullptr : memFile.file();

    // 7. create the control
    COleControlSite* pSite = nullptr;
    if (SUCCEEDED(hr) && pWndParent && impl__InitControlContainer_CWnd__IEAAHH_Z(pWndParent, TRUE)) {
        COleControlContainer* pCtrlCont =
            impl__GetControlContainer_CWnd__QEAAPEAVCOleControlContainer__XZ(pWndParent);
        if (pCtrlCont &&
            impl__CreateControl_COleControlContainer__QEAAHPEAVCWnd__AEBU_GUID__PEB_WKAEBUtagRECT__IPEAVCFile__HPEA_WPEAPEAVCOleControlSite___Z(
                pCtrlCont, nullptr, &clsid, nullptr, pItem->style, &rect, pItem->id, pFile,
                nMsg == kOccLoadFromStorage, bstrLicKey, &pSite)) {
            impl__FreezeEvents_COleControlSite__QEAAXH_Z(pSite, TRUE);
            ::SetWindowPos(pSite->m_hWnd, hwAfter, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
            // retail's binding stores into site+0xe0/+0xe8/+0xf0/+0xf4/+0xf8
            // have no OpenMFC member to land in (see above).
        }
    }

    // 8. tear down
    if (bstrLicKey) {
        ::SysFreeString(bstrLicKey);
    }
    *phWnd = pSite ? pSite->m_hWnd : nullptr;
    return pSite ? TRUE : FALSE;
}

// Symbol: ?CreateDlgControls@COccManager@@UEAAHPEAVCWnd@@PEAXPEAU_AFX_OCC_DIALOG_INFO@@@Z
// COccManager::CreateDlgControls(CWnd*, void* lpResource, _AFX_OCC_DIALOG_INFO*)
// -- retail 0x237950 (mfc140u):
//   if (pOccDialogInfo->m_pNewTemplate == NULL) return TRUE;   // no OLE items
//   hwParent = pWndParent ? pWndParent->m_hWnd : NULL;
//   bDialogEx = m_pNewTemplate->signature == 0xFFFF;
//   bSuccess = TRUE;
//   if (lpResource) {
//       ppOleDlgItems = pOccDialogInfo->m_ppOleDlgItems; iItem = 0; hwAfter = NULL;
//       while (*(WORD*)lpnRes != 0) {              // DLGINIT: {WORD nIDC; WORD nMsg; DWORD dwLen; data}
//           nMsg = lpnRes[1]; dwLen = *(DWORD*)(lpnRes + 2); data = lpnRes + 4 (bytes +8);
//           if (nMsg in 0x376..0x378 or 0x37a..0x37b) {
//               // next OLE item, walking the non-OLE child windows past
//               while ((pDlgItem = ppOleDlgItems[iItem++]) == NULL)
//                   hwAfter = hwAfter ? ::GetWindow(hwAfter, GW_HWNDNEXT)
//                                     : ::GetWindow(hwParent, GW_CHILD);
//               hwNew = NULL;
//               if (pDlgItem == (DLGITEMTEMPLATE*)-1)            // ran off the array
//                   bSuccess = FALSE;
//               else {
//                   bSuccess = CreateDlgControl(pWndParent, hwAfter, bDialogEx, pDlgItem,
//                                               nMsg, data, dwLen, &hwNew);      // 0x237ce0
//                   if (bSuccess && hwNew) {
//                       if (bDialogEx) ::SetWindowContextHelpId(hwNew, ((DLGITEMTEMPLATEEX*)pDlgItem)->helpID);
//                       if (::GetParent(hwNew) == hwParent) hwAfter = hwNew;
//                   }
//               }
//           }
//           lpnRes = data + dwLen;
//           if (!bSuccess) return FALSE;
//       }
//   }
//   if (pWndParent->m_pCtrlCont(+0xc8)) pWndParent->m_pCtrlCont->FreezeAllEvents(FALSE);  // 0x235fc0
//   BindControls(pWndParent);                                                              // 0x237ba0
//   return bSuccess;
// Transcribed as is; the container is read through CWnd::GetControlContainer
// (OpenMFC's stand-in for +0xc8), guarded against a NULL pWndParent that
// retail would dereference, and BindControls is this file's stub (see it).
extern "C" int MS_ABI impl__CreateDlgControls_COccManager__UEAAHPEAVCWnd__PEAXPEAU_AFX_OCC_DIALOG_INFO___Z(
    void* pThis, CWnd* pWndParent, void* lpResource, void* pOccDialogInfo) {
    OccDialogInfoLayout* pInfo = static_cast<OccDialogInfoLayout*>(pOccDialogInfo);
    if (pInfo->m_pNewTemplate == nullptr) {
        return TRUE;
    }
    const HWND hwParent = pWndParent ? pWndParent->m_hWnd : nullptr;
    const int bDialogEx = IsDialogExTemplate(pInfo->m_pNewTemplate) ? TRUE : FALSE;

    int bSuccess = TRUE;
    if (lpResource) {
        DLGITEMTEMPLATE** ppOleDlgItems = pInfo->m_ppOleDlgItems;
        int iItem = 0;
        HWND hwAfter = nullptr;
        unsigned char* lpnRes = static_cast<unsigned char*>(lpResource);
        while (RdW(lpnRes) != 0) {
            const WORD nMsg = RdW(lpnRes + 2);
            const DWORD dwLen = RdDW(lpnRes + 4);
            unsigned char* data = lpnRes + 8;
            if (static_cast<WORD>(nMsg - kOccLoadFromStream) <= 2 ||
                static_cast<WORD>(nMsg - kOccLoadFromStreamEx) <= 1) {
                DLGITEMTEMPLATE* pDlgItem;
                while ((pDlgItem = ppOleDlgItems[iItem++]) == nullptr) {
                    hwAfter = hwAfter ? ::GetWindow(hwAfter, GW_HWNDNEXT)
                                      : ::GetWindow(hwParent, GW_CHILD);
                }
                HWND hwNew = nullptr;
                if (pDlgItem == reinterpret_cast<DLGITEMTEMPLATE*>(-1)) {
                    bSuccess = FALSE;
                } else {
                    bSuccess = impl__CreateDlgControl_COccManager__IEAAHPEAVCWnd__PEAUHWND____HPEAUDLGITEMTEMPLATE__GPEAEKPEAPEAU3__Z(
                        pThis, pWndParent, hwAfter, bDialogEx, pDlgItem, nMsg, data, dwLen, &hwNew);
                    if (bSuccess && hwNew) {
                        if (bDialogEx) {
                            ::SetWindowContextHelpId(
                                hwNew, reinterpret_cast<const DlgItemTemplateExLayout*>(pDlgItem)->helpID);
                        }
                        if (::GetParent(hwNew) == hwParent) {
                            hwAfter = hwNew;
                        }
                    }
                }
            }
            lpnRes = data + dwLen;
            if (!bSuccess) {
                return FALSE;
            }
        }
    }

    if (pWndParent) {
        COleControlContainer* pCtrlCont =
            impl__GetControlContainer_CWnd__QEAAPEAVCOleControlContainer__XZ(pWndParent);
        if (pCtrlCont) {
            impl__FreezeAllEvents_COleControlContainer__QEAAXH_Z(pCtrlCont, FALSE);
        }
    }
    impl__BindControls_COccManager__IEAAXPEAVCWnd___Z(pThis, pWndParent);
    return bSuccess;
}

// Symbol: ?CreateDlgControls@COccManager@@UEAAHPEAVCWnd@@PEB_WPEAU_AFX_OCC_DIALOG_INFO@@@Z
// COccManager::CreateDlgControls(CWnd*, LPCTSTR, _AFX_OCC_DIALOG_INFO*) --
// retail 0x237890 (mfc140u):
//   lpResource = NULL;
//   if (lpszResourceName) {
//       hInst = AfxFindResourceHandle(lpszResourceName, RT_DLGINIT);   // 0x2aeb50, RT_DLGINIT == 240
//       hRsrc = ::FindResource(hInst, lpszResourceName, RT_DLGINIT);
//       if (hRsrc) {
//           hGlobal = ::LoadResource(hInst, hRsrc);
//           if (!hGlobal) return FALSE;              // 0x1802378e6 -> epilogue with eax == 0
//           lpResource = ::LockResource(hGlobal);
//       }
//   }
//   bResult = TRUE;
//   if (lpResource) bResult = CreateDlgControls(pWndParent, lpResource, pOccDialogInfo);   // this, slot 8
//   if (pWndParent->m_pCtrlCont(+0xc8))
//       pWndParent->m_pCtrlCont->FillListSitesOrWnds(pOccDialogInfo);   // container vftable +0x148 = slot 41
//   return bResult;
// The container vftable slot was checked against the mfc140u
// COleControlContainer vftable at 0x32ada8 (+0x148 -> 0x2368c0 =
// FillListSitesOrWnds).  Deviations: OpenMFC's container is a mingw C++ object
// whose vtable is not laid out like retail's, so that call is made to the
// export thunk directly (a client class deriving from COleControlContainer
// and overriding it would not be honoured); that thunk is itself a stub today
// (core/ole/COleControlContainer.cpp:733).  The container is read through
// CWnd::GetControlContainer, guarded against a NULL pWndParent.
extern "C" int MS_ABI impl__CreateDlgControls_COccManager__UEAAHPEAVCWnd__PEB_WPEAU_AFX_OCC_DIALOG_INFO___Z(
    void* pThis, CWnd* pWndParent, const wchar_t* lpszResourceName, void* pOccDialogInfo) {
    void* lpResource = nullptr;
    if (lpszResourceName) {
        const wchar_t* lpszType = MAKEINTRESOURCEW(kRtDlgInit);
        HINSTANCE hInst = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(lpszResourceName, lpszType);
        HRSRC hRsrc = ::FindResourceW(hInst, lpszResourceName, lpszType);
        if (hRsrc) {
            HGLOBAL hGlobal = ::LoadResource(hInst, hRsrc);
            if (!hGlobal) {
                return FALSE;
            }
            lpResource = ::LockResource(hGlobal);
        }
    }

    int bResult = TRUE;
    if (lpResource) {
        PfnCreateDlgControlsRes pfn =
            reinterpret_cast<PfnCreateDlgControlsRes>(OccManagerSlot(pThis, kSlot_CreateDlgControlsRes));
        bResult = pfn(pThis, pWndParent, lpResource, pOccDialogInfo);
    }

    if (pWndParent) {
        COleControlContainer* pCtrlCont =
            impl__GetControlContainer_CWnd__QEAAPEAVCOleControlContainer__XZ(pWndParent);
        if (pCtrlCont) {
            impl__FillListSitesOrWnds_COleControlContainer__UEAAHPEAU_AFX_OCC_DIALOG_INFO___Z(
                pCtrlCont, pOccDialogInfo);
        }
    }
    return bResult;
}

// Symbol: ?CreateSite@COccManager@@UEAAPEAVCOleControlSite@@PEAVCOleControlContainer@@@Z
// COccManager::CreateSite(COleControlContainer*) -- vftable slot 4 of the
// mfc140u COccManager vftable (0x32af30) is 0x71e0, the DLL-wide folded body
// `xor eax,eax ; ret` (the ordinal join resolves this export to the same
// RVA): the legacy overload returns NULL and creates nothing.  Site creation
// happens in the CControlCreationInfo overload below, which calls this one
// first so that a client override can still supply the site.
extern "C" COleControlSite* MS_ABI impl__CreateSite_COccManager__UEAAPEAVCOleControlSite__PEAVCOleControlContainer___Z(
    void* pThis, COleControlContainer* pCtrlCont) {
    (void)pThis;
    (void)pCtrlCont;
    return nullptr;
}

// Symbol: ?CreateSite@COccManager@@UEAAPEAVCOleControlSite@@PEAVCOleControlContainer@@AEBVCControlCreationInfo@@@Z
// COccManager::CreateSite(COleControlContainer*, const CControlCreationInfo&)
// -- retail 0x237420 (mfc140u):
//   pSite = this->CreateSite(pCtrlCont);                       // vftable slot 4 (0x20)
//   if (pSite) return (info.m_hk(+0x00) == NullHandle /*2*/) ? pSite : NULL;
//   pMgr = CProcessLocalObject::GetData(&_afxSiteFactoryMgr, <0x238540>);   // 0x14d100
//   if (pMgr) { pSite = pMgr->CreateSite(pCtrlCont, info); if (pSite) return pSite; }   // 0x2382d0
//   AfxThrowInvalidArgException();                             // 0x227720
// The process-local creator at 0x238540 is CNoTrackObject::operator new(0x48)
// (0x14c860) plus the CControlSiteFactoryMgr ctor (0x2381a0), and that ctor
// creates a default COleControlSiteFactory (an 8-byte object with the
// vftable at 0x32afd0), stores it at +0x40 and passes it to
// RegisterSiteFactory (0x238340); that factory's CreateSite (0x238140) is `new COleControlSite(pCtrlCont)` -- operator
// new(0x180) then the site ctor at 0x2394e0 -- with no look at the info.
// CControlSiteFactoryMgr::CreateSite walks the registered factories in order
// and returns the first non-NULL site.
// Deviation: OpenMFC's process-local CControlSiteFactoryMgr is the file-local
// g4_GetSiteFactoryMgr() in featurepack/CMFC_misc_stubs.cpp:536, unreachable
// from here, and it registers no default factory in any case, so
// client-registered factories (AfxRegisterSiteFactory) are not consulted.
// What IS reproduced is the outcome the retail manager produces when no
// client factory claims the request: the default factory's
// `new COleControlSite(pCtrlCont)`, made through the exported site
// constructor thunk (a factory in this DLL, see its declaration).  That thunk
// allocates with C++ new and cannot return NULL, so the terminal
// AfxThrowInvalidArgException is unreachable through it.
extern "C" COleControlSite* MS_ABI impl__CreateSite_COccManager__UEAAPEAVCOleControlSite__PEAVCOleControlContainer__AEBVCControlCreationInfo___Z(
    void* pThis, COleControlContainer* pCtrlCont, const void* pCreationInfo) {
    PfnCreateSiteCont pfn = reinterpret_cast<PfnCreateSiteCont>(OccManagerSlot(pThis, kSlot_CreateSiteCont));
    COleControlSite* pSite = pfn(pThis, pCtrlCont);
    if (pSite) {
        const CreationInfoLayout* info = static_cast<const CreationInfoLayout*>(pCreationInfo);
        return (info->m_hk == kCreationInfoNullHandle) ? pSite : nullptr;
    }
    pSite = static_cast<COleControlSite*>(impl___0COleControlSite__QEAA_PEAVCOleControlContainer___Z(pCtrlCont));
    if (!pSite) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    return pSite;
}

// Symbol: ?GetDefBtnCode@COccManager@@SAKPEAVCWnd@@@Z
// COccManager::GetDefBtnCode(CWnd*) -- retail 0x241e80 (mfc140u):
//   if (!pWnd) return 0;
//   pSite = pWnd->m_pCtrlSite(+0xd0);
//   if (pSite) {
//       if (!(pSite->m_dwMiscStatus(+0xb0) & OLEMISC_ACTSLIKEBUTTON)) return 0;
//       return ((~pSite->m_dwStyle(+0xac) & 1) + 1) << 4;    // DLGC_DEFPUSHBUTTON / DLGC_UNDEFPUSHBUTTON
//   }
//   return ::SendMessage(pWnd->m_hWnd(+0x40), WM_GETDLGCODE, 0, 0) & (DLGC_DEFPUSHBUTTON | DLGC_UNDEFPUSHBUTTON);
// The site arm is COleControlSite::GetDefBtnCode (0x23ad40) inlined; it is
// called through its thunk here, whose body is that same computation
// (core/ole/COleControlSite.cpp:891).  The site link is SiteOfWnd (see it).
extern "C" unsigned long MS_ABI impl__GetDefBtnCode_COccManager__SAKPEAVCWnd___Z(CWnd* pWnd) {
    if (!pWnd) {
        return 0;
    }
    if (COleControlSite* pSite = SiteOfWnd(pWnd)) {
        return impl__GetDefBtnCode_COleControlSite__QEAAKXZ(pSite);
    }
    return static_cast<unsigned long>(::SendMessage(pWnd->m_hWnd, WM_GETDLGCODE, 0, 0)) &
           (DLGC_DEFPUSHBUTTON | DLGC_UNDEFPUSHBUTTON);
}

// Symbol: ?IsDialogMessageW@COccManager@@UEAAHPEAVCWnd@@PEAUtagMSG@@@Z
// Not implemented.  Retail 0x241fa0 (mfc140u) is the container-aware dialog
// keyboard manager (~650 instructions).  It reads the focus CWnd's
// m_pCtrlSite(+0xd0) and offers keyboard messages to that site's
// IOleInPlaceActiveObject at site+0x90 (TranslateAccelerator, slot 5), then
// implements Tab / arrow / mnemonic / default-button navigation over the
// dialog's m_pCtrlCont(+0xc8)->m_listSitesOrWnds -- the retail list of
// 24-byte COleControlSiteOrWnd records {HWND, site, bAutoRadioButton} that
// FillListSitesOrWnds builds so that OLE sites and plain child windows share
// one tab order -- through two unexported walkers (0x241914, 0x241990) and
// CWnd::GetNextDlgGroupItem(COleControlSiteOrWnd*) (0x2aa1b0), the
// container's focus site (+0xd8), and the two UI-activation helpers below.  OpenMFC's container keeps only a list
// of COleControlSite* (no window records, no +0xd8), its site has no +0x90
// and no CWnd has m_pCtrlSite, so the tab-order walk this body is built on
// cannot be reproduced; substituting ::IsDialogMessage would invent
// behaviour.  Returns FALSE (message not handled).
extern "C" int MS_ABI impl__IsDialogMessageW_COccManager__UEAAHPEAVCWnd__PEAUtagMSG___Z(
    void* pThis, CWnd* pWndDlg, MSG* lpMsg) {
    (void)pThis;
    (void)pWndDlg;
    (void)lpMsg;
    return FALSE;
}

// Symbol: ?IsLabelControl@COccManager@@SAHPEAUCOleControlSiteOrWnd@@@Z
// COccManager::IsLabelControl(COleControlSiteOrWnd*) -- retail 0x241bb0 (mfc140u):
//   if (!pSiteOrWnd) return FALSE;
//   pSite = pSiteOrWnd->m_pSite(+0x08);
//   hWnd  = pSite ? pSite->m_hWnd(+0x48) : pSiteOrWnd->m_hWnd(+0x00);
//   if (hWnd) { if (!::IsWindowEnabled(hWnd)) return FALSE; }
//   else if (pSite && (pSite->GetStyle() /* site vftable slot 32 (0x100) */ & WS_DISABLED)) return FALSE;
//   if (!pSite) return FALSE;
//   return (pSite->m_dwMiscStatus(+0xb0) & OLEMISC_ACTSLIKELABEL) != 0;
// OpenMFC's COleControlSiteOrWnd (include/openmfc/afxole.h:1571) is
// {COleControlSite* m_pSite; CWnd* m_pWnd; 16 bytes}, NOT retail's
// {HWND m_hWnd; COleControlSite* m_pSite; BOOL m_bAutoRadioButton}
// (afxocc.h:484); the body is written against the OpenMFC members, which is
// what every other COleControlSiteOrWnd body in this DLL uses, and the window
// handle of a window entry is m_pWnd->m_hWnd.  GetStyle is called through its
// thunk (OpenMFC's site has no retail-layout vtable), and the misc-status word
// through SiteMiscStatusOf.
extern "C" int MS_ABI impl__IsLabelControl_COccManager__SAHPEAUCOleControlSiteOrWnd___Z(
    COleControlSiteOrWnd* pSiteOrWnd) {
    if (!pSiteOrWnd) {
        return FALSE;
    }
    COleControlSite* pSite = pSiteOrWnd->m_pSite;
    HWND hWnd = pSite ? pSite->m_hWnd : (pSiteOrWnd->m_pWnd ? pSiteOrWnd->m_pWnd->m_hWnd : nullptr);
    if (hWnd) {
        if (!::IsWindowEnabled(hWnd)) {
            return FALSE;
        }
    } else if (pSite && (impl__GetStyle_COleControlSite__UEBAKXZ(pSite) & WS_DISABLED)) {
        return FALSE;
    }
    if (!pSite) {
        return FALSE;
    }
    return (SiteMiscStatusOf(pSite) & OLEMISC_ACTSLIKELABEL) ? TRUE : FALSE;
}

// Symbol: ?IsLabelControl@COccManager@@SAHPEAVCWnd@@@Z
// COccManager::IsLabelControl(CWnd*) -- retail 0x241b60 (mfc140u):
//   if (!pWnd) AfxThrowInvalidArgException();                 // 0x227720
//   if (!pWnd->IsWindowEnabled()) return FALSE;               // 0x2a9b00
//   pSite = pWnd->m_pCtrlSite(+0xd0);
//   if (!pSite) return FALSE;
//   return (pSite->m_dwMiscStatus(+0xb0) & OLEMISC_ACTSLIKELABEL) != 0;
// Site link via SiteOfWnd, misc status via SiteMiscStatusOf (see both).
extern "C" int MS_ABI impl__IsLabelControl_COccManager__SAHPEAVCWnd___Z(CWnd* pWnd) {
    if (!pWnd) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    if (!impl__IsWindowEnabled_CWnd__QEBAHXZ(pWnd)) {
        return FALSE;
    }
    COleControlSite* pSite = SiteOfWnd(pWnd);
    if (!pSite) {
        return FALSE;
    }
    return (SiteMiscStatusOf(pSite) & OLEMISC_ACTSLIKELABEL) ? TRUE : FALSE;
}

// Symbol: ?IsMatchingMnemonic@COccManager@@SAHPEAUCOleControlSiteOrWnd@@PEAUtagMSG@@@Z
// COccManager::IsMatchingMnemonic(COleControlSiteOrWnd*, MSG*) -- retail
// 0x241ae0 (mfc140u):
//   if (!pSiteOrWnd) AfxThrowInvalidArgException();
//   pSite = pSiteOrWnd->m_pSite(+0x08);
//   return pSite && pSite->IsMatchingMnemonic(lpMsg) ? TRUE : FALSE;   // 0x23b250
// Written against OpenMFC's COleControlSiteOrWnd::m_pSite (see the layout
// note at IsLabelControl above).
extern "C" int MS_ABI impl__IsMatchingMnemonic_COccManager__SAHPEAUCOleControlSiteOrWnd__PEAUtagMSG___Z(
    COleControlSiteOrWnd* pSiteOrWnd, MSG* lpMsg) {
    if (!pSiteOrWnd) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    COleControlSite* pSite = pSiteOrWnd->m_pSite;
    if (!pSite) {
        return FALSE;
    }
    return impl__IsMatchingMnemonic_COleControlSite__QEAAHPEAUtagMSG___Z(pSite, lpMsg) ? TRUE : FALSE;
}

// Symbol: ?IsMatchingMnemonic@COccManager@@SAHPEAVCWnd@@PEAUtagMSG@@@Z
// COccManager::IsMatchingMnemonic(CWnd*, MSG*) -- retail 0x241aa0 (mfc140u):
//   if (!pWnd) AfxThrowInvalidArgException();
//   pSite = pWnd->m_pCtrlSite(+0xd0);
//   return pSite && pSite->IsMatchingMnemonic(lpMsg) ? TRUE : FALSE;   // 0x23b250
// Site link via SiteOfWnd.
extern "C" int MS_ABI impl__IsMatchingMnemonic_COccManager__SAHPEAVCWnd__PEAUtagMSG___Z(CWnd* pWnd, MSG* lpMsg) {
    if (!pWnd) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    COleControlSite* pSite = SiteOfWnd(pWnd);
    if (!pSite) {
        return FALSE;
    }
    return impl__IsMatchingMnemonic_COleControlSite__QEAAHPEAUtagMSG___Z(pSite, lpMsg) ? TRUE : FALSE;
}

// Symbol: ?OnEvent@COccManager@@UEAAHPEAVCCmdTarget@@IPEAUAFX_EVENT@@PEAUAFX_CMDHANDLERINFO@@@Z
// COccManager::OnEvent(CCmdTarget*, UINT, AFX_EVENT*, AFX_CMDHANDLERINFO*) --
// retail 0x2373d0 (mfc140u) drops `this`, shifts the four arguments down one
// register and tail-jumps to 0x236e70, which the ordinal join names
// ?OnEvent@CCmdTarget@@QEAAHIPEAUAFX_EVENT@@PEAUAFX_CMDHANDLERINFO@@@Z:
//     return pCmdTarget->OnEvent(idCtrl, pEvent, pHandlerInfo);
// That is the whole body and it is reproduced as the same call.  Note that
// the callee in this DLL (core/runtime/CCmdTarget.cpp:1211) is still a
// generated placeholder that returns 0 without reading its arguments, so
// until it is implemented every event routed through here reports
// "not handled", which is retail's own result for a target with no matching
// event-sink entry.
extern "C" int MS_ABI impl__OnEvent_COccManager__UEAAHPEAVCCmdTarget__IPEAUAFX_EVENT__PEAUAFX_CMDHANDLERINFO___Z(
    void* pThis, CCmdTarget* pCmdTarget, unsigned int idCtrl, void* pEvent, void* pHandlerInfo) {
    (void)pThis;
    return impl__OnEvent_CCmdTarget__QEAAHIPEAUAFX_EVENT__PEAUAFX_CMDHANDLERINFO___Z(
        pCmdTarget, idCtrl, pEvent, pHandlerInfo);
}

// Symbol: ?PostCreateDialog@COccManager@@UEAAXPEAU_AFX_OCC_DIALOG_INFO@@@Z
// COccManager::PostCreateDialog(_AFX_OCC_DIALOG_INFO*) -- retail 0x237600
// (mfc140u), three guarded releases with the pointer nulled after each:
//   if (m_pNewTemplate)  { ::GlobalFree(m_pNewTemplate);  m_pNewTemplate  = NULL; }   // KERNEL32 GlobalFree
//   if (m_ppOleDlgItems) { free(m_ppOleDlgItems);         m_ppOleDlgItems = NULL; }   // CRT free
//   if (m_pItemInfo)     { free(m_pItemInfo);             m_pItemInfo     = NULL; }   // CRT free
// These pair with PreCreateDialog's GlobalAlloc / calloc / operator new (this
// DLL's operator new export is std::malloc, detail/MemcoreSupport.cpp:5, so
// free is the matching release here as it is in retail).
extern "C" void MS_ABI impl__PostCreateDialog_COccManager__UEAAXPEAU_AFX_OCC_DIALOG_INFO___Z(
    void* pThis, void* pOccDialogInfo) {
    (void)pThis;
    OccDialogInfoLayout* pInfo = static_cast<OccDialogInfoLayout*>(pOccDialogInfo);
    if (pInfo->m_pNewTemplate) {
        ::GlobalFree(static_cast<HGLOBAL>(static_cast<void*>(pInfo->m_pNewTemplate)));
        pInfo->m_pNewTemplate = nullptr;
    }
    if (pInfo->m_ppOleDlgItems) {
        std::free(pInfo->m_ppOleDlgItems);
        pInfo->m_ppOleDlgItems = nullptr;
    }
    if (pInfo->m_pItemInfo) {
        std::free(pInfo->m_pItemInfo);
        pInfo->m_pItemInfo = nullptr;
    }
}

// Symbol: ?PreCreateDialog@COccManager@@UEAAPEBUDLGTEMPLATE@@PEAU_AFX_OCC_DIALOG_INFO@@PEBU2@@Z
// COccManager::PreCreateDialog(_AFX_OCC_DIALOG_INFO*, const DLGTEMPLATE*) --
// retail 0x237490 (mfc140u):
//   if (!pOrigTemplate) AfxThrowInvalidArgException();
//   cItems = DlgTemplateItemCount(pOrigTemplate);            // WORD at +0x10 (EX) / +0x08
//   m_ppOleDlgItems = calloc(sizeof(void*), cItems + 1);     // CRT calloc
//   if (!m_ppOleDlgItems) return NULL;
//   m_pNewTemplate = this->SplitDialogTemplate(pOrigTemplate, m_ppOleDlgItems);   // slot 7 (0x38)
//   pItem = _AfxFindFirstDlgItem(pOrigTemplate); bDialogEx = signature == 0xFFFF;
//   m_pItemInfo = operator new(cItems * 8 /* SIZE_MAX on overflow */);            // 0x27f0
//   memset(m_pItemInfo, 0, cItems * 8);
//   m_cItems = cItems;
//   for (i = 0; i < cItems; i++) {
//       pNext = _AfxFindNextDlgItem(pItem, bDialogEx);
//       m_pItemInfo[i].nId = bDialogEx ? ((DLGITEMTEMPLATEEX*)pItem)->id (+0x14, DWORD)
//                                      : pItem->id (+0x10, WORD);
//       pwClass = class-name words (+0x18 / +0x12); style = +0x08 / +0x00;
//       m_pItemInfo[i].bAutoRadioButton =
//           pwClass[0] == 0xFFFF && pwClass[1] == 0x0080 /* button */ &&
//           (style & 0xF) == BS_AUTORADIOBUTTON;
//       pItem = pNext;
//   }
//   return m_pNewTemplate ? m_pNewTemplate : pOrigTemplate;
// Transcribed as is.  The item-info array comes from this DLL's exported
// operator new (std::malloc) so that PostCreateDialog's free matches it.
extern "C" const DLGTEMPLATE* MS_ABI impl__PreCreateDialog_COccManager__UEAAPEBUDLGTEMPLATE__PEAU_AFX_OCC_DIALOG_INFO__PEBU2__Z(
    void* pThis, void* pOccDialogInfo, const DLGTEMPLATE* pOrigTemplate) {
    if (!pOrigTemplate) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    OccDialogInfoLayout* pInfo = static_cast<OccDialogInfoLayout*>(pOccDialogInfo);

    const unsigned cItems = DlgTemplateItemCount(pOrigTemplate);
    pInfo->m_ppOleDlgItems =
        static_cast<DLGITEMTEMPLATE**>(std::calloc(sizeof(DLGITEMTEMPLATE*), static_cast<size_t>(cItems) + 1));
    if (!pInfo->m_ppOleDlgItems) {
        return nullptr;
    }

    PfnSplitDialogTemplate pfnSplit =
        reinterpret_cast<PfnSplitDialogTemplate>(OccManagerSlot(pThis, kSlot_SplitDialogTemplate));
    pInfo->m_pNewTemplate = pfnSplit(pThis, pOrigTemplate, pInfo->m_ppOleDlgItems);

    DLGITEMTEMPLATE* pItem = FindFirstDlgItem(pOrigTemplate);
    const int bDialogEx = IsDialogExTemplate(pOrigTemplate) ? TRUE : FALSE;

    const size_t cbInfo = static_cast<size_t>(cItems) * sizeof(OccDialogInfoLayout::ItemInfo);
    OccDialogInfoLayout::ItemInfo* pItemInfo =
        static_cast<OccDialogInfoLayout::ItemInfo*>(impl___2_YAPEAX_K_Z(cbInfo));
    pInfo->m_pItemInfo = pItemInfo;
    std::memset(pItemInfo, 0, cbInfo);
    pInfo->m_cItems = cItems;

    for (unsigned i = 0; i < cItems; ++i) {
        DLGITEMTEMPLATE* pNext = FindNextDlgItem(pItem, bDialogEx);
        const WORD* pwClass = DlgItemClassName(pItem, bDialogEx);
        DWORD dwStyle;
        if (bDialogEx) {
            const DlgItemTemplateExLayout* pEx = reinterpret_cast<const DlgItemTemplateExLayout*>(pItem);
            pItemInfo[i].nId = pEx->id;
            dwStyle = pEx->style;
        } else {
            pItemInfo[i].nId = pItem->id;
            dwStyle = pItem->style;
        }
        pItemInfo[i].bAutoRadioButton =
            (pwClass[0] == 0xFFFF && pwClass[1] == 0x0080 && (dwStyle & 0xF) == BS_AUTORADIOBUTTON) ? TRUE : FALSE;
        pItem = pNext;
    }

    return pInfo->m_pNewTemplate ? pInfo->m_pNewTemplate : pOrigTemplate;
}

// Symbol: ?SetDefaultButton@COccManager@@SAXPEAVCWnd@@H@Z
// COccManager::SetDefaultButton(CWnd*, BOOL) -- retail 0x241e10 (mfc140u):
//   pSite = pWnd->m_pCtrlSite(+0xd0);
//   if (pSite) { pSite->SetDefaultButton(bDefault); return; }        // 0x23ad60
//   dwCode = ::SendMessage(pWnd->m_hWnd(+0x40), WM_GETDLGCODE, 0, 0);
//   if (dwCode & (bDefault ? DLGC_UNDEFPUSHBUTTON : DLGC_DEFPUSHBUTTON))
//       ::SendMessage(pWnd->m_hWnd, BM_SETSTYLE, bDefault ? BS_DEFPUSHBUTTON : BS_PUSHBUTTON, TRUE);
// Retail dereferences pWnd unconditionally; a NULL pWnd returns here.  Site
// link via SiteOfWnd.
extern "C" void MS_ABI impl__SetDefaultButton_COccManager__SAXPEAVCWnd__H_Z(CWnd* pWnd, int bDefault) {
    if (!pWnd) {
        return;
    }
    if (COleControlSite* pSite = SiteOfWnd(pWnd)) {
        impl__SetDefaultButton_COleControlSite__QEAAXH_Z(pSite, bDefault);
        return;
    }
    const LRESULT dwCode = ::SendMessage(pWnd->m_hWnd, WM_GETDLGCODE, 0, 0);
    if (dwCode & (bDefault ? DLGC_UNDEFPUSHBUTTON : DLGC_DEFPUSHBUTTON)) {
        ::SendMessage(pWnd->m_hWnd, BM_SETSTYLE, bDefault ? BS_DEFPUSHBUTTON : BS_PUSHBUTTON, TRUE);
    }
}

// Symbol: ?SplitDialogTemplate@COccManager@@UEAAPEAUDLGTEMPLATE@@PEBU2@PEAPEAUDLGITEMTEMPLATE@@@Z
// COccManager::SplitDialogTemplate(const DLGTEMPLATE*, DLGITEMTEMPLATE**) --
// retail 0x237660 (mfc140u):
//   pFirstItem = _AfxFindFirstDlgItem(pTemplate);   // 0x2372a4 (throws on NULL)
//   if (!pTemplate) AfxThrowInvalidArgException();
//   cbHeader = (BYTE*)pFirstItem - (BYTE*)pTemplate; cbNewTemplate = cbHeader;
//   bDialogEx = signature == 0xFFFF; nItems = DlgTemplateItemCount(pTemplate);
//   // pass 1: is there an OLE control (class name starts with L'{'), and how
//   // big is a template holding only the other items?
//   bHasOleControls = FALSE;
//   for (pItem = pFirstItem, i = 0; i < nItems; i++, pItem = pNext) {
//       pNext = _AfxFindNextDlgItem(pItem, bDialogEx);                     // 0x237354
//       if (className(pItem)[0] == L'{') bHasOleControls = TRUE;
//       else cbNewTemplate += (BYTE*)pNext - (BYTE*)pItem;
//   }
//   if (!bHasOleControls) { ppOleDlgItems[0] = (DLGITEMTEMPLATE*)-1; return NULL; }
//   pNew = ::GlobalAlloc(GMEM_FIXED, cbNewTemplate); if (!pNew) AfxThrowMemoryException();   // 0x2276c0
//   memcpy_s(pNew, cbNewTemplate, pTemplate, cbHeader); pDst = pNew + cbHeader;
//   DlgTemplateItemCount(pNew) = 0;
//   // pass 2: copy the non-OLE items across, record the OLE items
//   for (pItem = pFirstItem, i = 0; i < nItems; i++, pItem = pNext) {
//       pNext = _AfxFindNextDlgItem(pItem, bDialogEx);
//       if (className(pItem)[0] == L'{') ppOleDlgItems[i] = pItem;
//       else { cb = pNext - pItem; memcpy_s(pDst, ..., pItem, cb); pDst += cb;
//              ++DlgTemplateItemCount(pNew); ppOleDlgItems[i] = NULL; }
//   }
//   ppOleDlgItems[nItems] = (DLGITEMTEMPLATE*)-1;
//   return pNew;
// (A template with zero items takes the no-OLE-controls exit: the pass-1 loop
// never runs, 0x1802376d6.)  The memcpy_s parameter checks retail inlines
// (dest NULL / source NULL / cbHeader > cbNewTemplate -> errno + invalid-
// parameter handler) cannot trigger on the sizes computed above and are
// plain memcpy here.
extern "C" DLGTEMPLATE* MS_ABI impl__SplitDialogTemplate_COccManager__UEAAPEAUDLGTEMPLATE__PEBU2_PEAPEAUDLGITEMTEMPLATE___Z(
    void* pThis, const DLGTEMPLATE* pTemplate, DLGITEMTEMPLATE** ppOleDlgItems) {
    (void)pThis;
    DLGITEMTEMPLATE* pFirstItem = FindFirstDlgItem(pTemplate);
    if (!pTemplate) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    const unsigned cbHeader = static_cast<unsigned>(
        reinterpret_cast<const BYTE*>(pFirstItem) - reinterpret_cast<const BYTE*>(pTemplate));
    unsigned cbNewTemplate = cbHeader;
    const int bDialogEx = IsDialogExTemplate(pTemplate) ? TRUE : FALSE;
    const int nItems = DlgTemplateItemCount(pTemplate);

    bool bHasOleControls = false;
    DLGITEMTEMPLATE* pItem = pFirstItem;
    for (int i = 0; i < nItems; ++i) {
        DLGITEMTEMPLATE* pNext = FindNextDlgItem(pItem, bDialogEx);
        if (DlgItemClassName(pItem, bDialogEx)[0] == L'{') {
            bHasOleControls = true;
        } else {
            cbNewTemplate += static_cast<unsigned>(
                reinterpret_cast<const BYTE*>(pNext) - reinterpret_cast<const BYTE*>(pItem));
        }
        pItem = pNext;
    }
    if (!bHasOleControls) {
        ppOleDlgItems[0] = reinterpret_cast<DLGITEMTEMPLATE*>(-1);
        return nullptr;
    }

    BYTE* pNew = static_cast<BYTE*>(::GlobalAlloc(GMEM_FIXED, cbNewTemplate));
    if (!pNew) {
        impl__AfxThrowMemoryException__YAXXZ();
    }
    DLGTEMPLATE* pNewTemplate = reinterpret_cast<DLGTEMPLATE*>(pNew);
    if (cbHeader) {
        std::memcpy(pNew, pTemplate, cbHeader);
    }
    BYTE* pDst = pNew + cbHeader;
    SetDlgTemplateItemCount(pNewTemplate, 0);

    pItem = pFirstItem;
    for (int i = 0; i < nItems; ++i) {
        DLGITEMTEMPLATE* pNext = FindNextDlgItem(pItem, bDialogEx);
        if (DlgItemClassName(pItem, bDialogEx)[0] == L'{') {
            ppOleDlgItems[i] = pItem;
        } else {
            const unsigned cbItem = static_cast<unsigned>(
                reinterpret_cast<const BYTE*>(pNext) - reinterpret_cast<const BYTE*>(pItem));
            if (cbItem) {
                std::memcpy(pDst, pItem, cbItem);
            }
            pDst += cbItem;
            SetDlgTemplateItemCount(pNewTemplate, static_cast<WORD>(DlgTemplateItemCount(pNewTemplate) + 1));
            ppOleDlgItems[i] = nullptr;
        }
        pItem = pNext;
    }
    ppOleDlgItems[nItems] = reinterpret_cast<DLGITEMTEMPLATE*>(-1);
    return pNewTemplate;
}

// Symbol: ?UIActivateControl@COccManager@@KAXPEAVCWnd@@@Z
// COccManager::UIActivateControl(CWnd* pWndNewFocus) -- retail 0x241c20 (mfc140u):
//   if (!pWndNewFocus) return;
//   pWnd = pWndNewFocus;
//   for (;;) {
//       if (pWnd->m_pCtrlCont(+0xc8)) return;         // reached a container window: nothing to activate
//       pSite = pWnd->m_pCtrlSite(+0xd0); if (pSite) break;
//       pWnd = CWnd::FromHandle(::GetParent(pWnd->m_hWnd(+0x40)));   // 0x28ad70
//       if (!pWnd) return;
//   }
//   pSite->SetFocus();                                 // site vftable slot 46 (0x170) = SetFocus()
//   if (CWnd::FromHandle(::GetFocus()) != pWndNewFocus && ::IsWindow(pWndNewFocus->m_hWnd))
//       pWndNewFocus->SetFocus();                      // 0x2a9b60
// The container test is CWnd::GetControlContainer (OpenMFC's stand-in for
// +0xc8), the site link is SiteOfWnd, and the site's SetFocus is called
// through its thunk (OpenMFC's site has no retail-layout vtable).
extern "C" void MS_ABI impl__UIActivateControl_COccManager__KAXPEAVCWnd___Z(CWnd* pWndNewFocus) {
    if (!pWndNewFocus) {
        return;
    }
    CWnd* pWnd = pWndNewFocus;
    COleControlSite* pSite;
    for (;;) {
        if (impl__GetControlContainer_CWnd__QEAAPEAVCOleControlContainer__XZ(pWnd)) {
            return;
        }
        pSite = SiteOfWnd(pWnd);
        if (pSite) {
            break;
        }
        pWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pWnd->m_hWnd));
        if (!pWnd) {
            return;
        }
    }
    impl__SetFocus_COleControlSite__UEAAPEAVCWnd__XZ(pSite);
    if (impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetFocus()) != pWndNewFocus &&
        ::IsWindow(pWndNewFocus->m_hWnd)) {
        impl__SetFocus_CWnd__QEAAPEAV1_XZ(pWndNewFocus);
    }
}

// Symbol: ?UIDeactivateIfNecessary@COccManager@@KAXPEAVCWnd@@0@Z
// Not implemented.  Retail 0x241cd0 (mfc140u):
//   if (!pWndOldFocus || !::IsWindow(pWndOldFocus->m_hWnd) || pWndOldFocus == pWndNewFocus) return;
//   walk pWndOldFocus up through CWnd::FromHandle(::GetParent(...)) to the first
//   window with m_pCtrlCont(+0xc8); none -> return;
//   pSiteUIActive = pCtrlCont->m_pSiteUIActive(+0xc8); pWndUIActive = NULL;
//   if (pSiteUIActive && !pSiteUIActive->windowless(+0xb4))
//       { pWndUIActive = CWnd::FromHandle(pSiteUIActive->m_hWnd(+0x48)); if (!pWndUIActive) return; }
//   if (pWndNewFocus && pWndUIActive &&
//       (pWndNewFocus == pWndUIActive || ::IsChild(pWndUIActive->m_hWnd, pWndNewFocus->m_hWnd))) return;
//   pCtrlCont->OnUIActivate(NULL);                     // container vftable slot 26 (0xd0) = 0x236070
// The decision hinges on the container's UI-active site.  OpenMFC keeps that
// retail member (+0xc8) in ContainerExtra::pSiteUIActive, a file-local side
// table inside core/ole/COleControlContainer.cpp with no accessor, so the
// "focus is moving inside the UI-active control" exemption cannot be
// evaluated here; calling OnUIActivate(NULL) without it would UI-deactivate a
// control the user is tabbing into.  Left a no-op; see the headerRequest.
extern "C" void MS_ABI impl__UIDeactivateIfNecessary_COccManager__KAXPEAVCWnd__0_Z(
    CWnd* pWndOldFocus, CWnd* pWndNewFocus) {
    (void)pWndOldFocus;
    (void)pWndNewFocus;
}
