// CDataBoundProperty — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// CDataBoundProperty is the node of a COleControlSite's bound-property list
// (retail keeps the list head at COleControlSite+0xe0; afxocc.h:328 names it
// COleControlSite::m_pBindings).  The shipped SDK headers have only a forward
// declaration (afxocc.h:31) -- the full declaration lives in MFC's private
// sources, which are not on this host -- and include/openmfc declares nothing
// for it, so the layout is pinned in this file from the retail constructor.
//
// Bodies are transcribed from the retail disassembly.  disas.py reads the ANSI
// twin mfc140.dll, whose function bodies are byte-identical to mfc140u.dll's
// but sit at different RVAs, so every RVA below names its image.  Three of the
// exports (RemoveSource, SetClientSite, SetDSCSite) do not resolve in the
// mfc140u symbol map; for those only the mfc140 RVA is given.
//
// Object layout -- retail ctor ??0CDataBoundProperty@@QEAA@PEAV0@JG@Z,
// RVA 0x240130 (mfc140u) / 0x23e730 (mfc140):
//     xor  %eax,%eax
//     mov  %r9w,0x8(%rcx)      m_ctlid       = ctlid
//     mov  %rax,(%rcx)         m_pClientSite = NULL
//     mov  %rax,0x10(%rcx)     m_pDSCSite    = NULL
//     mov  %eax,0x1c(%rcx)     m_bIsDirty    = FALSE
//     mov  %rcx,%rax           return this
//     mov  %r8d,0xc(%rcx)      m_dispid      = dispid
//     mov  %rdx,0x20(%rcx)     m_pNext       = pLast
// +0x18 is not written by the ctor; Notify passes its address as the
// BOOL* lpfOwnXferOut argument of IBoundObject::OnSourceChanged, which is what
// identifies it as MFC's m_bOwnXferOut.  COleControlSite::BindProperty
// allocates the node with 0x28 bytes (see the BindProperty comment in
// core/ole/COleControlSite.cpp; mfc140u 0x23c170 does `mov $0x28,%ecx`
// before operator new), so sizeof == 0x28.  The offsets are the retail ones;
// the member names follow MFC's private occimpl.h from memory and were not
// cross-checked against it (it is not on this host) -- only m_bOwnXferOut is
// corroborated, by olebind.h's lpfOwnXferOut parameter name.
//
// The COleControlSite the node points at is NOT laid out like retail in
// OpenMFC (see the mapping table at the top of core/ole/COleControlSite.cpp).
// That table maps retail site+0x80 (IOleObject*) onto
// COleControlSite::m_lpObject, which is what Notify uses below; retail
// site+0xd8 (the CDataSourceControl*) has no OpenMFC equivalent, which is why
// GetCursor stays a stub.

#include "detail/ManualSmallStubImplementationsSupport.h"

// Retail GetCursor is the only way Notify's fallback path gets a cursor; it is
// defined below and declared here so Notify can call it.
extern "C" IUnknown* MS_ABI impl__GetCursor_CDataBoundProperty__QEAAPEAUIUnknown__XZ(void* pThis);
extern "C" void      MS_ABI impl__Notify_CDataBoundProperty__QEAAXXZ(void* pThis);
// detail/MfcExceptionsSupport.cpp
extern "C" void      MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();

namespace {

struct S_DataBoundProperty {
    COleControlSite*     m_pClientSite;   // +0x00
    WORD                 m_ctlid;         // +0x08
    DISPID               m_dispid;        // +0x0c
    COleControlSite*     m_pDSCSite;      // +0x10
    BOOL                 m_bOwnXferOut;   // +0x18 (not initialised by the ctor)
    BOOL                 m_bIsDirty;      // +0x1c
    S_DataBoundProperty* m_pNext;         // +0x20
};
static_assert(offsetof(S_DataBoundProperty, m_pClientSite) == 0x00, "retail ctor: mov %rax,(%rcx)");
static_assert(offsetof(S_DataBoundProperty, m_ctlid)       == 0x08, "retail ctor: mov %r9w,0x8(%rcx)");
static_assert(offsetof(S_DataBoundProperty, m_dispid)      == 0x0c, "retail ctor: mov %r8d,0xc(%rcx)");
static_assert(offsetof(S_DataBoundProperty, m_pDSCSite)    == 0x10, "retail ctor: mov %rax,0x10(%rcx)");
static_assert(offsetof(S_DataBoundProperty, m_bOwnXferOut) == 0x18, "retail Notify: lea 0x18(%rbx),%r9");
static_assert(offsetof(S_DataBoundProperty, m_bIsDirty)    == 0x1c, "retail ctor: mov %eax,0x1c(%rcx)");
static_assert(offsetof(S_DataBoundProperty, m_pNext)       == 0x20, "retail ctor: mov %rdx,0x20(%rcx)");
static_assert(sizeof(S_DataBoundProperty) == 0x28, "retail BindProperty allocates 0x28 bytes");

inline S_DataBoundProperty* Self(void* pThis) {
    return static_cast<S_DataBoundProperty*>(pThis);
}

// afxocc.h: #define DISPID_DATASOURCE 0x80010001 (Notify compares m_dispid
// against the literal `cmpl $0x80010001,0xc(%rcx)`).
const DISPID kDispidDataSource = static_cast<DISPID>(0x80010001);

// IID_IBoundObject {9BFBBC00-EFF1-101A-84ED-00AA00341D07} -- the 16 bytes
// Notify passes to QueryInterface (mfc140u VA 0x1802d98b8 / mfc140 VA
// 0x1802d7808, each read from its own image and identical; the value matches
// atlmfc/include/olebind.h:23).
const GUID kIID_IBoundObject =
    { 0x9BFBBC00, 0xEFF1, 0x101A, { 0x84, 0xED, 0x00, 0xAA, 0x00, 0x34, 0x1D, 0x07 } };

// IBoundObject as declared in atlmfc/include/olebind.h: IUnknown, then
// OnSourceChanged (slot 3, byte offset 0x18 -- the slot Notify calls) and
// IsDirty (slot 4).
struct IBoundObjectShadow : public IUnknown {
    virtual HRESULT STDMETHODCALLTYPE OnSourceChanged(DISPID dispid, BOOL fBound, BOOL* lpfOwnXferOut) = 0;
    virtual HRESULT STDMETHODCALLTYPE IsDirty(DISPID dispid) = 0;
};

} // namespace

