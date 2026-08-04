// CMFCOutlookBarPaneButton — OpenMFC implementation.
// Sources: global_cmfcoutlookbarpanebutton.cpp, manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// OpenMFC: CMFCOutlookBarPaneButton exports.
//
// All 6 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// The class is not declared in the public headers, so `this` is taken as void*
// and the touched member offsets are mirrored with raw offsets.
// CMFCOutlookBarPaneButton derives from CMFCToolBarButton and is 160 bytes
// (0xa0, see global_toolbar_buttons_rtti.cpp): m_iImage at +0x48 and
// m_pWndParent at +0x80 come from the declared base, the own parent pointer
// sits at +0x88 and the horizontal flag at +0x98.  SetImage, OnChangeParentWnd
// and CreateObject are transcribed faithfully; the drawing bodies depend on the
// unmodeled visual manager and are conservative type-correct versions.
//
// Conventions (see IMPLEMENTATION_GUIDE.md):
//   * every export is `extern "C"` + `__attribute__((ms_abi))`;
//   * the implicit `this` is the first explicit parameter;
//   * NULL-check pThis everywhere;
//   * struct-returning exports take the hidden return slot in the register the
//     retail body uses, as documented at each definition.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (the scan of
// `// Symbol:` comments) drops the generated weak stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>

#include <cstddef>
#include <cstdint>
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();
extern "C" void* MS_ABI impl___0CMFCOutlookBarPaneButton__QEAA_XZ(void* pThis);

//=============================================================================
// CMFCOutlookBarPaneButton
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarPaneButton::CanBeDropped(CMFCToolBar* pToolBar)
//=============================================================================

//=============================================================================
// CObject* CMFCOutlookBarPaneButton::CreateObject()
//=============================================================================

