#pragma once
// Shared internals of the former global_cmfctoolbarbutton_msvtbl.cpp translation unit.
// Definitions live in detail/CMFCToolBarButtonSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {} } }
using namespace openmfc::detail::cmfctoolbarbutton;
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

namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
void tbb_Serialize(CMFCToolBarButton* b, CArchive* par);
} } }

namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
void tbb_CopyFrom(CMFCToolBarButton* d, const CMFCToolBarButton* s);
} } }

namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
int  tbb_CompareWith(const CMFCToolBarButton* a, const CMFCToolBarButton* b);
} } }

namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
void tbb_SetImage(CMFCToolBarButton* b, int iImage);
} } }

// ---- 53 MS-ABI vtable slot wrappers ----
// Behavioral slots delegate to the shared bodies; stub slots return the SDK-header
// inline defaults. Unused trailing args ride in registers and are harmlessly ignored.
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
CRuntimeClass* MS_ABI v00_GetRuntimeClass(const void* p);
} } }
// Qualified (CMFCToolBarButton::) destructor call is statically bound, so it does NOT
// re-dispatch through vptr[1] (== this very wrapper) and recurse. Same guard the CFile
// spike uses. flags&1 is the MSVC "deleting" bit.
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
void* MS_ABI v01_dtor(CMFCToolBarButton* p, unsigned int flags);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
void  MS_ABI v02_Serialize(CMFCToolBarButton* p, CArchive* ar);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
void  MS_ABI v03_AssertValid(const void* pThis);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
void  MS_ABI v04_Dump(const void* pThis, void*);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
BOOL  MS_ABI v05_PrepareDrag(CMFCToolBarButton* p, void*);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
BOOL  MS_ABI v06_CanBeDropped(CMFCToolBarButton* p, void*);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
void  MS_ABI v07_CopyFrom(CMFCToolBarButton* p, const CMFCToolBarButton* s);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
void  MS_ABI v08_OnDraw(void* pThis, void* pDC, const void* pRect, void* pImageList, BOOL bHighlight, BOOL bPushed, BOOL bChecked, BOOL bDisabled, BOOL bFocused);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
SIZE  MS_ABI v09_OnCalculateSize(void*, void*, const SIZE* sizeDefault, BOOL);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
BOOL  MS_ABI v10_OnClick(CMFCToolBarButton* p, void*, BOOL);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
BOOL  MS_ABI v11_OnClickUp(CMFCToolBarButton* p);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
void  MS_ABI v12_OnChangeParentWnd(CMFCToolBarButton* p, CWnd* pWndParent);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
BOOL  MS_ABI v13_ExportToMenuButton(CMFCToolBarButton* p, void*);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
void  MS_ABI v14_OnMove(void* pThis);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
void  MS_ABI v15_OnSize(void* pThis, int nSize);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
HWND  MS_ABI v16_GetHwnd(CMFCToolBarButton* p);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
BOOL  MS_ABI v17_CanBeStretched(CMFCToolBarButton* p);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
BOOL  MS_ABI v18_NotifyCommand(CMFCToolBarButton* p, int nID);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
void  MS_ABI v19_OnAddToCustomizePage(void* pThis);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
HBRUSH MS_ABI v20_OnCtlColor(void*, void*, UINT nCtlType);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
void  MS_ABI v21_OnDblClk(void* pThis, void*);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
BOOL  MS_ABI v22_CanBeStored(CMFCToolBarButton* p);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
BOOL  MS_ABI v23_HaveHotBorder(const CMFCToolBarButton* p);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
void  MS_ABI v24_OnCancelMode(void* pThis);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
void  MS_ABI v25_OnGlobalFontsChanged(void* pThis);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
BOOL  MS_ABI v26_IsEditable(const CMFCToolBarButton* p);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
BOOL  MS_ABI v27_OnContextHelp(CMFCToolBarButton* p, void*);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
BOOL  MS_ABI v28_OnCustomizeMenu(CMFCToolBarButton* p, void*);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
int   MS_ABI v29_OnDrawOnCustomizeList(CMFCToolBarButton* p, void*, const void*, BOOL);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
BOOL  MS_ABI v30_IsDroppedDown(const CMFCToolBarButton* p);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
BOOL  MS_ABI v31_OnBeforeDrag(const CMFCToolBarButton* p);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
BOOL  MS_ABI v32_OnBeforeDrop(CMFCToolBarButton* p, void*);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
BOOL  MS_ABI v33_OnToolHitTest(CMFCToolBarButton* p, const void*, void*);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
void  MS_ABI v34_SaveBarState(void* pThis);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
void  MS_ABI v35_OnShow(CMFCToolBarButton* p, BOOL bShow);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
CRect MS_ABI v36_GetInvalidateRect(const CMFCToolBarButton* p);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
void  MS_ABI v37_SetStyle(CMFCToolBarButton* p, UINT nStyle);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
void  MS_ABI v38_ResetImageToDefault(void*);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
int   MS_ABI v39_CompareWith(const CMFCToolBarButton* p, const CMFCToolBarButton* o);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
void  MS_ABI v40_EnableWindow(CMFCToolBarButton* p, BOOL bEnable);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
BOOL  MS_ABI v41_IsWindowVisible(CMFCToolBarButton* p);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
BOOL  MS_ABI v42_IsOwnerOf(CMFCToolBarButton* p, HWND hwnd);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
BOOL  MS_ABI v43_HasFocus(const CMFCToolBarButton* p);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
BOOL  MS_ABI v44_OnGetCustomToolTipText(CMFCToolBarButton* p, void*);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
BOOL  MS_ABI v45_OnUpdateToolTip(CMFCToolBarButton* p, void*, int, void*, void*);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
BOOL  MS_ABI v46_IsFirstInGroup(CMFCToolBarButton* p);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
BOOL  MS_ABI v47_IsLastInGroup(CMFCToolBarButton* p);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
BOOL  MS_ABI v48_SetACCData(CMFCToolBarButton* p, void*, void*);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
int   MS_ABI v49_GetAccCount(CMFCToolBarButton* p);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
void  MS_ABI v50_SetImage(CMFCToolBarButton* p, int iImage);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
void  MS_ABI v51_SetRadio(void* pThis);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
BOOL  MS_ABI v52_IsExtraSize(const CMFCToolBarButton* p);
} } }

namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
extern void* const g_CMFCToolBarButton_msvtbl[53];
} } }


// Install the MSVC-layout vptr so drop-in MSVC clients dispatch virtuals correctly.
// Called from the exported CMFCToolBarButton constructors (thunks.cpp). Returns pObj.
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
extern "C" void* OpenMFC_PatchToolBarButtonVtable(void* pObj);
} } }
namespace openmfc { namespace detail { namespace cmfctoolbarbutton {
extern "C" const void* OpenMFC_ToolBarButtonVtableAddr();
} } }

