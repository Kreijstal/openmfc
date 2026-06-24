// OpenMFC: hand-authored MSVC-layout vtable for CMFCToolBarButton (53 slots).
//
// The DLL is built with mingw-gcc (Itanium C++ ABI), whose vtable layout differs from
// MSVC's (Itanium emits TWO destructor slots; MSVC one), so a real MSVC client calling
// CMFCToolBarButton virtuals through the object vptr would mis-dispatch every slot past
// the destructor. This is the same problem the CFile spike (global_cfile_msvtbl.cpp)
// fixed for CFile/CMemFile/CStdioFile; we apply the identical fix here.
//
// The 53-slot order is HARVESTED + cross-checked against real mfc140u in
// tools/harvest/family/cmfctoolbarbutton_vtable.json (every exported virtual found by
// exact-address match in the real vtable lands exactly where SDK-header
// afxtoolbarbutton.h declaration order predicts). CObject overrides
// (~dtor/Serialize/AssertValid/Dump) fold into base slots 1-4; CMFCToolBarButton's own
// 48 new virtuals start at slot 5.
//
// Each slot is a small MS-ABI wrapper. Behavioral slots reproduce the REAL semantics,
// harvested from mfc140u under wine (tools/harvest/family/*_probe.cpp):
//   SetImage(i): user button -> m_iUserImage=i, else m_iImage=i (matches GetImage()).
//   ResetImageToDefault(): NO-OP on the base class (only derived classes override it).
//   CompareWith(other): base compares ONLY m_nID (all other field diffs still compare
//                       equal) -> return m_nID==other.m_nID.
//   CopyFrom(src): full data-member copy (not the runtime m_rect/m_sizeText/m_pWndParent).
//   SetStyle / GetInvalidateRect / IsExtraSize: inline bodies from the SDK header.
// GUI/OLE slots (OnDraw, PrepareDrag, hit-test, ACC, group-position, ...) keep faithful
// signatures and honest minimal returns; we do not fabricate pixel/drag behavior.
//
// vptr is installed by OpenMFC_PatchToolBarButtonVtable, called from the exported ctors
// (thunks.cpp) right after the C++ ctor sets the mingw vptr. Named global_* so
// build_phase4.sh's shard glob compiles it; the // Symbol: markers auto-exclude the weak
// stubs for the methods we give real bodies (Serialize/CopyFrom/CompareWith/SetImage/
// ResetImageToDefault).

#include "openmfc/afx.h"
#include "openmfc/afxmfc.h"
#include <cstdlib>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// GetRuntimeClass slot reuses the real exported descriptor getter (no self-dispatch).
extern "C" CRuntimeClass* MS_ABI
    impl__GetRuntimeClass_CMFCToolBarButton__UEBAPEAUCRuntimeClass__XZ(const void*);