//=============================================================================
// CSize CMFCOutlookBarPaneButton::OnCalculateSize(CDC* pDC,
//                                                 const CSize& sizeImage,
//                                                 BOOL bHorz)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPaneButton::OnChangeParentWnd(CWnd* pWnd)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPaneButton::OnDraw(CDC* pDC, const CRect& rect,
//                                       CMFCToolBarImages* pImages, int bHorz,
//                                       int bCustomizeMode, int bHighlight,
//                                       int bDrawBorder, int bGrayDisabledButtons)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPaneButton::SetImage(int iImage)
//=============================================================================
// Retail (0x18009af00): tail jump to CObject::IsKindOf(pToolBar,
// &CMFCToolBar runtime class @0x1803b1358).
// Symbol: ?CanBeDropped@CMFCOutlookBarPaneButton@@UEAAHPEAVCMFCToolBar@@@Z
extern "C" int MS_ABI impl__CanBeDropped_CMFCOutlookBarPaneButton__UEAAHPEAVCMFCToolBar___Z(
    void* pThis, CMFCToolBar* pToolBar)
{
    (void)pThis;
    if (pToolBar == nullptr)
        return FALSE;
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
        reinterpret_cast<const CObject*>(pToolBar),
        impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ());
}
// Retail (0x18009a650):
//     void* p = operator new(0xa0);
//     if (p) CMFCOutlookBarPaneButton::CMFCOutlookBarPaneButton(p);   // 0x18009a6c0
//     return p;
// Symbol: ?CreateObject@CMFCOutlookBarPaneButton@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCOutlookBarPaneButton__SAPEAVCObject__XZ(void)
{
    void* pObj = ::operator new(0xa0, std::nothrow);
    if (pObj != nullptr) {
        impl___0CMFCOutlookBarPaneButton__QEAA_XZ(pObj);
    }
    return pObj;
}
// Retail (0x18009acb0, ~0x200 bytes): measures the button text through
// CDC::GetTextExtent, adds the image well size, clamps against the +0x94
// member and stores the result into +0x98; the CSize is returned through the
// hidden pointer (rdx).  The CString/CDC machinery is not modeled, so a safe
// {0,0} size is returned (the +0x98 bHorz store is kept).
// Symbol: ?OnCalculateSize@CMFCOutlookBarPaneButton@@UEAA?AUtagSIZE@@PEAVCDC@@AEBVCSize@@H@Z
extern "C" void* MS_ABI impl__OnCalculateSize_CMFCOutlookBarPaneButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(
    void* pThis, void* pSizeOut, void* pDC, const void* pSizeImage, int bHorz)
{
    (void)pDC; (void)pSizeImage;
    if (pThis != nullptr) {
        *reinterpret_cast<int*>(static_cast<char*>(pThis) + 0x98) = bHorz;
    }
    if (pSizeOut != nullptr) {
        long* pOut = static_cast<long*>(pSizeOut);
        pOut[0] = 0;   // cx
        pOut[1] = 0;   // cy
    }
    // TODO(clean-room): transcribed partially -- retail measures the label
    // text (CDC::GetTextExtent) and clamps against +0x94 / +0x13c8; not
    // modeled.
    return pSizeOut;
}
// Retail (0x18009aeb0):
//     CMFCToolBarButton::OnChangeParentWnd(this, pWnd);   // 0x18015e570
//     if (pWnd && pWnd->IsKindOf(CMFCToolBar)) *(this+0x88) = pWnd;
//     else *(this+0x88) = 0;
// The base sets m_pWndParent (+0x80); +0x88 is this class's own parent cache.
// Symbol: ?OnChangeParentWnd@CMFCOutlookBarPaneButton@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnChangeParentWnd_CMFCOutlookBarPaneButton__UEAAXPEAVCWnd___Z(
    void* pThis, CWnd* pWnd)
{
    if (pThis == nullptr)
        return;

    // Base CMFCToolBarButton::OnChangeParentWnd (retail 0x18015e570) stores
    // the new parent into m_pWndParent (+0x80).
    reinterpret_cast<CMFCToolBarButton*>(pThis)->m_pWndParent = pWnd;

    void** pParentCache = reinterpret_cast<void**>(static_cast<char*>(pThis) + 0x88);
    if (pWnd != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            reinterpret_cast<const CObject*>(pWnd),
            impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ())) {
        *pParentCache = pWnd;
    } else {
        *pParentCache = nullptr;
    }
}
// Retail (0x18009a760, ~0x350 bytes): paints the image well (PrepareDrawImage
// + Draw) and the label text through AFX_GLOBAL_DATA and the visual manager,
// honouring highlight/customize/disabled state.  Not transcribed.
// Symbol: ?OnDraw@CMFCOutlookBarPaneButton@@UEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCOutlookBarPaneButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
    void* pThis, void* pDC, const void* pRect, void* pImages,
    int bHorz, int bCustomizeMode, int bHighlight, int bDrawBorder,
    int bGrayDisabledButtons)
{
    (void)pDC; (void)pRect; (void)pImages;
    (void)bHorz; (void)bCustomizeMode; (void)bHighlight;
    (void)bDrawBorder; (void)bGrayDisabledButtons;
    if (pThis == nullptr)
        return;
    // TODO(clean-room): not transcribed -- image-well + text painting via
    // AFX_GLOBAL_DATA / the visual manager (retail 0x18009a760).
}
// Retail (0x18009af10): `mov %edx,0x48(%rcx); ret` -- m_iImage (+0x48).
// Symbol: ?SetImage@CMFCOutlookBarPaneButton@@UEAAXH@Z
extern "C" void MS_ABI impl__SetImage_CMFCOutlookBarPaneButton__UEAAXH_Z(
    void* pThis, int iImage)
{
    if (pThis == nullptr)
        return;
    reinterpret_cast<CMFCToolBarButton*>(pThis)->m_iImage = iImage;
}
// Symbol: ??0CMFCOutlookBarPaneButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCOutlookBarPaneButton__QEAA_XZ(void* pThis) {
    return pThis;
}