// Symbol: ??0CDataBoundProperty@@QEAA@PEAV0@JG@Z
// CDataBoundProperty::CDataBoundProperty(CDataBoundProperty* pLast, DISPID
// dispid, WORD ctlid) -- RVA 0x240130 (mfc140u) / 0x23e730 (mfc140); the full
// body is quoted in the layout comment at the top of this file.  The previous
// parameter list here was an auto-generated placeholder (pointer, uint64,
// float) that did not match the mangled name (PEAV0@ J G).
extern "C" void* MS_ABI impl___0CDataBoundProperty__QEAA_PEAV0_JG_Z(
    void* pThis, void* pLast, long dispid, unsigned short ctlid) {
    S_DataBoundProperty* p = Self(pThis);
    p->m_ctlid = ctlid;
    p->m_pClientSite = nullptr;
    p->m_pDSCSite = nullptr;
    p->m_bIsDirty = FALSE;
    p->m_dispid = dispid;
    p->m_pNext = static_cast<S_DataBoundProperty*>(pLast);
    return pThis;
}

// Symbol: ?GetCursor@CDataBoundProperty@@QEAAPEAUIUnknown@@XZ
// Only the null-DSC path is implemented.  Retail RVA 0x240280 (mfc140u) /
// 0x23e880 (mfc140):
//   if (m_pDSCSite(+0x10) == NULL) return NULL;
//   m_pDSCSite->EnableDSC();                              // site vtable +0x178
//   m_pDSCSite->m_pDataSourceControl(+0xd8)->BindProp(this, TRUE);   // DSC vtable +0x10
//   return m_pDSCSite->m_pDataSourceControl->GetCursor();  // DSC vtable +0x08, tail jump
// (The three indirect calls go through the CFG dispatch pointer with the
// target in %rax; the DSC slot names are those in core/db/CDataSourceControl.cpp.)
// Not reproduced: OpenMFC's COleControlSite has no member for the retail
// site+0xd8 CDataSourceControl* and its EnableDSC thunk is a no-op, so there is
// no data-source control to bind to or take a cursor from.  With a non-null
// DSC site this returns NULL where retail returns the DSC's cursor.
extern "C" IUnknown* MS_ABI impl__GetCursor_CDataBoundProperty__QEAAPEAUIUnknown__XZ(void* pThis) {
    S_DataBoundProperty* p = Self(pThis);
    if (p->m_pDSCSite == nullptr) return nullptr;
    return nullptr;   // see above: the DSC path has nothing to run against
}

// Symbol: ?GetNext@CDataBoundProperty@@QEAAPEAV1@XZ
// Retail RVA 0x240180 (mfc140u) / 0x23e780 (mfc140):
//   mov 0x20(%rcx),%rax; ret      -> return m_pNext;
extern "C" void* MS_ABI impl__GetNext_CDataBoundProperty__QEAAPEAV1_XZ(void* pThis) {
    return Self(pThis)->m_pNext;
}