// ---- shared behavioral bodies (used by BOTH the vtable slots and the named exports) ----
namespace {

void tbb_Serialize(CMFCToolBarButton* b, CArchive* par) {
    CArchive& ar = *par;
    if (ar.IsStoring()) {
        ar << b->m_nID;
        ar << b->m_nStyle;
        ar << (int)b->GetImage();          // active image (user->iUserImage else iImage)
        ar << b->m_strText;
        ar << (UINT)b->m_bUserButton;
        ar << (UINT)0;                      // reserved DWORD, always 0 for base button
        ar << (UINT)b->m_bText;
        ar << (UINT)b->m_bImage;
        ar << (UINT)b->m_bVisible;
    } else {
        int img; UINT u, reserved;
        ar >> b->m_nID;
        ar >> b->m_nStyle;
        ar >> img;
        ar >> b->m_strText;
        ar >> u; b->m_bUserButton = u;
        ar >> reserved;
        ar >> u; b->m_bText = u;
        ar >> u; b->m_bImage = u;
        ar >> u; b->m_bVisible = u;
        if (b->m_bUserButton) b->m_iUserImage = img; else b->m_iImage = img;
    }
}

void tbb_CopyFrom(CMFCToolBarButton* d, const CMFCToolBarButton* s) {
    d->m_nID = s->m_nID;
    d->m_nStyle = s->m_nStyle;
    d->m_dwdItemData = s->m_dwdItemData;
    d->m_strText = s->m_strText;
    d->m_strTextCustom = s->m_strTextCustom;
    d->m_bUserButton = s->m_bUserButton;
    d->m_bText = s->m_bText;
    d->m_bImage = s->m_bImage;
    d->m_bWrap = s->m_bWrap;
    d->m_bWholeText = s->m_bWholeText;
    d->m_bTextBelow = s->m_bTextBelow;
    d->m_bDragFromCollection = s->m_bDragFromCollection;
    d->m_iImage = s->m_iImage;
    d->m_iUserImage = s->m_iUserImage;
    d->m_bLocked = s->m_bLocked;
    d->m_bIsHidden = s->m_bIsHidden;
    d->m_bDisableFill = s->m_bDisableFill;
    d->m_bExtraSize = s->m_bExtraSize;
    d->m_bHorz = s->m_bHorz;
    d->m_bVisible = s->m_bVisible;
}

int  tbb_CompareWith(const CMFCToolBarButton* a, const CMFCToolBarButton* b) {
    return a->m_nID == b->m_nID ? TRUE : FALSE;
}

void tbb_SetImage(CMFCToolBarButton* b, int iImage) {
    if (b->m_bUserButton) b->m_iUserImage = iImage; else b->m_iImage = iImage;
}

// ---- 53 MS-ABI vtable slot wrappers ----
// Behavioral slots delegate to the shared bodies; stub slots return the SDK-header
// inline defaults. Unused trailing args ride in registers and are harmlessly ignored.
CRuntimeClass* MS_ABI v00_GetRuntimeClass(const void* p) { return impl__GetRuntimeClass_CMFCToolBarButton__UEBAPEAUCRuntimeClass__XZ(p); }
void* MS_ABI v01_dtor(CMFCToolBarButton* p, unsigned int flags) { p->~CMFCToolBarButton(); if (flags & 1) ::operator delete(p); return p; }
void  MS_ABI v02_Serialize(CMFCToolBarButton* p, CArchive* ar) { tbb_Serialize(p, ar); }
void  MS_ABI v03_AssertValid(const void*) {}
void  MS_ABI v04_Dump(const void*, void*) {}
BOOL  MS_ABI v05_PrepareDrag(void*, void*) { return FALSE; }
BOOL  MS_ABI v06_CanBeDropped(void*, void*) { return TRUE; }
void  MS_ABI v07_CopyFrom(CMFCToolBarButton* p, const CMFCToolBarButton* s) { tbb_CopyFrom(p, s); }
void  MS_ABI v08_OnDraw(void*, void*, const void*, void*, BOOL, BOOL, BOOL, BOOL, BOOL) {}
SIZE  MS_ABI v09_OnCalculateSize(void*, void*, const SIZE* sizeDefault, BOOL) { return sizeDefault ? *sizeDefault : SIZE{0,0}; }
BOOL  MS_ABI v10_OnClick(void*, void*, BOOL) { return FALSE; }
BOOL  MS_ABI v11_OnClickUp(void*) { return FALSE; }
void  MS_ABI v12_OnChangeParentWnd(CMFCToolBarButton* p, CWnd* pWndParent) { p->m_pWndParent = pWndParent; }
BOOL  MS_ABI v13_ExportToMenuButton(const void*, void*) { return FALSE; }
void  MS_ABI v14_OnMove(void*) {}
void  MS_ABI v15_OnSize(void*, int) {}
HWND  MS_ABI v16_GetHwnd(void*) { return NULL; }
BOOL  MS_ABI v17_CanBeStretched(const void*) { return FALSE; }
BOOL  MS_ABI v18_NotifyCommand(void*, int) { return FALSE; }
void  MS_ABI v19_OnAddToCustomizePage(void*) {}
HBRUSH MS_ABI v20_OnCtlColor(void*, void*, UINT) { return NULL; }
void  MS_ABI v21_OnDblClk(void*, void*) {}
BOOL  MS_ABI v22_CanBeStored(const void*) { return TRUE; }
BOOL  MS_ABI v23_HaveHotBorder(const void*) { return TRUE; }
void  MS_ABI v24_OnCancelMode(void*) {}
void  MS_ABI v25_OnGlobalFontsChanged(void*) {}
BOOL  MS_ABI v26_IsEditable(const void*) { return TRUE; }
BOOL  MS_ABI v27_OnContextHelp(void*, void*) { return FALSE; }
BOOL  MS_ABI v28_OnCustomizeMenu(void*, void*) { return FALSE; }
int   MS_ABI v29_OnDrawOnCustomizeList(void*, void*, const void*, BOOL) { return 0; }
BOOL  MS_ABI v30_IsDroppedDown(const void*) { return FALSE; }
BOOL  MS_ABI v31_OnBeforeDrag(const void*) { return TRUE; }
BOOL  MS_ABI v32_OnBeforeDrop(void*, void*) { return TRUE; }
BOOL  MS_ABI v33_OnToolHitTest(void*, const void*, void*) { return FALSE; }
void  MS_ABI v34_SaveBarState(void*) {}
void  MS_ABI v35_OnShow(void*, BOOL) {}
CRect MS_ABI v36_GetInvalidateRect(const CMFCToolBarButton* p) { return p->m_rect; }
void  MS_ABI v37_SetStyle(CMFCToolBarButton* p, UINT nStyle) { p->m_nStyle = nStyle; }
void  MS_ABI v38_ResetImageToDefault(void*) { /* base class: no-op (harvested) */ }
int   MS_ABI v39_CompareWith(const CMFCToolBarButton* p, const CMFCToolBarButton* o) { return tbb_CompareWith(p, o); }
void  MS_ABI v40_EnableWindow(void*, BOOL) {}
BOOL  MS_ABI v41_IsWindowVisible(void*) { return FALSE; }
BOOL  MS_ABI v42_IsOwnerOf(void*, HWND) { return FALSE; }
BOOL  MS_ABI v43_HasFocus(const void*) { return FALSE; }
BOOL  MS_ABI v44_OnGetCustomToolTipText(void*, void*) { return FALSE; }
BOOL  MS_ABI v45_OnUpdateToolTip(void*, void*, int, void*, void*) { return FALSE; }
BOOL  MS_ABI v46_IsFirstInGroup(const void*) { return FALSE; }
BOOL  MS_ABI v47_IsLastInGroup(const void*) { return FALSE; }
BOOL  MS_ABI v48_SetACCData(void*, void*, void*) { return FALSE; }
int   MS_ABI v49_GetAccCount(void*) { return 0; }
void  MS_ABI v50_SetImage(CMFCToolBarButton* p, int iImage) { tbb_SetImage(p, iImage); }
void  MS_ABI v51_SetRadio(void*) {}
BOOL  MS_ABI v52_IsExtraSize(const CMFCToolBarButton* p) { return p->m_bExtraSize; }

void* const g_CMFCToolBarButton_msvtbl[53] = {
    (void*)v00_GetRuntimeClass,   (void*)v01_dtor,               (void*)v02_Serialize,          (void*)v03_AssertValid,
    (void*)v04_Dump,              (void*)v05_PrepareDrag,        (void*)v06_CanBeDropped,       (void*)v07_CopyFrom,
    (void*)v08_OnDraw,            (void*)v09_OnCalculateSize,    (void*)v10_OnClick,            (void*)v11_OnClickUp,
    (void*)v12_OnChangeParentWnd, (void*)v13_ExportToMenuButton, (void*)v14_OnMove,             (void*)v15_OnSize,
    (void*)v16_GetHwnd,           (void*)v17_CanBeStretched,     (void*)v18_NotifyCommand,      (void*)v19_OnAddToCustomizePage,
    (void*)v20_OnCtlColor,        (void*)v21_OnDblClk,           (void*)v22_CanBeStored,        (void*)v23_HaveHotBorder,
    (void*)v24_OnCancelMode,      (void*)v25_OnGlobalFontsChanged,(void*)v26_IsEditable,        (void*)v27_OnContextHelp,
    (void*)v28_OnCustomizeMenu,   (void*)v29_OnDrawOnCustomizeList,(void*)v30_IsDroppedDown,    (void*)v31_OnBeforeDrag,
    (void*)v32_OnBeforeDrop,      (void*)v33_OnToolHitTest,      (void*)v34_SaveBarState,       (void*)v35_OnShow,
    (void*)v36_GetInvalidateRect, (void*)v37_SetStyle,           (void*)v38_ResetImageToDefault,(void*)v39_CompareWith,
    (void*)v40_EnableWindow,      (void*)v41_IsWindowVisible,    (void*)v42_IsOwnerOf,          (void*)v43_HasFocus,
    (void*)v44_OnGetCustomToolTipText,(void*)v45_OnUpdateToolTip,(void*)v46_IsFirstInGroup,     (void*)v47_IsLastInGroup,
    (void*)v48_SetACCData,        (void*)v49_GetAccCount,        (void*)v50_SetImage,           (void*)v51_SetRadio,
    (void*)v52_IsExtraSize,
};

} // namespace

// Install the MSVC-layout vptr so drop-in MSVC clients dispatch virtuals correctly.
// Called from the exported CMFCToolBarButton constructors (thunks.cpp). Returns pObj.
extern "C" void* OpenMFC_PatchToolBarButtonVtable(void* pObj) {
    if (pObj) *(const void**)pObj = (const void*)&g_CMFCToolBarButton_msvtbl[0];
    return pObj;
}
extern "C" const void* OpenMFC_ToolBarButtonVtableAddr() { return (const void*)&g_CMFCToolBarButton_msvtbl[0]; }

// ---- named exports: real bodies (flip the weak null stubs to real behavior) ----
// Symbol: ?Serialize@CMFCToolBarButton@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMFCToolBarButton__UEAAXAEAVCArchive___Z(void* pThis, CArchive* ar) {
    tbb_Serialize((CMFCToolBarButton*)pThis, ar);
}
// Symbol: ?CopyFrom@CMFCToolBarButton@@UEAAXAEBV1@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(void* pThis, const CMFCToolBarButton* src) {
    tbb_CopyFrom((CMFCToolBarButton*)pThis, src);
}
// Symbol: ?CompareWith@CMFCToolBarButton@@UEBAHAEBV1@@Z
extern "C" int MS_ABI impl__CompareWith_CMFCToolBarButton__UEBAHAEBV1__Z(const void* pThis, const CMFCToolBarButton* other) {
    return tbb_CompareWith((const CMFCToolBarButton*)pThis, other);
}
// Symbol: ?SetImage@CMFCToolBarButton@@UEAAXH@Z
extern "C" void MS_ABI impl__SetImage_CMFCToolBarButton__UEAAXH_Z(void* pThis, int iImage) {
    tbb_SetImage((CMFCToolBarButton*)pThis, iImage);
}
// Symbol: ?ResetImageToDefault@CMFCToolBarButton@@UEAAXXZ
extern "C" void MS_ABI impl__ResetImageToDefault_CMFCToolBarButton__UEAAXXZ(void*) {
    /* base class: no-op (harvested from real mfc140u) */
}