// Symbol: ?Notify@CDataBoundProperty@@QEAAXXZ
// Retail RVA 0x2401a0 (mfc140u) / 0x23e7a0 (mfc140):
//   if (m_dispid(+0xc) == DISPID_DATASOURCE) return;
//   if (m_pClientSite(+0x0) == NULL) return;
//   IUnknown* pObj = m_pClientSite->m_pObject(site+0x80);
//   if (pObj == NULL) AfxThrowInvalidArgException();       // call 0x225b80 (mfc140)
//   IBoundObject* pBO;
//   if (SUCCEEDED(pObj->QueryInterface(IID_IBoundObject, &pBO))) {   // slot 0
//       pBO->OnSourceChanged(m_dispid, m_pDSCSite(+0x10) != NULL,
//                            &m_bOwnXferOut(+0x18));                // slot 3 (+0x18)
//       pBO->Release();                                             // slot 2 (+0x10)
//   } else {
//       IUnknown* pUnk = GetCursor();                               // direct call
//       if (pUnk != NULL)
//           m_pClientSite->SetProperty(m_dispid,
//               VT_UNKNOWN | (m_pDSCSite->m_pDataSourceControl(site+0xd8) ? 0x8000 : 0),
//               pUnk);                                              // site vtable +0xf0
//   }
// (Vtable slot +0xf0 of COleControlSite is SetProperty: the entry there in the
// mfc140u vftable (0x18032b308, loaded by the COleControlSite ctor) is
// 0x23b7b0, which has no mfc140u symbol-map entry but is the same instruction
// sequence -- differing only in the rip-relative displacement to the CFG
// dispatch pointer -- as mfc140's ?SetProperty@COleControlSite@@UEAAXJGZZ at
// 0x239db0.  The VARTYPE is built as `neg; sbb; and $0x8000; add $0xd`, i.e.
// VT_UNKNOWN (0xd) plus 0x8000 when site+0xd8 is non-null; 0x8000 is
// VT_MFCFORCEPUTREF (afxocc.h:214).)
//
// The retail site+0x80 member maps onto COleControlSite::m_lpObject (the
// mapping table at the top of core/ole/COleControlSite.cpp).
//
// Deviation, on a path that is unreachable in OpenMFC: the else branch stops
// after GetCursor.  GetCursor above can only return NULL here (OpenMFC has no
// CDataSourceControl behind a site), and the SetProperty step could not be
// written faithfully anyway -- OpenMFC's COleControlSite has no retail
// vtable, and the ?SetProperty@COleControlSite@@UEAAXJGZZ thunk in
// core/ole/Thunks.cpp is declared without the variadic value argument, so
// calling it would drop pUnk.
extern "C" void MS_ABI impl__Notify_CDataBoundProperty__QEAAXXZ(void* pThis) {
    S_DataBoundProperty* p = Self(pThis);
    if (p->m_dispid == kDispidDataSource) return;
    COleControlSite* pSite = p->m_pClientSite;
    if (pSite == nullptr) return;
    IUnknown* pObj = pSite->m_lpObject;
    if (pObj == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();

    IBoundObjectShadow* pBO = nullptr;
    if (SUCCEEDED(pObj->QueryInterface(kIID_IBoundObject, reinterpret_cast<void**>(&pBO)))) {
        pBO->OnSourceChanged(p->m_dispid, p->m_pDSCSite != nullptr, &p->m_bOwnXferOut);
        pBO->Release();
        return;
    }

    IUnknown* pUnk = impl__GetCursor_CDataBoundProperty__QEAAPEAUIUnknown__XZ(pThis);
    if (pUnk == nullptr) return;
    // Unreachable in OpenMFC; retail would SetProperty(m_dispid,
    // VT_UNKNOWN|VT_MFCFORCEPUTREF, pUnk) here -- see the deviation note above.
}

// Symbol: ?RemoveSource@CDataBoundProperty@@QEAAXXZ
// Retail RVA 0x23e790 (mfc140; no mfc140u symbol-map entry):
//   movq $0x0,0x10(%rcx)     m_pDSCSite = NULL;
//   jmp  0x18023e7a0         tail call Notify (mfc140 0x23e7a0), unconditional
extern "C" void MS_ABI impl__RemoveSource_CDataBoundProperty__QEAAXXZ(void* pThis) {
    Self(pThis)->m_pDSCSite = nullptr;
    impl__Notify_CDataBoundProperty__QEAAXXZ(pThis);
}

// Symbol: ?SetClientSite@CDataBoundProperty@@QEAAXPEAVCOleControlSite@@@Z
// Retail RVA 0x23e750 (mfc140; no mfc140u symbol-map entry):
//   mov %rdx,(%rcx); ret     -> m_pClientSite = pClientSite;
extern "C" void MS_ABI impl__SetClientSite_CDataBoundProperty__QEAAXPEAVCOleControlSite___Z(
    void* pThis, COleControlSite* pClientSite) {
    Self(pThis)->m_pClientSite = pClientSite;
}

// Symbol: ?SetDSCSite@CDataBoundProperty@@QEAAXPEAVCOleControlSite@@@Z
// Retail RVA 0x23e760 (mfc140; no mfc140u symbol-map entry):
//   if (m_pDSCSite(+0x10) != pDSCSite) { m_pDSCSite = pDSCSite; Notify(); }
// (Notify is the direct call to mfc140 0x23e7a0.)
extern "C" void MS_ABI impl__SetDSCSite_CDataBoundProperty__QEAAXPEAVCOleControlSite___Z(
    void* pThis, COleControlSite* pDSCSite) {
    S_DataBoundProperty* p = Self(pThis);
    if (p->m_pDSCSite == pDSCSite) return;
    p->m_pDSCSite = pDSCSite;
    impl__Notify_CDataBoundProperty__QEAAXXZ(pThis);
}
